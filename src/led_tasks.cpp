#include "ledeyes.h"
#include "led_tasks.h"


/// @brief 任务-LED基础颜色变化
/// @param pvParameters LED眼睛对象的指针
void taskLedsColorShift(void *pvParameters)
{
    LedEyes &ledEyes = *(LedEyes *)pvParameters;
    while (1)
    {
        switch (ledEyes.color_shift_mode)
        {
        case COLOR_SHIFT_OFF:
            // 250511修改bug：在COLOR_SHIFT_OFF状态下，使用clr +/-命令无法切换颜色
            // ledEyes.color_seq_delta_idx = 0;
            break;

        case COLOR_SHIFT_ON:
            ledEyes.color_seq_delta_idx = 1;
            break;

        case COLOR_SHIFT_ON_INVERSE:
            ledEyes.color_seq_delta_idx = -1;
            break;
        }

        if (ledEyes.color_shift_mode != COLOR_SHIFT_OFF)
        {
            ledEyes.color_seq_idx += ledEyes.color_seq_delta_idx;
            if (ledEyes.color_seq_idx >= ledEyes.color24_seq_len)
            {
                ledEyes.color_seq_idx -= ledEyes.color24_seq_len;
            }
            if (ledEyes.color_seq_idx < 0)
            {
                ledEyes.color_seq_idx += ledEyes.color24_seq_len;
            }
            // 250511修改bug：在COLOR_SHIFT_OFF状态下，使用clr命令切换自定义颜色，会在下一时刻切换回颜色列表中的颜色，自定义颜色不能保持
            // 这里加一个判断
            ledEyes.setLeds2SingleColor(ledEyes.leds_color_l, ledEyes.leds_color_r, ledEyes.color24_seq[ledEyes.color_seq_idx]);
        }
        // ledEyes.setLeds2SingleColor(ledEyes.leds_color_l, ledEyes.leds_color_r, ledEyes.color24_seq[ledEyes.color_seq_idx]);

        vTaskDelay(ledEyes.color_shift_delay_ms / portTICK_PERIOD_MS);
    }
}

/// @brief 任务-LED颜色平滑过渡效果
/// @param pvParameters LED眼睛对象的指针
void taskLedsColorShiftGradient(void *pvParameters)
{
    LedEyes &ledEyes = *(LedEyes *)pvParameters;
    static float color_shift_step[3] = {0.0, 0.0, 0.0};
    float step_factor = 0.2;

    while (1)
    {
        color_shift_step[0] = (ledEyes.leds_color_l[0].r - ledEyes.leds_colorshift_l[0].r) * step_factor;
        color_shift_step[1] = (ledEyes.leds_color_l[0].g - ledEyes.leds_colorshift_l[0].g) * step_factor;
        color_shift_step[2] = (ledEyes.leds_color_l[0].b - ledEyes.leds_colorshift_l[0].b) * step_factor;

        for (int led_idx = 0; led_idx < NUM_LEDS; led_idx++)
        {
            ledEyes.leds_colorshift_l[led_idx].r += color_shift_step[0];
            ledEyes.leds_colorshift_l[led_idx].g += color_shift_step[1];
            ledEyes.leds_colorshift_l[led_idx].b += color_shift_step[2];
            ledEyes.leds_colorshift_r[led_idx].r += color_shift_step[0];
            ledEyes.leds_colorshift_r[led_idx].g += color_shift_step[1];
            ledEyes.leds_colorshift_r[led_idx].b += color_shift_step[2];
        }

        vTaskDelay(50 / portTICK_PERIOD_MS);
    }
}

