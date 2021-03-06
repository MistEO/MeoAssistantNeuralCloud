# MeoAssistance

A game assistance for NeuralCloud

新赛道！一款云图计划的游戏辅助，供PC端安卓模拟器使用，也勉强兼容安卓手机，正在开发中……

目前还没想好要做哪些功能，主要是作者才刚玩这游戏（x，移植自[Meo明日方舟辅助](https://github.com/MistEO/MeoAssistance-Arknights)

## 项目状态

咕咕咕_(:з」∠)_  

有兴趣的大佬可以fork过去自行修改，基本全是面向json的热流程，需要的话我再整个文档出来  
然后改完如果可以pr过来，我会非常感激的！♪(･ω･)ﾉ

## 下载地址

<https://github.com/MistEO/MeoAssistance-NeuralCloud/releases/latest>

## 功能介绍

初版效果演示：<https://www.bilibili.com/video/BV1rR4y1p7XG>

- 目前的版本是专门用来刷技能枢核的，刷暗域4-3
- 其他优势
    - 所有点击操作，都是点击按钮内随机位置，并模拟泊松分布（按钮偏中间位置点的概率大，越往旁边点到的概率越小）
    - 点击操作支持手动设置随机延时，没有封号风险~
    - 模拟器窗口可以被遮挡、可以最小化、甚至可以老板键隐藏！即使全屏看视频、玩游戏，也完全不影响辅助运行
- 支持多款主流模拟器
- 勉强兼容安卓手机（USB调试、无线调试）
- 自适应分辨率及屏幕缩放
- 未来更多功能见[Todo](#Todo)

### 模拟器支持

### 模拟器支持

#### 蓝叠模拟器

完美支持。需要在模拟器`设置`-`引擎设置`-打开`允许ADB连接`

#### 蓝叠模拟器国际版

完美支持。需要在模拟器`设定`-`进阶`-打开`Android调试桥`

#### 夜神模拟器

完美支持

#### MuMu模拟器

支持

#### 雷电模拟器

支持

#### 逍遥模拟器

支持

#### 腾讯手游助手

不支持，新版本的腾讯好像也是自研引擎了，没开放ADB端口。但是测试是能响应Win32 Api的，有需求再做

#### MuMu手游助手（星云引擎）  

不支持，星云引擎这个版本不响应Win32的SendMessage鼠标消息，而且不支持adb控制，无解_(:з」∠)_

#### 其他模拟器

若有其他需要，欢迎给我提[ISSUE](https://github.com/MistEO/MeoAssistance-NeuralCloud/issues)，会根据情况尽量适配~

#### 安卓手机/平板

正在开发中……

## 使用说明

### 基本说明

1. 在支持的模拟器设置中，打开`ADB`选项，蓝叠中该选项名称为`允许ADB连接`，MUMU模拟器默认就是打开的
2. 解压压缩包，到**没有中文或特殊符号**的文件夹路径
3. 目前的版本是专门用来刷技能枢核的，从暗域4-3界面开始，想刷其他的改一句tasks就行
![4)`$M4VKINZA B}D_P2MLUP](https://user-images.githubusercontent.com/18511905/140064282-d0b4cd0d-f05a-4e4b-b071-24657ebcef36.png)

5. 第一次运行辅助，**请使用管理员权限**打开`TestCaller.exe`。运行过一次后，后续不再需要管理员权限（之后的版本会尝试完全去掉管理员权限）
6. 运行期间，模拟器窗口可以最小化，全屏玩游戏、看视频等，完全不影响
7. 第一次运行不出枢核的话，点击退出需要打勾勾，有需要的大佬可以自行完善一下task，或者干脆不搞，点一次也不费事
![J 7WR 4PCDE0 UX}U}8U{}B](https://user-images.githubusercontent.com/18511905/140064395-e21cb779-c186-4299-9a66-7b1ae3ebff6e.png)


### 设置操作延时

每次点击之前，均随机延时一定的时间，降低封号风险  
该功能默认是关闭的，需要的话请手动打开：请手动修改`resource\config.json`文件中，`options`.`controlDelayRange`字段的值，格式为`[最小延时, 最大延时]`，单位为毫秒，例如想设置3~5秒的随机延时，即设置为`[ 3000, 5000]` 即可。文件保存后请重新打开程序。

![图例](images/controlDelayRange.png)

### 不使用模拟器，而是USB连接安卓手机

1. 下载[ADB程序](https://dl.google.com/android/repository/platform-tools-latest-windows.zip)，将`platform-tools`文件夹解压到`TestCaller.exe`的同级目录
2. 使用USB有线连接安卓手机和电脑
3. 请在手机`设置`-`开发者选项`中打开`USB调试`、`USB调试（安全设置）`两个选项。具体操作方式不同品牌手机各不相同，请自行百度查询
4. 请手动修改`resource\config.json`文件中，`options`.`connectType`为`1`。文件保存后请重新打开程序

### 不使用模拟器，而是无线连接安卓手机

1. 下载[ADB程序](https://dl.google.com/android/repository/platform-tools-latest-windows.zip)，将`platform-tools`文件夹解压到`TestCaller.exe`的同级目录
2. 请保证安卓手机和电脑处于同一局域网中
3. 请在手机`设置`-`开发者选项`中打开`USB调试`、`USB调试（安全设置）`、`无线调试`三个选项。具体操作方式不同品牌手机各不相同，请自行百度查询
4. 请手动修改`resource\config.json`文件中，`options`.`connectType`为`2`
5. 请手动修改`resource\config.json`文件中，`options`.`connectRemoteAddress`为无线调试的IP地址和端口号。文件保存后请重新打开程序

**警告：`无线调试`是非常危险的选项，若局域网中存在其他攻击性设备或程序，可能导致手机数据泄露或丢失，请勿在公共的WLAN中使用该功能！**

## Todo

- [ ] 去掉管理员权限
- [ ] 图形化界面
- [ ] 自动刷图性能优化

## 致谢

### 开源库

- 图像识别库：[opencv](https://github.com/opencv/opencv.git)
- 文字识别库：[chineseocr_lite](https://github.com/DayBreak-u/chineseocr_lite.git)
- C++ JSON库：[meojson](https://github.com/MistEO/meojson.git)

### 贡献/参与者

- 感谢tiernath大佬帮忙重新整理刷图流程~

## 广告

[B站直播间](https://live.bilibili.com/2808861)：每晚直播敲代码，近期很长一段时间应该都是在写本辅助软件  
[QQ群](https://jq.qq.com/?_wv=1027&k=ypbzXcA2)：直播间关联Q群，欢迎加入~
