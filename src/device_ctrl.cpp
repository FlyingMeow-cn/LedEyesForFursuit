#include "device_ctrl.h"

void deviceSetup()
{
    pinMode(FUN_CRIL_PIN, OUTPUT);
    pinMode(TEST_LED_G, OUTPUT);
    pinMode(TEST_LED_B, OUTPUT);

    digitalWrite(TEST_LED_G, LOW);
    digitalWrite(TEST_LED_B, LOW);

    // funEnable();
    funDisable();
}

void funEnable()
{
    digitalWrite(FUN_CRIL_PIN, HIGH);
}

void funDisable()
{
    digitalWrite(FUN_CRIL_PIN, LOW);
}

void taskLedBlueBlink(void *pvParameters)
{
    while (1)
    {
        if (led_B_tigger)
        {
            digitalWrite(TEST_LED_B, HIGH);
            vTaskDelay(250 / portTICK_PERIOD_MS);
            digitalWrite(TEST_LED_B, LOW);
            vTaskDelay(250 / portTICK_PERIOD_MS);
            digitalWrite(TEST_LED_B, HIGH);
            vTaskDelay(250 / portTICK_PERIOD_MS);
            digitalWrite(TEST_LED_B, LOW);
            vTaskDelay(250 / portTICK_PERIOD_MS);
            led_B_tigger = 0;
        }
        else
        {
            digitalWrite(TEST_LED_B, LOW);
            vTaskDelay(500 / portTICK_PERIOD_MS);
        }
    }
}
