# MeoAssistance

## 很抱歉，已按相关方要求，删除本辅助源码。给游戏制作组和玩家朋友们带来困扰，还望谅解，谢谢~

A game assistance for NeuralCloud

新赛道！一款云图计划的游戏辅助，供PC端安卓模拟器使用，也勉强兼容安卓手机，正在开发中……

目前还没想好要做哪些功能，主要是作者才刚玩这游戏（x，移植自[Meo明日方舟辅助](https://github.com/MistEO/MeoAssistance-Arknights)

## 功能介绍

初版效果演示：<https://www.bilibili.com/video/BV1rR4y1p7XG>

- 自动刷图
- 其他优势
    - 所有点击操作，都是点击按钮内随机位置，并模拟泊松分布（按钮偏中间位置点的概率大，越往旁边点到的概率越小）
    - 点击操作支持手动设置随机延时，没有封号风险~
    - 模拟器窗口可以被遮挡、可以最小化、甚至可以老板键隐藏！即使全屏看视频、玩游戏，也完全不影响辅助运行
- 支持多款主流模拟器
- 勉强兼容安卓手机（USB调试、无线调试）
- 自适应分辨率及屏幕缩放
- 未来更多功能见[Todo](#Todo)

## 使用说明

1. 解压压缩包，到**没有中文或特殊符号**的文件夹路径
2. 游戏打开关卡选择界面，有橙色`准备作战`按钮的界面；或游戏打开编辑队伍，有橙色`作战开始`的界面
3. 第一次运行辅助，**请使用管理员权限**打开`TestCaller.exe`。运行过一次后，后续不再需要管理员权限（之后的版本会尝试完全去掉管理员权限）
4. 运行期间，模拟器窗口可以最小化，全屏玩游戏、看视频等，完全不影响

详细说明请参考`dev`分支

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

暂无

## 广告

[B站直播间](https://live.bilibili.com/2808861)：每晚直播敲代码，近期很长一段时间应该都是在写本辅助软件  
[QQ群](https://jq.qq.com/?_wv=1027&k=ypbzXcA2)：直播间关联Q群，欢迎加入~
