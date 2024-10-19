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

    // led_CRGBcolor_current = color24_1;
    led_brightness = LED_BRIGNTNESS_INIT;
    timer_ms = 0;
    // flag_eyes_blink = true;
    flag_eyes_blink = false;
    flag_eyes_bri_gradient = true;
    // flag_eyes_bri_gradient = false;

    // setLeds2SingleColor(led_CRGBcolor_current);

    for (int i = 0; i < NUM_LEDS; i++)
    {
        leds_color_l[i] = led_CRGBcolor_init;
        leds_color_r[i] = led_CRGBcolor_init;
    }

    for (int i = 0; i < NUM_LEDS; i++)
    {
        leds_colortrans_l[i] = leds_color_l[i];
        leds_colortrans_r[i] = leds_color_r[i];
    }
}

void LedEyes::update()
{
    // setLeds2SingleColor(led_CRGBcolor_current);

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

// 实现彩虹效果
// 注：程序上，亮度的时变效果也可以使用相同思路实现，使用HSV转换
void taskLedsColorTrans(void *pvParameters)
{
    LedEyes &ledEyes = *(LedEyes *)pvParameters;
    while (1)
    {
        int colorTransSpeed = 5;
        // int time = millis() * colorTransSpeed / 100;
        // time = time % 256 / 256.0 * 360.0;
        int time = millis();
        
        float r_scale = 0.2;
        float r_offset = 0.3;
        float g_scale = 0.8;
        float g_offset = 0.1;
        float b_scale = 0.5;
        float b_offset = 0.5;  

        float r_phi = 0;
        float g_phi = 0.4*PI;
        float b_phi = 1.2*PI;


        for (int led_index = 0; led_index < NUM_LEDS; led_index++)
        {
            // float h_scale = 0.5;
            // float h_offset = 0.5;
            // float p = sin(2*PI*time/1000.0 *colorTransSpeed  - float(led_index/NUM_LEDS) * 2 * PI);
            // int h = ((p+1)/2 * h_scale + h_offset)*256;
            // ledEyes.leds_color_l[led_index] = hsv2Rgb(h, 255, 255);
            // ledEyes.leds_color_r[led_index] = hsv2Rgb(h, 255, 255);      
            float r_sin = sin(2*PI*time/10000.0 *colorTransSpeed  - float(led_index)/NUM_LEDS * 2 * PI + r_phi);
            int r = ((r_sin+1)/2 * r_scale + r_offset)*256;
            float g_sin = sin(2*PI*time/10000.0 *colorTransSpeed  - float(led_index)/NUM_LEDS * 2 * PI + g_phi);
            int g = ((g_sin+1)/2 * g_scale + g_offset)*256;
            float b_sin = sin(2*PI*time/10000.0 *colorTransSpeed  - float(led_index)/NUM_LEDS * 2 * PI + b_phi);
            int b = ((b_sin+1)/2 * b_scale + b_offset)*256;
            ledEyes.leds_color_l[led_index] = CRGB(r, g, b);
            ledEyes.leds_color_r[led_index] = CRGB(r, g, b);      

        }

        for (int led_index = 0; led_index < NUM_LEDS; led_index++)
        {
            ledEyes.leds_colortrans_l[led_index] = ledEyes.leds_color_l[led_index];
            ledEyes.leds_colortrans_r[led_index] = ledEyes.leds_color_r[led_index];
        }

        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}


// float triangle_wave(float x) {
//     return 2.0 * fabs(x / (2 * PI) - floor(x / (2 * PI) + 0.5));
// }

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