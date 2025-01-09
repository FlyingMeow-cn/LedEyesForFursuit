#include "voice_msg_hdl.h"
#include "device_ctrl.h"

void voiceMsgHandler()
{
    String incoming_string = ""; // 创建一个空字符串来存储接收到的数据
    while (Serial.available())
    {                                             // 当还有数据可读时
        char incoming_char = (char)Serial.read(); // 读取字符
        incoming_string += incoming_char;          // 将字符添加到字符串
    }
    // while (Serial2.available())
    // {                                             // 当还有数据可读时
    //     char incoming_char = Serial2.read(); // 读取字符
    //     incoming_string += incoming_char;          // 将字符添加到字符串        
    // }

    // 如果incoming_string为空，说明没有接收到任何数据，直接返回
    if (incoming_string == "")
    {
        return;
    }
    else
    {
        Serial.println("incoming_string: " + incoming_string);
        incoming_string.trim();  // 去掉字符串前后的空格
        // Serial.println(incoming_string == "0000");
    }

    // 0000 语音唤醒
    if (incoming_string == "0000")
    {
        Serial.println("语音唤醒");
        SerialBT.println("语音唤醒");
        // return;
    }

    // 0011	打开风扇
    if (incoming_string == "0011")
    {
        funEnable();
        Serial.println("打开风扇");
        SerialBT.println("打开风扇");
        // return;
    }

    // 0012	关闭风扇
    if (incoming_string == "0012")
    {
        funDisable();
        Serial.println("关闭风扇");
        SerialBT.println("关闭风扇");
        // return;
    }

    // 0020	重置亮度
    if (incoming_string == "0020")
    {
        ledEyes.bri_seq_idx = ledEyes.bri_seq_idx_rst;
        ledEyes.led_brightness = ledEyes.bri_seq[ledEyes.bri_seq_idx];
        Serial.println("重置亮度");
        SerialBT.println("重置亮度");
        return;
    }

    // 0021	最小亮度
    if (incoming_string == "0021")
    {
        ledEyes.bri_seq_idx = 0;
        ledEyes.led_brightness = ledEyes.bri_seq[ledEyes.bri_seq_idx];
        Serial.println("最小亮度");
        SerialBT.println("最小亮度");
        return;
    }

    // 0022	关闭灯光
    if (incoming_string == "0022")
    {
        ledEyes.bri_seq_idx = 0;
        ledEyes.led_brightness = 0.0;
        Serial.println("关闭灯光");
        SerialBT.println("关闭灯光");
        return;
    }

    // 0023	最大亮度
    if (incoming_string == "0023")
    {
        ledEyes.bri_seq_idx = ledEyes.bri_seq_len - 1;
        ledEyes.led_brightness = ledEyes.bri_seq[ledEyes.bri_seq_idx];
        Serial.println("最大亮度");
        SerialBT.println("最大亮度");
        return;
    }

    // 0024	增大亮度
    if (incoming_string == "0024")
    {
        if (ledEyes.bri_seq_idx < ledEyes.bri_seq_len - 1)
        {
            ledEyes.bri_seq_idx++;
            ledEyes.led_brightness = ledEyes.bri_seq[ledEyes.bri_seq_idx];
            Serial.println("增大亮度");
            SerialBT.println("增大亮度");
        }
        else
        {
            Serial.println("已经是最大亮度");
            SerialBT.println("已经是最大亮度");
        }
        return;
    }

    // 0025	减小亮度
    if (incoming_string == "0025")
    {
        if (ledEyes.bri_seq_idx > 0)
        {
            ledEyes.bri_seq_idx--;
            ledEyes.led_brightness = ledEyes.bri_seq[ledEyes.bri_seq_idx];
            Serial.println("减小亮度");
            SerialBT.println("减小亮度");
        }
        else
        {
            Serial.println("已经是最小亮度");
            SerialBT.println("已经是最小亮度");
        }
        return;
    }

    // 0026	打开渐变
    if (incoming_string == "0026")
    {
        ledEyes.flag_eyes_bri_gradient = true;
        Serial.println("打开渐变");
        SerialBT.println("打开渐变");
        return;
    }

    // 0027	关闭渐变
    if (incoming_string == "0027")
    {
        ledEyes.flag_eyes_bri_gradient = false;
        Serial.println("关闭渐变");
        SerialBT.println("关闭渐变");
        return;
    }

    


}
