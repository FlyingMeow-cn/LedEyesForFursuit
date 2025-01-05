#ifndef LEDEYE_H
#define LEDEYE_H

#define NUM_LEDS 20

#define LED_L_PIN 2
#define LED_R_PIN 4
// #define LED_L_PIN 16
// #define LED_R_PIN 17

#include <Arduino.h>
#include <FastLED.h>
#include <math.h>

void taskEyesBlink(void *pvParameters); // 眨眼睛任务 传入参数为LedEyes对象
void taskEyesUpdate(void *pvParameters);
void taskLedsColorTrans(void *pvParameters);
CRGB hsv2Rgb(uint16_t h, uint8_t s, uint8_t v);
CRGB briScale(CRGB color, float scale);

class LedEyes
{
private:
    // 内部常量值
    const int FastLED_BRIGNTNESS_INIT = 255; // 0~255
    const float LED_BRIGNTNESS_INIT = 0.1;     // 0.0~1.0  // 调试用
    // const float LED_BRIGNTNESS_INIT = 0.5;      // 0.0~1.0
    const CRGB color24_1 = CRGB(50, 149, 183);  // 湖蓝色
    const CRGB color24_2 = CRGB(255, 0, 0);     // 红色 不好看
    const CRGB color24_3 = CRGB(0, 255, 0);     // 绿色 不好看
    const CRGB color24_4 = CRGB(0, 0, 255);     // 蓝色 可以
    const CRGB color24_5 = CRGB(255, 255, 0);   // 黄色 一般
    const CRGB color24_6 = CRGB(255, 0, 255);   // 紫色 一般
    const CRGB color24_7 = CRGB(0, 255, 255);   // 青色 和湖蓝色效果差不多
    const CRGB color24_8 = CRGB(255, 255, 255); // 白色 可以
    const CRGB color24_9 = CRGB(255, 165, 0);   // 橙色  和黄色差不多，一般
    const CRGB color24_10 = CRGB(165, 20, 255); // 紫色 可以
    const CRGB color24_11 = CRGB(20, 165, 255); // 和湖蓝色效果差不多
    const CRGB color24_12 = CRGB(20, 255, 165); // 和湖蓝色效果差不多
    const int LED_COLORTRANS_SPEED_INIT = 4;    // 颜色变换速度

    // 类的内部变量

    // 内部标志变量

public:
    CRGB leds_color_l[NUM_LEDS];      // 未经变换的初始CRGB值
    CRGB leds_color_r[NUM_LEDS];      // 未经变换的初始CRGB值
    CRGB leds_colortrans_l[NUM_LEDS]; // 颜色变换后的CRGB值
    CRGB leds_colortrans_r[NUM_LEDS]; // 颜色变换后的CRGB值
    CRGB leds_britrans_l[NUM_LEDS];   // 亮度变换后的CRGB值
    CRGB leds_britrans_r[NUM_LEDS];   // 亮度变换后的CRGB值
    CRGB leds_l[NUM_LEDS];            // 最后赋值给WS2812的CRGB值
    CRGB leds_r[NUM_LEDS];            // 最后赋值给WS2812的CRGB值

    CRGB led_CRGBcolor_init = color24_1;        // 初始颜色的RGB值
    float led_brightness = LED_BRIGNTNESS_INIT; // 当前亮度0.0~1.0

    int eyes_blink_palse_ms = 5000; // 眨眼睛间隔时间
    int eyes_blink_delay_ms = 20;   // 眨眼睛task CRGB值刷新的间隔时间
    bool flag_eyes_blink = true;    // 眨眼睛标志位

    bool flag_eyes_bri_gradient = true; // 眼睛颜色渐变标志位

    bool flag_eyes_colortrans = false; // 眼睛颜色变换标志位
    int colorTransSpeed = LED_COLORTRANS_SPEED_INIT;          // 颜色变换速度

    LedEyes();  // 构造函数
    ~LedEyes(); // 析构函数

    void init();   // 初始化LED
    void update(); // 更新所有LED的CRGB值到WS2812
    // void setLeds2SingleColor(CRGB color); // 设置LED为单一颜色
    void setLeds2SingleColor(CRGB leds_l[], CRGB leds_r[], CRGB color);
    void setLedsColorsSame(CRGB leds_l[], CRGB leds_r[], CRGB colors[]);
    void setLedsColorsDiff(CRGB leds_l[], CRGB leds_r[], CRGB colors_l[], CRGB colors_r[]);

    void setLedsBrightness(); // 设置LED亮度
};

#endif // LEDEYE_H
