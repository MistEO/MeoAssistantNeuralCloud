# MeoAssistance

A game assistance for NeuralCloud

新赛道！一款云图计划的游戏辅助，供PC端安卓模拟器使用，也勉强兼容安卓手机，正在开发中……

目前还没想好要做哪些功能，主要是作者才刚玩这游戏（x，移植自[Meo明日方舟辅助](https://github.com/MistEO/MeoAssistance-Arknights)

## 下载地址

暂无

## 功能介绍

暂无

### 模拟器支持

#### 蓝叠模拟器

完美支持

#### MuMu模拟器

完美支持

#### MuMu手游助手（星云引擎）  

不支持，MuMu不响应SendMessage鼠标消息，而且这个版本不支持adb控制，正在想办法……

#### 其他模拟器

其他模拟器已停止支持，若有需要，欢迎给我提[ISSUE](https://github.com/MistEO/MeoAssistance/issues)，会根据情况尽量适配~

#### 安卓手机/平板

正在开发中……

## 使用说明

### 基本说明

1. 在支持的模拟器设置中，打开`ADB`选项，蓝叠中该选项名称为`允许ADB连接`，MUMU模拟器默认就是打开的
2. 解压压缩包，到**没有中文或特殊符号**的文件夹路径
3. 第一次运行辅助，**请使用管理员权限**打开`MeoAsstGui.exe`。运行过一次后，后续不再需要管理员权限（之后的版本会尝试完全去掉管理员权限）
4. 运行期间，模拟器窗口可以最小化，全屏玩游戏、看视频等，完全不影响

### 不使用模拟器，而是USB连接安卓手机

1. 下载[ADB程序](https://dl.google.com/android/repository/platform-tools-latest-windows.zip)，将`platform-tools`文件夹解压到`MeoAsstGui.exe`的同级目录
2. 使用USB有线连接安卓手机和电脑
3. 请在手机`设置`-`开发者选项`中打开`USB调试`、`USB调试（安全设置）`两个选项。具体操作方式不同品牌手机各不相同，请自行百度查询
4. 请手动修改`resource\config.json`文件中，`options`.`connectType`为`1`。文件保存后请重新打开程序

### 不使用模拟器，而是无线连接安卓手机

1. 下载[ADB程序](https://dl.google.com/android/repository/platform-tools-latest-windows.zip)，将`platform-tools`文件夹解压到`MeoAsstGui.exe`的同级目录
2. 请保证安卓手机和电脑处于同一局域网中
3. 请在手机`设置`-`开发者选项`中打开`USB调试`、`USB调试（安全设置）`、`无线调试`三个选项。具体操作方式不同品牌手机各不相同，请自行百度查询
4. 请手动修改`resource\config.json`文件中，`options`.`connectType`为`2`
5. 请手动修改`resource\config.json`文件中，`options`.`connectRemoteAddress`为无线调试的IP地址和端口号。文件保存后请重新打开程序

**警告：`无线调试`是非常危险的选项，若局域网中存在其他攻击性设备或程序，可能导致手机数据泄露或丢失，请勿在公共的WLAN中使用该功能！**

## Todo

## 致谢

### 开源库

- 图像识别库：[opencv](https://github.com/opencv/opencv.git)
- 文字识别库：[chineseocr_lite](https://github.com/DayBreak-u/chineseocr_lite.git)
- C++ JSON库：[meojson](https://github.com/MistEO/meojson.git)
- WPF MVVW框架：[Stylet](https://github.com/canton7/Stylet)
- WPF控件库：[HandyControl](https://github.com/HandyOrg/HandyControl)
- C# JSON库: [Newtonsoft.Json](https://github.com/JamesNK/Newtonsoft.Json)

### 贡献/参与者

## 广告

[B站直播间](https://live.bilibili.com/2808861)：每晚直播敲代码，近期很长一段时间应该都是在写本辅助软件  
[QQ群](https://jq.qq.com/?_wv=1027&k=ypbzXcA2)：直播间关联Q群，欢迎加入~