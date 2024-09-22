#ifndef LEDEYE_H
#define LEDEYE_H

#include <Arduino.h>
#include <FastLED.h>

class LedEye
{
private:
    const int NUM_LEDS;
    const int LED_PIN;
    CRGB* leds;
    
    

public:
    LedEye(int num_leds, int led_pin);       // 构造函数
    ~LedEye();      // 析构函数
    void init();
    

};

#endif // LEDEYE_H
