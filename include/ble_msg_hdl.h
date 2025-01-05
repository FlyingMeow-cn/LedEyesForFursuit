#ifndef BLE_MSG_HDL_H
#define BLE_MSG_HDL_H

#include <Arduino.h>
#include "BluetoothSerial.h"
#include "ledeyes.h"

extern BluetoothSerial SerialBT;
extern LedEyes ledEyes;
extern String helpmsg;

void bleMsgHandler(); // 蓝牙消息处理函数
void helpMsgSetup();  // 帮助信息设置

#endif // BLE_MSG_HDL_H