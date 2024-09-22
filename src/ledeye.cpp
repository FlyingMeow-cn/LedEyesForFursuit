#include "ledeye.h"

LedEye::LedEye(int num_leds, int led_pin) 
    : NUM_LEDS(num_leds), LED_PIN(led_pin) // 初始化常量成员并分配内存
{
    leds = new CRGB[NUM_LEDS]; // 分配内存
}

LedEye::~LedEye()
{
    delete[] leds; // 释放内存
}

void LedEye::init()
{
    int led_num = this->NUM_LEDS;
    int led_pin = this->LED_PIN;
    FastLED.addLeds<WS2812B, led_pin, GRB>(leds, led_num);
    
}

