#include "ledeyes.h"

LedEyes::LedEyes()
{
    // 初始化类时，给leds_l和leds_r赋初始值
    for (int i = 0; i < NUM_LEDS; i++)
    {
        leds_l[i] = CRGB::Black;
        leds_r[i] = CRGB::Black;
    }
    xTaskCreate(taskEyesBlink, "taskEyesBlink", 1024, NULL, 1, NULL);
    // xTaskCreate的TaskFunction_t pvTaskCode参数不能为类的成员函数
    // copilot的建议：使用静态成员函数包装成一个普通函数：
    // 你遇到的错误“void (LedEyes::*)(void *pvParameters) 类型的实参与 TaskFunction_t (aka void (*)(void *)) 类型的形参不兼容”
    // 是因为你试图将一个成员函数作为任务函数传递给 xTaskCreate，而 xTaskCreate 期望一个普通的 C 风格函数指针。
    // 成员函数与普通函数指针不兼容，因为成员函数隐含地包含一个 this 指针。要解决这个问题，你可以使用静态成员函数或全局函数来包装你的成员函数。
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


void LedEyes::taskEyesBlink(void* pvParameters)
{
    LedEyes* instance = static_cast<LedEyes*>(pvParameters);
    instance->taskEyesBlinkImpl();
}


void LedEyes::taskEyesBlinkImpl()
{
    while (1)
    {
        CRGB color = led_CRGBcolor_current;
        // 闭眼睛
        for (int i = 0; i < NUM_LEDS / 2; i++)
        {
            for (int led_index = 0; led_index < NUM_LEDS; led_index++)
            {
                if (led_index < i || led_index >= NUM_LEDS - i)
                {
                    leds_l[led_index] = CRGB::Black;
                    leds_r[led_index] = CRGB::Black;
                }
                else
                {
                    leds_l[led_index] = color;
                    leds_r[led_index] = color;
                }
            }
            // FastLED.show();
            vTaskDelay(eyes_blink_delay_ms / portTICK_PERIOD_MS);
        }

        // 睁眼睛
        for (int i = NUM_LEDS / 2 - 1; i >= 0; i--)
        {
            for (int led_index = 0; led_index < NUM_LEDS; led_index++)
            {
                if (led_index < i || led_index >= NUM_LEDS - i)
                {
                    leds_l[led_index] = CRGB::Black;
                    leds_r[led_index] = CRGB::Black;
                }
                else
                {
                    leds_l[led_index] = color;
                    leds_r[led_index] = color;
                }
            }
            // FastLED.show();
            vTaskDelay(eyes_blink_delay_ms / portTICK_PERIOD_MS);
        }
        // Serial.println("眨眼睛任务");
        vTaskDelay(eyes_blink_palse_ms / portTICK_PERIOD_MS);
    }
}

void LedEyes::enableEyesBlink(bool flag)
{
    flag_eyes_blink = flag;
}
