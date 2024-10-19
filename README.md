# LedEyesForFursuit
 Designed by FlyingMeow, for electric eyes on fursuit



**==【注意！！！】烧录程序前根据硬件引脚检查宏定义==**





`2024-10-19 20:18` 整理程序思路：



**LED颜色显示逻辑：**  （后续使用流程图整理）

初始颜色 `leds_color_l`、`leds_color_r`
↓ 色彩变换
色彩变换后的颜色 `leds_colortrans_l`、`leds_colortrans_r`
↓ 亮度变换
亮度变换后的颜色 `leds_britrans_l`、`leds_britrans_r`
↓ 眨眼睛赋值操作
LED直接显示的颜色 `leds_l`、`leds_r`
↓
FastLED.show()刷新



**主程序main：**

- 初始化、启用task
- 蓝牙串口消息处理



**taskEyesBlink：控制眨眼睛的task**



**taskEyesUpdate：控制LED显示刷新的task**















---

~~LED颜色显示逻辑~~

1. ~~初始CRGB颜色 led_CRGBcolor_current~~
   1. ~~初始颜色时变（初始颜色变换）彩虹效果；~~
   2. ~~加效果（可选）  ；~~
2. ~~亮度调整~~
   1. ~~上亮下暗渐变【已完成】~~
   1. ~~亮度时变效果~~
3. ~~眨眼睛效果【已完成】~~
