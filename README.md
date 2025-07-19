# LedEyesForFursuit——开源兽装电子发光眼
 Designed by FlyingMeow, for electric eyes on fursuit



## 项目简介

这个项目是给自家崽子搓的发光电子眼，最早从24年初开始做的，到现在（25年8月）差不多基本的功能都做完了ovo，打算开源出来，如果有感兴趣的小伙伴可以动手复刻，也可以再次基础之上共同做一些改进和尝试！

【展示图片】





### 项目背景

现有发光眼方案研究





### 项目内容

为了克服现有方案缺点，同时设计一款较低成本的发光眼方案，决定采用MCU+WS2812灯珠的方式实现可编程控制的发光眼。



本方案的主要特点：

集成语音控制功能



方便的多模式切换



全灯珠可编程控制

亮度上下渐变 眨眼睛



项目开源内容包括：

- 发光眼主控板及灯板的PCB设计（使用立创EDA专业版设计）
- 发光眼外壳的结构设计（使用Solidworks设计）
- 发光眼的ESP32固件程序源码（使用VScode PlatformIO开发框架）
- ASRPRO语音识别程序的`.hd`文件（使用ASRPRO开发工具天问Bolck）





## 功能概述





## 硬件设计

### 电路设计



### 结构设计





## 软件设计

### 程序架构

![LedEyesForFursuit项目软件架构图-250719](https://gitee.com/FlyingMeow/mypic/raw/master/LedEyesForFursuit%E9%A1%B9%E7%9B%AE%E8%BD%AF%E4%BB%B6%E6%9E%B6%E6%9E%84%E5%9B%BE-250719.jpg)





### 交互逻辑



缺点：代码比较冗长



## 后续开发ideas



- MPU6050
- 光敏电阻
- 手机端上位机
- 音频节奏识别
  - 输入音量饱和





## 开源许可说明





## 附录1 蓝牙交互指令汇总





## 附录2 语音交互指令汇总





## 程序开发日志

**==【注意！！！】烧录程序前根据硬件引脚检查宏定义==**

`2024-10-19 20:18` 整理程序思路：

`2024-10-26 21:37` 整理程序思路：

**LED颜色显示逻辑：**

![image-20241026213716994](https://gitee.com/FlyingMeow/mypic/raw/master/image-20241026213716994.png)





**主程序main：**

- 初始化、启用task
- 蓝牙串口消息处理



**taskEyesBlink：控制眨眼睛的task**



**taskEyesUpdate：控制LED显示刷新的task**



`2025-01-04`

语音识别功能测试



`2025-03-17`

功能实现待办

- 修改眨眼逻辑【已完成】
  - 增加触发变量`ledeyes.trigger_eyes_blink`
  - 使用该变量单次触发眨眼效果，再重新写线程触发flag
  - 模式：关闭眨眼 / 等时间间隔 / 伪随机时间间隔
  - `ledEyes.flag_eyes_blink`标志位的逻辑改到flag的触发线程



- 将ledeyes.color24_1 ~ ledeyes.color24_12 改为数组形式【已完成】
- 优化颜色数组取值



- 增加颜色的平滑切换效果、引出相关参数接口
  - 线性的平滑切换
  - 朝向目标点的速率随距离缓慢下降的平滑切换



- 增加自动颜色切换模式
  - 固定时间，刷新目标颜色值
  - 平滑切换速度放慢



- 补充蓝牙控制接口和语音接口的控制指令





`2025-03-30`

![image-20250330171709657](https://gitee.com/FlyingMeow/mypic/raw/master/image-20250330171709657.png)



颜色值：

50, 149, 183  *湖蓝色*

38, 122, 253

50, 0, 255

255, 0, 255

241, 97, 146

230, 57, 54

254, 90, 36

254, 168, 40

253, 239, 89

223, 233, 116

129, 203, 197

130, 223, 237



`2025-04-12`

蓝牙命令cs inv后颜色不切换的原因找到了：taskLedsColorShiftGradient中与taskLedsColorShift中使用里同一种标志变量做switch判断

修改：taskLedsColorShiftGradient常开



【问题】

- taskEyesUpdate与taskEyesBlink leds_l、leds_r的赋值有冲突，导致眨眼睛会闪（关闭眼睛渐变后十分明显） 如何协调这两个线程？
- 上面的解释图中taskLedsColorShift的解释有误
  - 需要重新整理现在的taskLedsColorShift与taskLedsColorShiftGradient的逻辑

- 偶尔突然闪一下，亮度特别高？



`2025-06-28`

![image-20250628222312532](https://gitee.com/FlyingMeow/mypic/raw/master/image-20250628222312532.png)





`2025-06-29`

功能更新ideas

- 增加蓝牙指令功能：控制颜色切换时间  ✓
- 增加功能：语音唤醒成功会眨眼  ✓
- 增加亮度时变的呼吸灯效果
  - 语音指令：控制开关
  - 蓝牙指令：控制开关、切换速度



`2025-07-02`

增加亮度时变的呼吸灯效果✓

增加蓝牙控制指令：

- 控制呼吸灯效果开关、速度



`2025-07-19`

蓝牙&语音控制指令测试
