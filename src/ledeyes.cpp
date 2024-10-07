#include "ledeyes.h"

LedEyes::LedEyes()
{
    // 初始化类时，给leds_l和leds_r赋初始值
    for (int i = 0; i < NUM_LEDS; i++)
    {
        leds_l[i] = CRGB::Black;
        leds_r[i] = CRGB::Black;
        leds_color_l[i] = CRGB::Black;
        leds_color_r[i] = CRGB::Black;
        leds_colortrans_l[i] = CRGB::Black;
        leds_colortrans_r[i] = CRGB::Black;
        leds_britrans_l[i] = CRGB::Black;
        leds_britrans_r[i] = CRGB::Black;
    }
}

LedEyes::~LedEyes()
{
}

/// @brief 初始化LED
/// @param 无
/// @return 无
/// @todo
void LedEyes::init()
{
    FastLED.addLeds<WS2812B, LED_L_PIN, GRB>(leds_l, NUM_LEDS);
    FastLED.setBrightness(FastLED_BRIGNTNESS_INIT);
    FastLED.addLeds<WS2812B, LED_R_PIN, GRB>(leds_r, NUM_LEDS);
    FastLED.setBrightness(FastLED_BRIGNTNESS_INIT);

    led_CRGBcolor_current = color24_1;
    led_brightness = LED_BRIGNTNESS_INIT;
    timer_ms = 0;
    flag_eyes_blink = true;
    setLeds2SingleColor(led_CRGBcolor_current);

}

void LedEyes::update()
{
    setLeds2SingleColor(led_CRGBcolor_current);

    for(int i = 0; i < NUM_LEDS; i++)
    {
        leds_colortrans_l[i] = leds_color_l[i];
        leds_colortrans_r[i] = leds_color_r[i];
    }

    for(int i = 0; i < NUM_LEDS; i++)
    {
        leds_britrans_l[i] = leds_colortrans_l[i];
        leds_britrans_r[i] = leds_colortrans_r[i];
    }

    // setLedsBrightness(led_brightness);
    FastLED.show();
}

void LedEyes::setLeds2SingleColor(CRGB color)
{
    for (int i = 0; i < NUM_LEDS; i++)
    {
        leds_color_l[i] = color;
        leds_color_r[i] = color;
    }
}

void LedEyes::setLedsBrightness(float bri)
{
    // 整合到亮度变换效果函数中
    // if (bri > 1.0)
    // {
    //     bri = 1.0;
    // }
    // else if (bri < 0.0)
    // {
    //     bri = 0.0;
    // }
    // // CRGB color = led_CRGBcolor_current;
    // for (int i = 0; i < NUM_LEDS; i++)
    // {
    //     leds_l[i].r = leds_blink_l[i].r * bri;
    //     leds_l[i].g = leds_blink_l[i].g * bri;
    //     leds_l[i].b = leds_blink_l[i].b * bri;
    //     leds_r[i].r = leds_blink_r[i].r * bri;
    //     leds_r[i].g = leds_blink_r[i].g * bri;
    //     leds_r[i].b = leds_blink_r[i].b * bri;
    // }
}

void LedEyes::enableEyesBlink(bool flag)
{
    flag_eyes_blink = flag;
}
