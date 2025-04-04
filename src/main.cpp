#include <Arduino.h>
#include <FastLED.h>
#include "ledeyes.h"
#include "BluetoothSerial.h" //引入蓝牙函数库
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif // 判断蓝牙是否启用
#include "ble_msg_hdl.h"
#include "voice_msg_hdl.h"
#include "device_ctrl.h"

BluetoothSerial SerialBT;
LedEyes ledEyes;
String ble_helpmsg = "";

bool led_B_tigger = false;

void setup()
{
    deviceSetup();

    SerialBT.begin("ESP32_飞喵的发光眼设备"); // 启动蓝牙串口并设置蓝牙的名称
    Serial.begin(9600);
    Serial2.begin(9600);

    helpMsgSetup();
    ledEyes.init();

    xTaskCreate(taskEyesBlinkTrigger, "taskEyesBlinkTrigger", 1024, &ledEyes, 4, NULL);
    xTaskCreate(taskLedsColorTrans, "ledsColorTrans", 1024, &ledEyes, 3, NULL);
    xTaskCreate(taskLedsColorShiftGradient, "taskLedsColorShiftGradient", 1024, &ledEyes, 3, NULL);
    xTaskCreate(taskLedsColorShift, "taskLedsColorShift", 1024, &ledEyes, 3, NULL);
    xTaskCreate(taskEyesBlink, "taskEyesBlink", 1024, &ledEyes, 2, NULL);
    xTaskCreate(taskEyesUpdate, "taskEyesUpdate", 1024, &ledEyes, 1, NULL);
    xTaskCreate(taskLedBlueBlink, "taskLedBlueBlink", 1024, NULL, 0, NULL);
}

void loop()
{
    bleMsgHandler();
    delay(100);
    voiceMsgHandler();
    delay(100);

    // String str2 = "";
    // while (Serial2.available())
    // {
    //     char c = Serial2.read();
    //     str2 += c;
    // }
    // if (str2 != "")
    // {
    //     // 打印到串口0
    //     // delay(10);
    //     Serial.print("Serial2 receive: " + str2);
    //     str2.clear();
    // }


}
