#include "AsstCaller.h"
#include <stdio.h>
#include <iostream>

int main(int argc, char** argv)
{
    void* ptr = AsstCreate();
    auto ret = AsstCatchDefault(ptr);
    if (!ret) {
        std::cout << "连接失败，若为第一次使用，请尝试使用管理员权限运行" << std::endl;
        getchar();
        if (ptr) {
            AsstDestory(ptr);
            ptr = nullptr;
        }
        return -1;
    }

    AsstStartProcessTask(ptr, "FightBegin");

    std::cout << "运行中，按任意键即可退出程序" << std::endl;
    getchar();

    if (ptr) {
        AsstDestory(ptr);
        ptr = nullptr;
    }

    return 0;
}