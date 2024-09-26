#ifndef LEDEYE_H
#define LEDEYE_H

#define NUM_LEDS 20
#define LED_L_PIN 16
#define LED_R_PIN 17

#include <Arduino.h>
#include <FastLED.h>

class LedEyes
{
private:
    // 内部常量值
    const int LED_BRIGNTNESS_INIT = 128;       // 0~255
    const CRGB color24_1 = CRGB(50, 149, 183); // 湖蓝色

    // 类的内部变量
    CRGB leds_l[NUM_LEDS];                  // 最后赋值给WS2812的CRGB值
    CRGB leds_r[NUM_LEDS];                  // 最后赋值给WS2812的CRGB值
    CRGB led_CRGBcolor_current = color24_1; // 当前颜色的RGB值
    float led_brightness = 0.2;             // 当前亮度0.0~1.0
    int eyes_blink_palse_ms = 5000;         // 眨眼睛间隔时间
    int eyes_blink_delay_ms = 10;           // 眨眼睛task CRGB值刷新的间隔时间

    // 内部标志变量
    bool flag_eyes_blink = true; // 眨眼睛标志位

public:
    LedEyes();                                                 // 构造函数
    ~LedEyes();                                                // 析构函数
    void init();                                               // 初始化LED
    void update();                                             // 更新所有LED的CRGB值到WS2812
    void setLeds2SingleColor(CRGB leds[NUM_LEDS], CRGB color); // 设置LED为单一颜色
    static void taskEyesBlink(void *pvParameters);             // task create 静态成员函数
    void taskEyesBlinkImpl();                                  // 实际的眨眼睛任务实现
    void enableEyesBlink(bool flag);
};

#endif // LEDEYE_H
