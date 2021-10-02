#include "Assistance.h"

#include <time.h>
#include <filesystem>

#include <json.h>
#include <opencv2/opencv.hpp>

#include "WinMacro.h"
#include "Configer.h"
#include "Identify.h"
#include "Logger.hpp"
#include "AsstAux.h"
#include "Task.h"
#include "TaskConfiger.h"
#include "UserConfiger.h"

using namespace asst;

Assistance::Assistance(AsstCallback callback, void* callback_arg)
    : m_callback(callback), m_callback_arg(callback_arg)
{
    DebugTraceFunction;

    // 检查返回值，若为false则回调错误
    auto callback_error = [&](const std::string& filename = std::string()) {
        if (m_callback == nullptr) {
            return;
        }
        json::value callback_json;
        callback_json["filename"] = filename;
        callback_json["what"] = "Resource";
        m_callback(AsstMsg::InitFaild, callback_json, m_callback_arg);
    };

    bool ret = Configer::get_instance().load(GetResourceDir() + "config.json");
    if (!ret) {
        callback_error();
        return;
    }
    ret = TaskConfiger::get_instance().load(GetResourceDir() + "tasks.json");
    if (!ret) {
        callback_error();
        return;
    }

    m_identify_ptr = std::make_shared<Identify>();
    for (const auto& [name, info] : TaskConfiger::get_instance().m_all_tasks_info)
    {
        if (info->algorithm != AlgorithmType::MatchTemplate)
        {
            continue;
        }
        std::string filename = std::dynamic_pointer_cast<MatchTaskInfo>(info)->template_filename;
        ret = m_identify_ptr->add_image(name, GetResourceDir() + "template\\" + filename);
        if (!ret) {
            callback_error();
            return;
        }
    }

    m_identify_ptr->set_ocr_param(Configer::get_instance().m_options.ocr_gpu_index, Configer::get_instance().m_options.ocr_thread_number);
    ret = m_identify_ptr->ocr_init_models(GetResourceDir() + "OcrLiteOnnx\\models\\");
    if (!ret) {
        callback_error();
        return;
    }

    ret = UserConfiger::get_instance().load(GetCurrentDir() + "user.json");
    if (!ret) {
        callback_error();
        return;
    }

    m_controller_ptr = std::make_shared<WinMacro>();

    m_working_thread = std::thread(std::bind(&Assistance::working_proc, this));
    m_msg_thread = std::thread(std::bind(&Assistance::msg_proc, this));
}

Assistance::~Assistance()
{
    DebugTraceFunction;

    //if (m_controller_ptr != nullptr) {
    //	m_controller_ptr->show_window();
    //}

    m_thread_exit = true;
    m_thread_idle = true;
    m_condvar.notify_all();
    m_msg_condvar.notify_all();

    if (m_working_thread.joinable()) {
        m_working_thread.join();
    }
    if (m_msg_thread.joinable()) {
        m_msg_thread.join();
    }
}

bool asst::Assistance::catch_default()
{
    DebugTraceFunction;

    switch (Configer::get_instance().m_options.connect_type)
    {
    case ConnectType::Emulator:
        return catch_emulator();
    case ConnectType::USB:
        return catch_usb();
    case ConnectType::Remote:
        return catch_remote(Configer::get_instance().m_options.connect_remote_address);
    default:
        return false;
    }
}

bool Assistance::catch_emulator(const std::string& emulator_name)
{
    DebugTraceFunction;

    stop();

    bool ret = false;
    //std::string cor_name = emulator_name;

    std::unique_lock<std::mutex> lock(m_mutex);

    // 自动匹配模拟器，逐个找
    if (emulator_name.empty()) {
        for (const auto& [name, info] : Configer::get_instance().m_handles) {
            ret = m_controller_ptr->try_capture(info);
            if (ret) {
                //cor_name = name;
                break;
            }
        }
    }
    else { // 指定的模拟器
        ret = m_controller_ptr->try_capture(Configer::get_instance().m_handles[emulator_name]);
    }

    m_inited = ret;
    return ret;
}

