#include "ledeyes.h"

LedEyes::LedEyes()
{
    // 初始化类时，给leds_l和leds_r赋初始值
    // for (int i = 0; i < NUM_LEDS; i++)
    // {
    //     leds_l[i] = CRGB::Black;
    //     leds_r[i] = CRGB::Black;
    //     leds_color_l[i] = CRGB::Black;
    //     leds_color_r[i] = CRGB::Black;
    //     leds_colortrans_l[i] = CRGB::Black;
    //     leds_colortrans_r[i] = CRGB::Black;
    //     leds_britrans_l[i] = CRGB::Black;
    //     leds_britrans_r[i] = CRGB::Black;
    // }
    setLeds2SingleColor(leds_l, leds_r, CRGB::Black);
    setLeds2SingleColor(leds_color_l, leds_color_r, CRGB::Black);
    setLeds2SingleColor(leds_colortrans_l, leds_colortrans_r, CRGB::Black);
    setLeds2SingleColor(leds_britrans_l, leds_britrans_r, CRGB::Black);

    FastLED.addLeds<WS2812B, LED_L_PIN, GRB>(leds_l, NUM_LEDS);
    FastLED.setBrightness(FastLED_BRIGNTNESS_INIT);
    FastLED.addLeds<WS2812B, LED_R_PIN, GRB>(leds_r, NUM_LEDS);
    FastLED.setBrightness(FastLED_BRIGNTNESS_INIT);
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
    led_brightness = LED_BRIGHTNESS_INIT;
    colorTransSpeed = LED_COLORTRANS_SPEED_INIT;
    color_shift_delay_ms = COLOR_SHIFT_DELAY_MS_INIT;

    eyes_blink_mode = BLINK_ON_RANDOM;
    flag_eyes_blink = false;

    flag_eyes_bri_gradient = true;
    // flag_eyes_bri_gradient = false;

    // flag_eyes_colortrans = true;
    flag_eyes_colortrans = false;

    color_shift_mode = COLOR_SHIFT_ON;

    // for (int i = 0; i < NUM_LEDS; i++)
    // {
    //     leds_color_l[i] = led_CRGBcolor_init;
    //     leds_color_r[i] = led_CRGBcolor_init;
    // }
    setLeds2SingleColor(leds_color_l, leds_color_r, led_CRGBcolor_init);

    // for (int i = 0; i < NUM_LEDS; i++)
    // {
    //     leds_colortrans_l[i] = leds_color_l[i];
    //     leds_colortrans_r[i] = leds_color_r[i];
    // }
    setLedsColorsDiff(leds_colorshift_l, leds_colorshift_r, leds_color_l, leds_color_r);
    setLedsColorsDiff(leds_colortrans_l, leds_colortrans_r, leds_colorshift_l, leds_colorshift_r);
}

void LedEyes::update()
{
    // setLeds2SingleColor(led_CRGBcolor_current);

    setLedsBrightness();

    // for (int led_index = 0; led_index < NUM_LEDS; led_index++)
    // {
    //     leds_l[led_index] = leds_britrans_l[led_index];
    //     leds_r[led_index] = leds_britrans_r[led_index];
    // }
    setLedsColorsSame(leds_l, leds_r, leds_britrans_l);

    FastLED.show();
}

void LedEyes::setLeds2SingleColor(CRGB leds_l[], CRGB leds_r[], CRGB color)
{
    for (int i = 0; i < NUM_LEDS; i++)
    {
        leds_l[i] = color;
        leds_r[i] = color;
    }
}

void LedEyes::setLedsColorsSame(CRGB leds_l[], CRGB leds_r[], CRGB colors[])
{
    for (int i = 0; i < NUM_LEDS; i++)
    {
        leds_l[i] = colors[i];
        leds_r[i] = colors[i];
    }
}

void LedEyes::setLedsColorsDiff(CRGB leds_l[], CRGB leds_r[], CRGB colors_l[], CRGB colors_r[])
{
    for (int i = 0; i < NUM_LEDS; i++)
    {
        leds_l[i] = colors_l[i];
        leds_r[i] = colors_r[i];
    }
}

void LedEyes::setLedsBrightness()
{
    // 整体调整亮度
    for (int i = 0; i < NUM_LEDS; i++)
    {
        leds_britrans_l[i] = briScale(leds_colortrans_l[i], led_brightness);
        leds_britrans_r[i] = briScale(leds_colortrans_r[i], led_brightness);
    }

    // 设置亮度上下渐变效果
    if (flag_eyes_bri_gradient)
    {
        float bri_gradient_l[NUM_LEDS] = {
            0.0, 0.0, 0.0, 0.0, 0.05, 0.1, 0.25, 0.5, 0.8, 1.0, 1.0, 0.8, 0.5, 0.25, 0.1, 0.05, 0.0, 0.0, 0.0, 0.0};
        float bri_gradient_r[NUM_LEDS] = {
            0.0, 0.0, 0.0, 0.0, 0.05, 0.1, 0.25, 0.5, 0.8, 1.0, 1.0, 0.8, 0.5, 0.25, 0.1, 0.05, 0.00, 0.0, 0.0, 0.0};
        // 亮度渐变
        for (int i = 0; i < NUM_LEDS; i++)
        {
            leds_britrans_l[i] = briScale(leds_britrans_l[i], bri_gradient_l[i]);
            leds_britrans_r[i] = briScale(leds_britrans_r[i], bri_gradient_r[i]);
        }
    }
}

CRGB hsv2Rgb(uint16_t h, uint8_t s, uint8_t v)
{
    // 参考AP2102库的例程Rainbow.ino
    uint8_t f = (h % 60) * 255 / 60;
    uint8_t p = (255 - s) * (uint16_t)v / 255;
    uint8_t q = (255 - f * (uint16_t)s / 255) * (uint16_t)v / 255;
    uint8_t t = (255 - (255 - f) * (uint16_t)s / 255) * (uint16_t)v / 255;
    uint8_t r = 0, g = 0, b = 0;
    switch ((h / 60) % 6)
    // 对输入做冗余处理
    {
    case 0:
        r = v;
        g = t;
        b = p;
        break;
    case 1:
        r = q;
        g = v;
        b = p;
        break;
    case 2:
        r = p;
        g = v;
        b = t;
        break;
    case 3:
        r = p;
        g = q;
        b = v;
        break;
    case 4:
        r = t;
        g = p;
        b = v;
        break;
    case 5:
        r = v;
        g = p;
        b = q;
        break;
    }
    return CRGB(r, g, b);
}

CRGB briScale(CRGB color, float scale)
{
    CRGB color_new;
    color_new.r = color.r * scale;
    color_new.r = color_new.r > 255 ? 255 : color_new.r;
    color_new.r = color_new.r < 0 ? 0 : color_new.r;

    color_new.g = color.g * scale;
    color_new.g = color_new.g > 255 ? 255 : color_new.g;
    color_new.g = color_new.g < 0 ? 0 : color_new.g;

    color_new.b = color.b * scale;
    color_new.b = color_new.b > 255 ? 255 : color_new.b;
    color_new.b = color_new.b < 0 ? 0 : color_new.b;

    return color_new;
}