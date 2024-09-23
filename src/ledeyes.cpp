#include "ledeyes.h"

LedEyes::LedEyes()
{
}

LedEyes::~LedEyes()
{
}

void LedEyes::init()
{

    FastLED.addLeds<WS2812B, LED_L_PIN, GRB>(leds_l, NUM_LEDS);
    FastLED.setBrightness(this->LED_BRIGNTNESS_INIT);
    FastLED.addLeds<WS2812B, LED_R_PIN, GRB>(leds_r, NUM_LEDS);
    FastLED.setBrightness(this->LED_BRIGNTNESS_INIT);
}
