#include <Arduino.h>
#include <FastLED.h>
#include "BluetoothSerial.h" //引入蓝牙函数库
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif // 判断蓝牙是否启用

#define NUM_LEDS 20
#define LED_L_PIN 16
#define LED_R_PIN 17

CRGB leds_l[NUM_LEDS];
CRGB leds_r[NUM_LEDS];

CRGB color24_1 = CRGB(50, 149, 183); // 湖蓝色

BluetoothSerial SerialBT;

void setLed2SingleColor(CRGB leds[NUM_LEDS], CRGB color); // 设置LED为单一颜色
void taskEyesBlink(void *pvParameters);                   // 眨眼睛任务 传入参数为CRGB类型的color
void taskBleMsgHdl(void *pvParameters);                   // 蓝牙消息处理任务
void bleMessageHandler();                                 // 蓝牙消息处理函数
void setLedBrightness();                                  // 设置LED亮度

// 一些全局变量
float led_brightness = 0.2;             // 当前亮度0.0~1.0
CRGB led_CRGBcolor_current = color24_1; // 当前颜色的RGB值
CRGB led_CRGB_current = color24_1;      // 对应亮度的当前颜色
bool flag_eyes_blink = true;            // 眨眼睛标志位
int eyes_blink_palse_ms = 5000;          // 眨眼睛间隔时间
int eyes_blink_delay_ms = 10;          // 眨眼睛延时时间

void setup()
{
    int LED_BRIGNTNESS_INIT = 128; // 0~255
    FastLED.addLeds<WS2812B, LED_L_PIN, GRB>(leds_l, NUM_LEDS);
    FastLED.setBrightness(LED_BRIGNTNESS_INIT);
    FastLED.addLeds<WS2812B, LED_R_PIN, GRB>(leds_r, NUM_LEDS);
    FastLED.setBrightness(LED_BRIGNTNESS_INIT);

    SerialBT.begin("ESP32_飞喵的发光眼设备"); // 启动蓝牙串口并设置蓝牙的名称
    Serial.begin(115200);

    xTaskCreate(taskEyesBlink, "taskEyesBlink", 1024, NULL, 1, NULL);
    // xTaskCreate(taskBleMsgHdl, "taskBleMsgHdl", 1024, NULL, 1, NULL);
}

void loop()
{
    bleMessageHandler();

    setLedBrightness();
    setLed2SingleColor(leds_l, led_CRGB_current);
    setLed2SingleColor(leds_r, led_CRGB_current);
    FastLED.show();

    delay(100);
}

void setLed2SingleColor(CRGB leds[NUM_LEDS], CRGB color)
{
    for (int i = 0; i < NUM_LEDS; i++)
    {
        leds[i] = color;
    }
}

void taskEyesBlink(void *pvParameters)
{
    while (1)
    {
        CRGB color = led_CRGB_current;
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
            FastLED.show();
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
            FastLED.show();
            vTaskDelay(eyes_blink_delay_ms / portTICK_PERIOD_MS);
        }
        // Serial.println("眨眼睛任务");
        vTaskDelay(eyes_blink_palse_ms / portTICK_PERIOD_MS);
    }
}

void bleMessageHandler()
{
    String incoming_string = ""; // 创建一个空字符串来存储接收到的数据
    while (SerialBT.available())
    {                                              // 当还有数据可读时
        char incoming_har = (char)SerialBT.read(); // 读取字符
        incoming_string += incoming_har;           // 将字符添加到字符串
    }

    // 如果incoming_string为空，说明没有接收到任何数据，直接返回
    if (incoming_string == "")
    {
        return;
    }

    // rst 重置
    if (incoming_string == "rst")
    {
        eyes_blink_palse_ms = 5000;
        eyes_blink_delay_ms = 10;
        led_brightness = 0.2;

        String rstmsg = "发送 bp + 整数 以调整眨眼间隔时间\n";
        rstmsg += "发送 bd + 整数 以调整眨眼延时时间\n";
        rstmsg += "发送 bri + 0~100之间整数 以调整眨眼亮度";

        SerialBT.println(rstmsg);

    }

    // 修改眨眼间隔时间
    String prefix_blinkpulse = "bp ";
    if (incoming_string.startsWith(prefix_blinkpulse))
    {
        String numberPart = incoming_string.substring(prefix_blinkpulse.length());
        // 判断是否为数字
        if (std::all_of(numberPart.begin(), numberPart.end(), ::isdigit))
        {
            int value = numberPart.toInt();
            eyes_blink_palse_ms = value * 10;
            SerialBT.println("修改眨眼间隔时间为：" + String(eyes_blink_palse_ms) + "ms");
        }
        else
        {
            SerialBT.println("发送 bp + 整数 以调整眨眼间隔时间  " + numberPart);
            return;
        }
    }

    // 修改眨眼延时时间
    String prefix_blinkdelay = "bd ";
    if (incoming_string.startsWith(prefix_blinkdelay))
    {
        String numberPart = incoming_string.substring(prefix_blinkdelay.length());
        // 判断是否为数字
        if (std::all_of(numberPart.begin(), numberPart.end(), ::isdigit))
        {
            int value = numberPart.toInt();
            eyes_blink_delay_ms = value;
            SerialBT.println("修改眨眼延时时间为：" + String(eyes_blink_delay_ms) + "ms");
        }
        else
        {
            SerialBT.println("发送 bd + 整数 以调整眨眼延时时间  " + numberPart);
            return;
        }
    }

    // 修改亮度
    String prefix_brightness = "bri ";
    if (incoming_string.startsWith(prefix_brightness))
    {
        String numberPart = incoming_string.substring(prefix_brightness.length());
        // 判断是否为数字
        if (std::all_of(numberPart.begin(), numberPart.end(), ::isdigit))
        {
            int value = numberPart.toInt();
            led_brightness = value / 100.0;
            if (led_brightness > 1.0)
            {
                led_brightness = 1.0;
            }
            else if (led_brightness < 0.0)
            {
                led_brightness = 0.0;
            }
            SerialBT.println("修改亮度为：" + String(led_brightness));
        }
        else
        {
            SerialBT.println("发送 bri + 0~100之间整数 以调整眨眼亮度  " + numberPart);
            return;
        }
    }
}

void setLedBrightness()
{
    CRGB color = led_CRGBcolor_current;
    uint8_t red = color.r * led_brightness;   // 获取红色分量
    uint8_t green = color.g * led_brightness; // 获取绿色分量
    uint8_t blue = color.b * led_brightness;  // 获取蓝色分量
    led_CRGB_current = CRGB(red, green, blue);
}