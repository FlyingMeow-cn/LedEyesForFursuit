#include "voice_msg_hdl.h"

void voiceMsgHandler()
{
    String incoming_string = ""; // 创建一个空字符串来存储接收到的数据
    // while (Serial.available())
    // {                                             // 当还有数据可读时
    //     char incoming_har = (char)Serial.read(); // 读取字符
    //     incoming_string += incoming_har;          // 将字符添加到字符串
    // }
    while (Serial2.available())
    {                                             // 当还有数据可读时
        char incoming_har = (char)Serial2.read(); // 读取字符
        incoming_string += incoming_har;          // 将字符添加到字符串
    }

    // 如果incoming_string为空，说明没有接收到任何数据，直接返回
    if (incoming_string == "")
    {
        return;
    }

    // 0000 语音唤醒
    if (incoming_string == "0000")
    {
        Serial.println("语音唤醒");
        SerialBT.println("语音唤醒");
        return;
    }
}