bool asst::Assistance::catch_usb()
{
    DebugTraceFunction;

    stop();

    bool ret = false;

    std::unique_lock<std::mutex> lock(m_mutex);

    EmulatorInfo remote_info = Configer::get_instance().m_handles["USB"];

    ret = m_controller_ptr->try_capture(remote_info, true);

    m_inited = ret;
    return ret;
}

bool asst::Assistance::catch_remote(const std::string& address)
{
    DebugTraceFunction;

    stop();

    bool ret = false;

    std::unique_lock<std::mutex> lock(m_mutex);

    EmulatorInfo remote_info = Configer::get_instance().m_handles["Remote"];
    remote_info.adb.connect = StringReplaceAll(remote_info.adb.connect, "[Address]", address);
    remote_info.adb.click = StringReplaceAll(remote_info.adb.click, "[Address]", address);
    remote_info.adb.swipe = StringReplaceAll(remote_info.adb.swipe, "[Address]", address);
    remote_info.adb.display = StringReplaceAll(remote_info.adb.display, "[Address]", address);
    remote_info.adb.screencap = StringReplaceAll(remote_info.adb.screencap, "[Address]", address);

    ret = m_controller_ptr->try_capture(remote_info, true);

    m_inited = ret;
    return ret;
}

bool Assistance::start_process_task(const std::string& task, int retry_times, bool block)
{
    DebugTraceFunction;
    DebugTrace("Start |", task, block ? "block" : "non block");
    if (!m_thread_idle || !m_inited)
    {
        return false;
    }

    std::unique_lock<std::mutex> lock;
    if (block)
    {
        lock = std::unique_lock<std::mutex>(m_mutex);
        //clear_exec_times();
        m_identify_ptr->clear_cache();
    }

    append_match_task(task, { task }, retry_times);

    m_thread_idle = false;
    m_condvar.notify_one();

    return true;
}

#ifdef LOG_TRACE
bool Assistance::start_debug_task()
{
    DebugTraceFunction;
    if (!m_thread_idle || !m_inited)
    {
        return false;
    }

    std::unique_lock<std::mutex> lock(m_mutex);

    // append task to queue

    m_thread_idle = false;
    m_condvar.notify_one();

    return true;
}
#endif

void Assistance::stop(bool block)
{
    DebugTraceFunction;
    DebugTrace("Stop |", block ? "block" : "non block");

    m_thread_idle = true;

    std::unique_lock<std::mutex> lock;
    if (block)
    { // 外部调用
        lock = std::unique_lock<std::mutex>(m_mutex);
    }
    decltype(m_tasks_deque) empty;
    m_tasks_deque.swap(empty);
    clear_exec_times();
    m_identify_ptr->clear_cache();
}

bool Assistance::set_param(const std::string& type, const std::string& param, const std::string& value)
{
    DebugTraceFunction;
    DebugTrace("SetParam |", type, param, value);

    return TaskConfiger::get_instance().set_param(type, param, value);
}

void Assistance::working_proc()
{
    DebugTraceFunction;
    auto p_this = this;

    int retry_times = 0;
    while (!m_thread_exit)
    {
        //DebugTraceScope("Assistance::working_proc Loop");
        std::unique_lock<std::mutex> lock(m_mutex);

        if (!m_thread_idle && !m_tasks_deque.empty())
        {
            //m_controller_ptr->set_idle(false);

            auto start_time = std::chrono::system_clock::now();
            std::shared_ptr<AbstractTask> task_ptr = m_tasks_deque.front();
            // 先pop出来，如果执行失败再还原回去
            m_tasks_deque.pop_front();

            task_ptr->set_ptr(m_controller_ptr, m_identify_ptr);
            task_ptr->set_exit_flag(&m_thread_idle);
            bool ret = task_ptr->run();
            if (ret)
            {
                retry_times = 0;
            }
            else
            {
                // 失败了累加失败次数，超限了再pop
                if (retry_times >= task_ptr->get_retry_times())
                {
                    json::value task_error_json;
                    task_error_json["retry_limit"] = task_ptr->get_retry_times();
                    task_error_json["retry_times"] = retry_times;
                    task_error_json["task_chain"] = task_ptr->get_task_chain();
                    task_callback(AsstMsg::TaskError, std::move(task_error_json), p_this);

                    retry_times = 0;
                }
                else
                {
                    ++retry_times;
                    // 执行失败再还原回去
                    m_tasks_deque.emplace_front(task_ptr);
                    task_ptr->on_run_fails(retry_times);
                }
            }

            int& delay = Configer::get_instance().m_options.task_delay;
            m_condvar.wait_until(lock, start_time + std::chrono::milliseconds(delay),
                [&]() -> bool { return m_thread_idle; });
        }
        else
        {
            m_thread_idle = true;
            //m_controller_ptr->set_idle(true);
            m_condvar.wait(lock);
        }
    }
}

