#ifndef VOICE_MSG_HDL_H
#define VOICE_MSG_HDL_H

#include <Arduino.h>
#include "BluetoothSerial.h"
#include "ledeyes.h"

extern BluetoothSerial SerialBT;
extern LedEyes ledEyes;

void voiceMsgHandler();

#endif // VOICE_MSG_HDL_H
