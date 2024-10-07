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
        // 123123
        // 123123
        // 123123
        // 123123
        // 123123
    }

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

    led_CRGBcolor_current = color24_1;
    led_brightness = LED_BRIGNTNESS_INIT;
    timer_ms = 0;
    flag_eyes_blink = true;
    setLeds2SingleColor(led_CRGBcolor_current);
}

void LedEyes::update()
{
    setLeds2SingleColor(led_CRGBcolor_current);

    for (int i = 0; i < NUM_LEDS; i++)
    {
        leds_colortrans_l[i] = leds_color_l[i];
        leds_colortrans_r[i] = leds_color_r[i];
    }

    setLedsBrightness();

    for (int led_index = 0; led_index < NUM_LEDS; led_index++)
    {
        leds_l[led_index] = leds_britrans_l[led_index];
        leds_r[led_index] = leds_britrans_r[led_index];
    }

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
            0.0, 0.0, 0.0, 0.0, 0.05, 0.1, 0.25, 0.5, 0.8, 1.0, 1.0, 0.8, 0.5, 0.25, 0.1, 0.05, 0.0, 0.0, 0.0, 0.0
        };
        float bri_gradient_r[NUM_LEDS] = {
            0.0, 0.0, 0.0, 0.0, 0.05, 0.1, 0.25, 0.5, 0.8, 1.0, 1.0, 0.8, 0.5, 0.25, 0.1, 0.05, 0.00, 0.0, 0.0, 0.0
        };
        // 亮度渐变
        for (int i = 0; i < NUM_LEDS; i++)
        {
            leds_britrans_l[i] = briScale(leds_britrans_l[i], bri_gradient_l[i]);
            leds_britrans_r[i] = briScale(leds_britrans_r[i], bri_gradient_r[i]);
        }
    }
}


void taskEyesBlink(void *pvParameters)
{
    LedEyes &ledEyes = *(LedEyes *)pvParameters;
    while (1)
    {
        if (ledEyes.flag_eyes_blink)
        {
            // 闭眼睛
            for (int i = 0; i < NUM_LEDS / 2; i++)
            {
                for (int led_index = 0; led_index < NUM_LEDS; led_index++)
                {
                    if (led_index < i || led_index >= NUM_LEDS - i)
                    {
                        ledEyes.leds_l[led_index] = CRGB::Black;
                        ledEyes.leds_r[led_index] = CRGB::Black;
                    }
                    else
                    {
                        ledEyes.leds_l[led_index] = ledEyes.leds_britrans_l[led_index];
                        ledEyes.leds_r[led_index] = ledEyes.leds_britrans_r[led_index];
                    }
                }
                FastLED.show();
                vTaskDelay(ledEyes.eyes_blink_delay_ms / portTICK_PERIOD_MS);
            }

            // 睁眼睛
            for (int i = NUM_LEDS / 2 - 1; i >= 0; i--)
            {
                for (int led_index = 0; led_index < NUM_LEDS; led_index++)
                {
                    if (led_index < i || led_index >= NUM_LEDS - i)
                    {
                        ledEyes.leds_l[led_index] = CRGB::Black;
                        ledEyes.leds_r[led_index] = CRGB::Black;
                    }
                    else
                    {
                        ledEyes.leds_l[led_index] = ledEyes.leds_britrans_l[led_index];
                        ledEyes.leds_r[led_index] = ledEyes.leds_britrans_r[led_index];
                    }
                }
                FastLED.show();
                vTaskDelay(ledEyes.eyes_blink_delay_ms / portTICK_PERIOD_MS);
            }
            // Serial.println("task Eyes Blink");
            vTaskDelay(ledEyes.eyes_blink_palse_ms / portTICK_PERIOD_MS);
        }
        else
        {
            for (int led_index = 0; led_index < NUM_LEDS; led_index++)
            {
                ledEyes.leds_l[led_index] = ledEyes.leds_britrans_l[led_index];
                ledEyes.leds_r[led_index] = ledEyes.leds_britrans_r[led_index];
            }
            FastLED.show();
            vTaskDelay(ledEyes.eyes_blink_palse_ms / portTICK_PERIOD_MS);
        }
    }
}

void taskEyesUpdate(void *pvParameters)
{
    LedEyes &ledEyes = *(LedEyes *)pvParameters;
    while (1)
    {
        ledEyes.update();
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
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