/// @brief 任务-LED颜色变化效果（效果欠佳，暂时默认不启用）
/// @param pvParameters LED眼睛对象的指针
void taskLedsColorTrans(void *pvParameters)
{
    LedEyes &ledEyes = *(LedEyes *)pvParameters;
    while (1)
    {
        int colorTransSpeed = ledEyes.colorTransSpeed;
        int time = millis();

        // 正弦波幅值、偏移量、相位参数
        // 决定颜色环在RGB三维空间中的位置和形状
        float r_scale = 0.4;
        float r_offset = 0.0;

        float g_scale = 0.8;
        float g_offset = 0.1;

        float b_scale = 0.6;
        float b_offset = 0.4;

        float r_phi = 0;
        float g_phi = 0.4 * PI;
        float b_phi = 1.2 * PI;

        for (int led_index = 0; led_index < NUM_LEDS; led_index++)
        {
            float r_sin = sin(2 * PI * time / 100000.0 * colorTransSpeed - float(led_index) / NUM_LEDS * 2 * PI + r_phi);
            int r = ((r_sin + 1) / 2 * r_scale + r_offset) * 255;
            float g_sin = sin(2 * PI * time / 100000.0 * colorTransSpeed - float(led_index) / NUM_LEDS * 2 * PI + g_phi);
            int g = ((g_sin + 1) / 2 * g_scale + g_offset) * 255;
            float b_sin = sin(2 * PI * time / 100000.0 * colorTransSpeed - float(led_index) / NUM_LEDS * 2 * PI + b_phi);
            int b = ((b_sin + 1) / 2 * b_scale + b_offset) * 255;
            // ledEyes.leds_color_l[led_index] = CRGB(r, g, b);
            // ledEyes.leds_color_r[led_index] = CRGB(r, g, b);
            if (ledEyes.flag_eyes_colortrans)
            {
                ledEyes.leds_colortrans_l[led_index] = CRGB(r, g, b);
                ledEyes.leds_colortrans_r[led_index] = CRGB(r, g, b);
            }
            else
            {
                ledEyes.leds_colortrans_l[led_index] = ledEyes.leds_colorshift_l[led_index];
                ledEyes.leds_colortrans_r[led_index] = ledEyes.leds_colorshift_r[led_index];
            }
        }
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}


/// @brief 任务-LED亮度控制
/// @param pvParameters LED眼睛对象的指针
void taskLedsBriCtrl(void *pvParameters)
{
    LedEyes &ledEyes = *(LedEyes *)pvParameters;
    while (1)
    {
        ledEyes.setLedsBrightness();
        vTaskDelay(50 / portTICK_PERIOD_MS);
    }
}


/// @brief 任务-LED眨眼触发
/// @param pvParameters LED眼睛对象的指针
void taskEyesBlinkTrigger(void *pvParameters)
{
    LedEyes &ledEyes = *(LedEyes *)pvParameters;
    static int blink_pulse_idx = 0;
    while (1)
    {
        switch (ledEyes.eyes_blink_mode)
        {
        case BLINK_OFF:
            ledEyes.flag_eyes_blink = false;
            vTaskDelay(50 / portTICK_PERIOD_MS);
            break;
        case BLINK_ON_CONSTANT:
            blink_pulse_idx = 0;
            ledEyes.flag_eyes_blink = true;
            vTaskDelay(ledEyes.eyes_blink_palse_ms[blink_pulse_idx] / portTICK_PERIOD_MS);
            break;
        case BLINK_ON_RANDOM:
            blink_pulse_idx = (blink_pulse_idx + 1) % 10;
            ledEyes.flag_eyes_blink = true;
            vTaskDelay(ledEyes.eyes_blink_palse_ms[blink_pulse_idx] / portTICK_PERIOD_MS);
            break;
        default:
            ledEyes.flag_eyes_blink = false;
            vTaskDelay(50 / portTICK_PERIOD_MS);
            break;
        }
    }
}

/// @brief 眨眼睛效果+最终LED颜色赋值和刷新
/// @param pvParameters LED眼睛对象的指针
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
            // vTaskDelay(ledEyes.eyes_blink_palse_ms / portTICK_PERIOD_MS);
            ledEyes.flag_eyes_blink = false; // 眨眼睛结束后，设置标志位为false
        }
        else
        {
            for (int led_index = 0; led_index < NUM_LEDS; led_index++)
            {
                ledEyes.leds_l[led_index] = ledEyes.leds_britrans_l[led_index];
                ledEyes.leds_r[led_index] = ledEyes.leds_britrans_r[led_index];
            }
            FastLED.show();
            vTaskDelay(ledEyes.eyes_blink_delay_ms / portTICK_PERIOD_MS);
        }
    }
}

