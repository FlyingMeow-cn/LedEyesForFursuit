#include"device_ctrl.h"

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

