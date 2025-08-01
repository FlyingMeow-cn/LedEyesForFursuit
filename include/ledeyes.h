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

CRGB hsv2Rgb(uint16_t h, uint8_t s, uint8_t v);
CRGB briScale(CRGB color, float scale);

enum EyesBlinkMode
{
    BLINK_OFF,         // 关闭眨眼
    BLINK_ON_CONSTANT, // 恒定眨眼时间间隔
    BLINK_ON_RANDOM    // 伪随机时间间隔
};

enum ColorShiftMode
{
    COLOR_SHIFT_OFF,
    COLOR_SHIFT_ON,
    COLOR_SHIFT_ON_INVERSE
};

class LedEyes
{
private:
    // 内部常量值
    const int FastLED_BRIGNTNESS_INIT = 255; // 0~255

    // const float LED_BRIGHTNESS_INIT = 0.05;  // 0.0~1.0  // 调试用
    const float LED_BRIGHTNESS_INIT = 0.5;      // 0.0~1.0

    const float LED_BRIFACTOR_TIMESHIFT_INIT = 1.0;
    const float LED_BRI_TIMESHIFT_T_MS_INIT = 5 * 1000; // 亮度时变效果的周期时间（毫秒）

    // 颜色切换列表
    const CRGB color24_1 = CRGB(50, 149, 183);   // 0 湖蓝色
    const CRGB color24_2 = CRGB(38, 122, 253);   // 1 蓝色
    const CRGB color24_3 = CRGB(50, 0, 255);     // 2 蓝紫色
    const CRGB color24_4 = CRGB(255, 0, 255);    // 3 紫色
    const CRGB color24_5 = CRGB(241, 97, 146);   // 4 粉色
    const CRGB color24_6 = CRGB(230, 57, 54);    // 5 红色
    const CRGB color24_7 = CRGB(254, 90, 36);    // 6 橙色
    const CRGB color24_8 = CRGB(254, 168, 40);   // 7 橙黄色
    const CRGB color24_9 = CRGB(253, 239, 200);  // 8 白色
    const CRGB color24_10 = CRGB(223, 233, 116); // 9 浅黄色
    const CRGB color24_11 = CRGB(129, 203, 197); // 10 绿色
    const CRGB color24_12 = CRGB(130, 223, 237); // 11 青色

    const int LED_COLORTRANS_SPEED_INIT = 4; // 颜色变换速度

    // 类的内部变量

    // 内部标志变量

public:
    CRGB leds_color_l[NUM_LEDS];      // 未经变换的初始CRGB值（颜色切换的目标颜色值）
    CRGB leds_color_r[NUM_LEDS];      // 未经变换的初始CRGB值
    CRGB leds_colorshift_l[NUM_LEDS]; // 颜色切换后的CRGB值（颜色切换的当前颜色值）
    CRGB leds_colorshift_r[NUM_LEDS]; // 颜色切换后的CRGB值
    CRGB leds_colortrans_l[NUM_LEDS]; // 颜色变换后的CRGB值（效果：单个眼睛的不同LED呈现渐变效果）
    CRGB leds_colortrans_r[NUM_LEDS]; // 颜色变换后的CRGB值
    CRGB leds_britrans_l[NUM_LEDS];   // 亮度变换后的CRGB值
    CRGB leds_britrans_r[NUM_LEDS];   // 亮度变换后的CRGB值
    CRGB leds_l[NUM_LEDS];            // 最后赋值给WS2812的CRGB值
    CRGB leds_r[NUM_LEDS];            // 最后赋值给WS2812的CRGB值

    const CRGB led_CRGBcolor_init = color24_1;      // 初始颜色的RGB值
    float led_brightness = LED_BRIGHTNESS_INIT;     // 当前亮度0.0~1.0
    float led_brifactor_timeshift = LED_BRIFACTOR_TIMESHIFT_INIT; // 亮度时变效果中间变量
    float led_bri_timeshift_T_ms = LED_BRI_TIMESHIFT_T_MS_INIT;      // 亮度时变效果的周期时间（毫秒）
    bool flag_bri_breathe = false;                  // 亮度呼吸效果标志位

    // const int eyes_blink_palse_ms[10] = {5000, 3500, 6000, 4000, 7000, 4500, 8000, 5000, 9000, 5500}; // 眨眼睛间隔时间（普通版）
    const int eyes_blink_palse_ms[10] = {9000, 5500, 7000, 4000, 7000, 8000, 8500, 5500, 9000, 8500}; // 眨眼睛间隔时间（加长版）
    // int eyes_blink_palse_ms = 5000; // 眨眼睛间隔时间
    const int eyes_blink_delay_ms = 30;                // 眨眼睛task CRGB值刷新的间隔时间
    // const int eyes_blink_delay_ms = 20;                // 眨眼睛task CRGB值刷新的间隔时间
    bool flag_eyes_blink = true;                       // 眨眼睛标志位
    EyesBlinkMode eyes_blink_mode = BLINK_ON_CONSTANT; // 眨眼睛标志位

    bool flag_eyes_bri_gradient = true; // 眼睛颜色渐变标志位

    bool flag_eyes_colortrans = false;               // 眼睛颜色变换标志位
    int colorTransSpeed = LED_COLORTRANS_SPEED_INIT; // 颜色变换速度

    const int color_shift_update_pluse_ms = 50;
    int color_shift_mode = COLOR_SHIFT_OFF; // 颜色过渡模式
    const int COLOR_SHIFT_DELAY_MS_INIT = 1000 * 30; // 颜色切换延时
    // const int COLOR_SHIFT_DELAY_MS_INIT = 1000 * 2;       // 颜色切换延时  调试用
    int color_shift_delay_ms = COLOR_SHIFT_DELAY_MS_INIT; // 颜色切换延时

    const float bri_seq[8] = {0.03, 0.05, 0.1, 0.2, 0.3, 0.5, 0.7, 1.0};
    const int bri_seq_len = 8;
    const int bri_seq_idx_rst = 5;
    int bri_seq_idx = bri_seq_idx_rst;

    const CRGB color24_seq[12] = {color24_1, color24_2, color24_3, color24_4, color24_5, color24_6, color24_7, color24_8, color24_9, color24_10, color24_11, color24_12};
    const int color24_seq_len = 12;
    const int color24_seq_idx_rst = 0;
    int color_seq_idx = color24_seq_idx_rst;
    int color_seq_delta_idx = 1; // 颜色序列变化的步长 1或-1

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

