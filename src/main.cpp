#include <Arduino.h>
#include <FastLED.h>
#include "ledeyes.h"
#include "BluetoothSerial.h" //引入蓝牙函数库
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif // 判断蓝牙是否启用

BluetoothSerial SerialBT;
void bleMsgHandler(); // 蓝牙消息处理函数

LedEyes ledEyes;

void setup()
{

    SerialBT.begin("ESP32_飞喵的发光眼设备"); // 启动蓝牙串口并设置蓝牙的名称
    Serial.begin(115200);

    ledEyes.init();

    xTaskCreate(taskLedsColorTrans, "ledsColorTrans", 1024, &ledEyes, 3, NULL);
    xTaskCreate(taskEyesBlink, "taskEyesBlink", 1024, &ledEyes, 2, NULL);
    xTaskCreate(taskEyesUpdate, "taskEyesUpdate", 1024, &ledEyes, 1, NULL);

}

void loop()
{
    bleMsgHandler();
    delay(100);
}


void bleMsgHandler()
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
        ledEyes.init();

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
            ledEyes.eyes_blink_palse_ms = value * 10;
            SerialBT.println("修改眨眼间隔时间为：" + String(ledEyes.eyes_blink_palse_ms) + "ms");
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
            ledEyes.eyes_blink_delay_ms = value;
            SerialBT.println("修改眨眼延时时间为：" + String(ledEyes.eyes_blink_delay_ms) + "ms");
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
            ledEyes.led_brightness = value / 100.0;
            if (ledEyes.led_brightness > 1.0)
            {
                ledEyes.led_brightness = 1.0;
            }
            else if (ledEyes.led_brightness < 0.0)
            {
                ledEyes.led_brightness = 0.0;
            }
            SerialBT.println("修改亮度为：" + String(ledEyes.led_brightness));
        }
        else
        {
            SerialBT.println("发送 bri + 0~100之间整数 以调整眨眼亮度  " + numberPart);
            return;
        }
    }

    // 修改是否眨眼标志  "blk on" "blk off"
    String prefix_blinkflag = "blk ";
    if (incoming_string.startsWith(prefix_blinkflag))
    {
        String flag = incoming_string.substring(prefix_blinkflag.length());
        if (flag == "on")
        {
            ledEyes.flag_eyes_blink = true;
            SerialBT.println("眨眼标志位开启");
        }
        else if (flag == "off")
        {
            ledEyes.flag_eyes_blink = false;
            SerialBT.println("眨眼标志位关闭");
        }
        else
        {
            SerialBT.println("发送 blk on 或 blk off 以开启或关闭眨眼标志位");
            return;
        }
    }

    // 修改是否颜色渐变标志  "brig on" "brig off"
    String prefix_brigflag = "brig ";
    if (incoming_string.startsWith(prefix_brigflag))
    {
        String flag = incoming_string.substring(prefix_brigflag.length());
        if (flag == "on")
        {
            ledEyes.flag_eyes_bri_gradient = true;
            SerialBT.println("颜色渐变标志位开启");
        }
        else if (flag == "off")
        {
            ledEyes.flag_eyes_bri_gradient = false;
            SerialBT.println("颜色渐变标志位关闭");
        }
        else
        {
            SerialBT.println("发送 brig on 或 brig off 以开启或关闭颜色渐变标志位");
            return;
        }
    }

    // 修改眼睛颜色:
    // 
    
}

