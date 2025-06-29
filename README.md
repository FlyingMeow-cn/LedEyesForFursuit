# LedEyesForFursuit
 Designed by FlyingMeow, for electric eyes on fursuit



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

- 增加蓝牙指令功能：控制颜色切换时间
- 增加功能：语音唤醒成功会眨眼
- 增加亮度时变的呼吸灯效果
  - 语音指令：控制开关
  - 蓝牙指令：控制开关、切换速度