void Assistance::msg_proc()
{
    DebugTraceFunction;

    while (!m_thread_exit)
    {
        //DebugTraceScope("Assistance::msg_proc Loop");
        std::unique_lock<std::mutex> lock(m_msg_mutex);
        if (!m_msg_queue.empty())
        {
            // 结构化绑定只能是引用，后续的pop会使引用失效，所以需要重新构造一份，这里采用了move的方式
            auto&& [temp_msg, temp_detail] = m_msg_queue.front();
            AsstMsg msg = std::move(temp_msg);
            json::value detail = std::move(temp_detail);
            m_msg_queue.pop();
            lock.unlock();

            if (m_callback)
            {
                m_callback(msg, detail, m_callback_arg);
            }
        }
        else
        {
            m_msg_condvar.wait(lock);
        }
    }
}

void Assistance::task_callback(AsstMsg msg, const json::value& detail, void* custom_arg)
{
    DebugTrace("Assistance::task_callback |", msg, detail.to_string());

    Assistance* p_this = (Assistance*)custom_arg;
    json::value more_detail = detail;
    switch (msg)
    {
    case AsstMsg::PtrIsNull:
    case AsstMsg::ImageIsEmpty:
        p_this->stop(false);
        break;
        //case AsstMsg::WindowMinimized:
        //	p_this->m_controller_ptr->show_window();
        //	break;
    case AsstMsg::AppendProcessTask:
        more_detail["type"] = "ProcessTask";
        [[fallthrough]];
    case AsstMsg::AppendTask:
        p_this->append_task(more_detail, true);
        return;	// 这俩消息Assistance会新增任务，外部不需要处理，直接拦掉
        break;
    default:
        break;
    }

    // Todo: 有些不需要回调给外部的消息，得在这里给拦截掉
    // 加入回调消息队列，由回调消息线程外抛给外部
    p_this->append_callback(msg, std::move(more_detail));
}

void asst::Assistance::append_match_task(
    const std::string& task_chain, const std::vector<std::string>& tasks, int retry_times, bool front)
{
    auto task_ptr = std::make_shared<ProcessTask>(task_callback, (void*)this);
    task_ptr->set_task_chain(task_chain);
    task_ptr->set_tasks(tasks);
    task_ptr->set_retry_times(retry_times);
    if (front) {
        m_tasks_deque.emplace_front(task_ptr);
    }
    else {
        m_tasks_deque.emplace_back(task_ptr);
    }
}

void asst::Assistance::append_task(const json::value& detail, bool front)
{
    std::string task_type = detail.at("type").as_string();
    std::string task_chain = detail.get("task_chain", "");
    int retry_times = detail.get("retry_times", INT_MAX);

    if (task_type == "ProcessTask")
    {
        std::vector<std::string> next_vec;
        if (detail.exist("tasks"))
        {
            json::array next_arr = detail.at("tasks").as_array();
            for (const json::value& next_json : next_arr)
            {
                next_vec.emplace_back(next_json.as_string());
            }
        }
        else if (detail.exist("task"))
        {
            next_vec.emplace_back(detail.at("task").as_string());
        }
        append_match_task(task_chain, next_vec, retry_times, front);
    }
    // else if  // TODO
}

void asst::Assistance::append_callback(AsstMsg msg, json::value detail)
{
    std::unique_lock<std::mutex> lock(m_msg_mutex);
    m_msg_queue.emplace(msg, std::move(detail));
    m_msg_condvar.notify_one();
}

void Assistance::clear_exec_times()
{
    for (auto&& pair : TaskConfiger::get_instance().m_all_tasks_info)
    {
        pair.second->exec_times = 0;
    }
}