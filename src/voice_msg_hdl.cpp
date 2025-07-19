#include "voice_msg_hdl.h"
#include "device_ctrl.h"

void voiceMsgHandler()
{
    String incoming_string = ""; // 创建一个空字符串来存储接收到的数据
#ifdef VOICE_CTRL_DEBUG
    // 用于控制逻辑测试，使用电脑串口代替语音串口发送命令
    while (Serial.available())
    {                                             // 当还有数据可读时
        char incoming_char = (char)Serial.read(); // 读取字符
        incoming_string += incoming_char;         // 将字符添加到字符串
    }
#else
    // 使用语音控制
    while (Serial2.available())
    {                                        // 当还有数据可读时
        char incoming_char = Serial2.read(); // 读取字符
        incoming_string += incoming_char;    // 将字符添加到字符串
    }
#endif

    // 如果incoming_string为空，说明没有接收到任何数据，直接返回
    if (incoming_string == "")
    {
        return;
    }
    else
    {
        Serial.println("incoming_string: " + incoming_string);
        incoming_string.trim(); // 去掉字符串前后的空格
        // Serial.println(incoming_string == "0000");
    }

    // 0000 语音唤醒
    if (incoming_string == "0000")
    {
        led_B_tigger = 1;   // 蓝色LED灯闪烁两次
        ledEyes.flag_eyes_blink = true;  // 眨眼睛1次
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

    // 0028 开启眨眼 打开眨眼 （启用眨眼标志自动置位）
    if (incoming_string == "0028")
    {
        ledEyes.eyes_blink_mode = BLINK_ON_RANDOM;
        Serial.println("开启眨眼");
        SerialBT.println("开启眨眼");
        return;
    }

    // 0029 关闭眨眼
    if (incoming_string == "0029")
    {
        ledEyes.eyes_blink_mode = BLINK_OFF;
        Serial.println("关闭眨眼");
        SerialBT.println("关闭眨眼");
        return;
    }

    // 0030 亮度一
    if (incoming_string == "0030")
    {
        ledEyes.bri_seq_idx = 2;
        ledEyes.led_brightness = ledEyes.bri_seq[ledEyes.bri_seq_idx];
        Serial.println("亮度一");
        SerialBT.println("亮度一");
        return;
    }

    // 0031 亮度二
    if (incoming_string == "0031")
    {
        ledEyes.bri_seq_idx = 3;
        ledEyes.led_brightness = ledEyes.bri_seq[ledEyes.bri_seq_idx];
        Serial.println("亮度二");
        SerialBT.println("亮度二");
        return;
    }

    // 0032 亮度三
    if (incoming_string == "0032")
    {
        ledEyes.bri_seq_idx = 5;
        ledEyes.led_brightness = ledEyes.bri_seq[ledEyes.bri_seq_idx];
        Serial.println("亮度三");
        SerialBT.println("亮度三");
        return;
    }

    // 0033 呼吸灯速度一
    if (incoming_string == "0033")
    {
        ledEyes.led_bri_timeshift_T_ms = 5.0 * 1000; // 5秒
        Serial.println("呼吸灯速度一");
        SerialBT.println("呼吸灯速度一");
        return;
    }

    // 0034 呼吸灯速度二
    if (incoming_string == "0034")
    {
        ledEyes.led_bri_timeshift_T_ms = 2.0 * 1000; // 2秒
        Serial.println("呼吸灯速度二");
        SerialBT.println("呼吸灯速度二");
        return;
    }

    // 0035 呼吸灯速度三
    if (incoming_string == "0035")
    {
        ledEyes.led_bri_timeshift_T_ms = 1.0 * 1000; // 1秒
        Serial.println("呼吸灯速度三");
        SerialBT.println("呼吸灯速度三");
        return;
    }

    // 0036 呼吸灯速度四
    if (incoming_string == "0036")
    {
        ledEyes.led_bri_timeshift_T_ms = 0.5 * 1000; // 0.5秒
        Serial.println("呼吸灯速度四");
        SerialBT.println("呼吸灯速度四");
        return;
    }

    // 0037 呼吸灯速度五
    if (incoming_string == "0037")
    {
        ledEyes.led_bri_timeshift_T_ms = 0.2 * 1000; // 0.2秒
        Serial.println("呼吸灯速度五");
        SerialBT.println("呼吸灯速度五");
        return;
    }

    // 0038 打开呼吸灯
    if (incoming_string == "0038")
    {
        ledEyes.flag_bri_breathe = true;
        Serial.println("打开呼吸灯");
        SerialBT.println("打开呼吸灯");
        return;
    }

    // 0039 关闭呼吸灯
    if (incoming_string == "0039")
    {
        ledEyes.flag_bri_breathe = false;
        Serial.println("关闭呼吸灯");
        SerialBT.println("关闭呼吸灯");
        return;
    }

    // 0040 默认模式 普通模式 重置模式
    if (incoming_string == "0040")
    {
        ledEyes.flag_eyes_blink = true;
        ledEyes.flag_bri_breathe = false;
        ledEyes.led_bri_timeshift_T_ms = 5.0 * 1000; // 5秒
        ledEyes.color_shift_mode = COLOR_SHIFT_ON;
        ledEyes.color_shift_delay_ms = 30 * 1000; // 30秒
        ledEyes.led_brightness = 0.5;
        Serial.println("默认模式");
        SerialBT.println("默认模式");
        return;
    }

    // 0041 蹦迪模式 DJ模式
    if (incoming_string == "0041")
    {
        ledEyes.flag_eyes_blink = false;
        ledEyes.flag_bri_breathe = true;
        ledEyes.led_bri_timeshift_T_ms = 0.2 * 1000; // 0.2秒
        ledEyes.color_shift_mode = COLOR_SHIFT_ON;
        ledEyes.color_shift_delay_ms = 2 * 1000; // 2秒
        ledEyes.led_brightness = 1.0; // 最大亮度
        Serial.println("蹦迪模式");
        SerialBT.println("蹦迪模式");
        return;
    }

    // 0042 舒缓模式 平缓模式
    if (incoming_string == "0042")
    {
        ledEyes.flag_eyes_blink = false;
        ledEyes.flag_bri_breathe = true;
        ledEyes.led_bri_timeshift_T_ms = 4.0 * 1000; // 4秒
        ledEyes.color_shift_mode = COLOR_SHIFT_ON;
        ledEyes.color_shift_delay_ms = 30 * 1000; // 30秒
        ledEyes.led_brightness = 1.0; // 最大亮度
        Serial.println("舒缓模式");
        SerialBT.println("舒缓模式");
        return;
    }

    // 0100	重置颜色
    if (incoming_string == "0100")
    {
        ledEyes.color_seq_idx = ledEyes.color24_seq_idx_rst;
        ledEyes.setLeds2SingleColor(ledEyes.leds_color_l, ledEyes.leds_color_r, ledEyes.color24_seq[ledEyes.color_seq_idx]);
        Serial.println("重置颜色");
        SerialBT.println("重置颜色");
        return;
    }

    // 0101 打开颜色切换
    if (incoming_string == "0101")
    {
        ledEyes.color_shift_mode = COLOR_SHIFT_ON;
        Serial.println("颜色变化效果开启，正向切换");
        SerialBT.println("颜色变化效果开启，正向切换");
        return;
    }

    // 0102 关闭颜色切换
    if (incoming_string == "0102")
    {
        ledEyes.color_shift_mode = COLOR_SHIFT_OFF;
        Serial.println("颜色变化效果关闭");
        SerialBT.println("颜色变化效果关闭");
        return;
    }

    // 0103	切换颜色
    if (incoming_string == "0103")
    {
        ledEyes.color_seq_idx += ledEyes.color_seq_delta_idx;
        if (ledEyes.color_seq_idx >= ledEyes.color24_seq_len)
        {
            ledEyes.color_seq_idx -= ledEyes.color24_seq_len;
        }
        if (ledEyes.color_seq_idx < 0)
        {
            ledEyes.color_seq_idx += ledEyes.color24_seq_len;
        }
        ledEyes.setLeds2SingleColor(ledEyes.leds_color_l, ledEyes.leds_color_r, ledEyes.color24_seq[ledEyes.color_seq_idx]);
        Serial.println("切换颜色");
        SerialBT.println("切换颜色");
        return;
    }

    // 0104 颜色切换速度一
    if (incoming_string == "0104")
    {
        ledEyes.color_shift_delay_ms = 60 * 1000; // 60秒
        Serial.println("颜色切换速度一");
        SerialBT.println("颜色切换速度一");
        return;
    }

    // 0105 颜色切换速度二
    if (incoming_string == "0105")
    {
        ledEyes.color_shift_delay_ms = 30 * 1000; // 30秒
        Serial.println("颜色切换速度二");
        SerialBT.println("颜色切换速度二");
        return;
    }

    // 0106 颜色切换速度三
    if (incoming_string == "0106")
    {
        ledEyes.color_shift_delay_ms = 10 * 1000; // 10秒
        Serial.println("颜色切换速度三");
        SerialBT.println("颜色切换速度三");
        return;
    }

    // 0107 颜色切换速度四
    if (incoming_string == "0107")
    {
        ledEyes.color_shift_delay_ms = 5 * 1000; // 5秒
        Serial.println("颜色切换速度四");
        SerialBT.println("颜色切换速度四");
        return;
    }

    // 0108 颜色切换速度五
    if (incoming_string == "0108")
    {
        ledEyes.color_shift_delay_ms = 2 * 1000; // 2秒
        Serial.println("颜色切换速度五");
        SerialBT.println("颜色切换速度五");
        return;
    }

    // 0111 切换蓝色
    if (incoming_string == "0111")
    {
        ledEyes.color_seq_idx = 1; // 蓝色
        Serial.println("切换颜色为蓝色");
        SerialBT.println("切换颜色为蓝色");
        ledEyes.setLeds2SingleColor(ledEyes.leds_color_l, ledEyes.leds_color_r, ledEyes.color24_seq[ledEyes.color_seq_idx]);
        return;
    }

    // 0112 切换蓝紫色
    if (incoming_string == "0112")
    {
        ledEyes.color_seq_idx = 2; // 蓝紫色
        Serial.println("切换颜色为蓝紫色");
        SerialBT.println("切换颜色为蓝紫色");
        ledEyes.setLeds2SingleColor(ledEyes.leds_color_l, ledEyes.leds_color_r, ledEyes.color24_seq[ledEyes.color_seq_idx]);
        return;
    }

    // 0113 切换紫色
    if (incoming_string == "0113")
    {
        ledEyes.color_seq_idx = 3; // 紫色
        Serial.println("切换颜色为紫色");
        SerialBT.println("切换颜色为紫色");
        ledEyes.setLeds2SingleColor(ledEyes.leds_color_l, ledEyes.leds_color_r, ledEyes.color24_seq[ledEyes.color_seq_idx]);
        return;
    }

    // 0114 切换粉色
    if (incoming_string == "0114")
    {
        ledEyes.color_seq_idx = 4; // 粉色
        Serial.println("切换颜色为粉色");
        SerialBT.println("切换颜色为粉色");
        ledEyes.setLeds2SingleColor(ledEyes.leds_color_l, ledEyes.leds_color_r, ledEyes.color24_seq[ledEyes.color_seq_idx]);
        return;
    }

    // 0115 切换红色
    if (incoming_string == "0115")
    {
        ledEyes.color_seq_idx = 5; // 红色
        Serial.println("切换颜色为红色");
        SerialBT.println("切换颜色为红色");
        ledEyes.setLeds2SingleColor(ledEyes.leds_color_l, ledEyes.leds_color_r, ledEyes.color24_seq[ledEyes.color_seq_idx]);
        return;
    }

    // 0116	切换橙色
    if (incoming_string == "0116")
    {
        ledEyes.color_seq_idx = 6; // 橙色
        Serial.println("切换颜色为橙色");
        SerialBT.println("切换颜色为橙色");
        ledEyes.setLeds2SingleColor(ledEyes.leds_color_l, ledEyes.leds_color_r, ledEyes.color24_seq[ledEyes.color_seq_idx]);
        return;
    }

    // 0117	切换橙黄色
    if (incoming_string == "0117")
    {
        ledEyes.color_seq_idx = 7; // 橙黄色
        Serial.println("切换颜色为橙黄色");
        SerialBT.println("切换颜色为橙黄色");
        ledEyes.setLeds2SingleColor(ledEyes.leds_color_l, ledEyes.leds_color_r, ledEyes.color24_seq[ledEyes.color_seq_idx]);
        return;
    }

    // 0118	切换白色
    if (incoming_string == "0118")
    {
        ledEyes.color_seq_idx = 8; // 白色
        Serial.println("切换颜色为白色");
        SerialBT.println("切换颜色为白色");
        ledEyes.setLeds2SingleColor(ledEyes.leds_color_l, ledEyes.leds_color_r, ledEyes.color24_seq[ledEyes.color_seq_idx]);
        return;
    }

    // 0119	切换绿色
    if (incoming_string == "0119")
    {
        ledEyes.color_seq_idx = 10; // 绿色
        Serial.println("切换颜色为绿色");
        SerialBT.println("切换颜色为绿色");
        ledEyes.setLeds2SingleColor(ledEyes.leds_color_l, ledEyes.leds_color_r, ledEyes.color24_seq[ledEyes.color_seq_idx]);
        return;
    }

}
