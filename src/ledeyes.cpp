#include "ledeyes.h"

LedEyes::LedEyes()
{
    // 初始化类时，给leds_l和leds_r赋初始值
    for (int i = 0; i < NUM_LEDS; i++)
    {
        leds_l[i] = CRGB::Black;
        leds_r[i] = CRGB::Black;
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
    FastLED.setBrightness(LED_BRIGNTNESS_INIT);
    FastLED.addLeds<WS2812B, LED_R_PIN, GRB>(leds_r, NUM_LEDS);
    FastLED.setBrightness(LED_BRIGNTNESS_INIT);
}


void LedEyes::enableEyesBlink(bool flag)
{
    flag_eyes_blink = flag;
}
