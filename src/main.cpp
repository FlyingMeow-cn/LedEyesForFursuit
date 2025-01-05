#include <Arduino.h>
#include <FastLED.h>
#include "ledeyes.h"
#include "BluetoothSerial.h" //引入蓝牙函数库
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif // 判断蓝牙是否启用
#include "ble_msg_hdl.h"

BluetoothSerial SerialBT;
LedEyes ledEyes;
String helpmsg = "";

// void bleMsgHandler(); // 蓝牙消息处理函数
// void helpMsgSetup();  // 帮助信息设置

void setup()
{

    SerialBT.begin("ESP32_飞喵的发光眼设备"); // 启动蓝牙串口并设置蓝牙的名称
    Serial.begin(9600);

    helpMsgSetup();
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
