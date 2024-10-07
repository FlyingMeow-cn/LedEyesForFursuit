#ifndef LEDEYE_H
#define LEDEYE_H

#define NUM_LEDS 20
#define LED_L_PIN 2
#define LED_R_PIN 4

#include <Arduino.h>
#include <FastLED.h>

class LedEyes
{
private:
    // 内部常量值
    const int FastLED_BRIGNTNESS_INIT = 128;   // 0~255
    const float LED_BRIGNTNESS_INIT = 0.2;     // 0.0~1.0
    const CRGB color24_1 = CRGB(50, 149, 183); // 湖蓝色

    // 类的内部变量

    // 内部标志变量

public:
    // 初始颜色->变换颜色->亮度变换->眨眼效果(最终颜色)
    CRGB leds_color_l[NUM_LEDS];      // 未经变换的初始CRGB值
    CRGB leds_color_r[NUM_LEDS];      // 未经变换的初始CRGB值
    CRGB leds_colortrans_l[NUM_LEDS]; // 颜色变换后的CRGB值
    CRGB leds_colortrans_r[NUM_LEDS]; // 颜色变换后的CRGB值
    CRGB leds_britrans_l[NUM_LEDS];   // 亮度变换后的CRGB值
    CRGB leds_britrans_r[NUM_LEDS];   // 亮度变换后的CRGB值
    CRGB leds_l[NUM_LEDS];            // 最后赋值给WS2812的CRGB值
    CRGB leds_r[NUM_LEDS];            // 最后赋值给WS2812的CRGB值

    CRGB led_CRGBcolor_current = color24_1; // 当前颜色的RGB值
    float led_brightness = 0.9;             // 当前亮度0.0~1.0

    int timer_ms = 0;               // 内部计时器变量，每10ms加1
    int eyes_blink_palse_ms = 5000; // 眨眼睛间隔时间
    int eyes_blink_delay_ms = 10;   // 眨眼睛task CRGB值刷新的间隔时间

    bool flag_eyes_blink = true;         // 眨眼睛标志位
    bool flag_eyes_bri_gradient = false; // 眼睛颜色渐变标志位

    LedEyes();  // 构造函数
    ~LedEyes(); // 析构函数

    void init();                          // 初始化LED
    void update();                        // 更新所有LED的CRGB值到WS2812
    void setLeds2SingleColor(CRGB color); // 设置LED为单一颜色
    void setLedsBrightness(float bri);    // 设置LED亮度

    void enableEyesBlink(bool flag);
    void enableEyesBriGradient(bool flag);
};

#endif // LEDEYE_H
