#include "ble_msg_hdl.h"

void helpMsgSetup()
{
    ble_helpmsg = "发送 rst 重置\n";
    // ble_helpmsg += "发送 bp + 整数 调整眨眼间隔时间\n";
    // ble_helpmsg += "发送 bd + 整数 调整眨眼延时时间\n";
    ble_helpmsg += "发送 bri + 0~100之间整数 调整眨眼亮度\n";
    ble_helpmsg += "发送 blk on/off 开启/关闭眨眼\n";
    ble_helpmsg += "发送 brig on/off 开启/关闭颜色渐变\n";
    ble_helpmsg += "发送 brth on/off 开启/关闭呼吸灯效果\n";
    ble_helpmsg += "发送 brths + 0~100之间整数 调整呼吸灯速度\n";
    ble_helpmsg += "发送 clr r g b 设置眼睛颜色\n";
    ble_helpmsg += "发送 clr + 切换下一个颜色\n";
    ble_helpmsg += "发送 clr - 切换上一个颜色\n";
    ble_helpmsg += "发送 clr rst 重置眼睛颜色\n";
    ble_helpmsg += "发送 clr show 显示眼睛当前颜色\n";
    ble_helpmsg += "发送 cs on/off/inv 控制颜色变化效果开关/反向\n";
    ble_helpmsg += "发送 csd + 整数 控制颜色切换时延(秒)\n";
    ble_helpmsg += "发送 csd rst 重置颜色切换时延\n";    
    // ble_helpmsg += "发送 ct on/off 开启/关闭颜色变化效果\n";
    // ble_helpmsg += "发送 cts + 整数 调整颜色变化速度\n";
    ble_helpmsg += "发送 fan on/off 控制风扇开关\n";
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

    // help 帮助
    if (incoming_string == "help")
    {
        SerialBT.println(ble_helpmsg);
    }

    // rst 重置
    if (incoming_string == "rst")
    {
        ledEyes.init();
        SerialBT.println("重置设置");
        SerialBT.println(ble_helpmsg);
    }

    // // 修改眨眼间隔时间
    // String prefix_blinkpulse = "bp ";
    // if (incoming_string.startsWith(prefix_blinkpulse))
    // {
    //     String numberPart = incoming_string.substring(prefix_blinkpulse.length());
    //     // 判断是否为数字
    //     if (std::all_of(numberPart.begin(), numberPart.end(), ::isdigit))
    //     {
    //         int value = numberPart.toInt();
    //         ledEyes.eyes_blink_palse_ms[0] = value * 10;
    //         SerialBT.println("修改眨眼间隔时间为：" + String(ledEyes.eyes_blink_palse_ms[0]) + "ms");
    //     }
    //     else
    //     {
    //         SerialBT.println("发送 bp + 正整数 以调整眨眼间隔时间  " + numberPart);
    //         return;
    //     }
    // }

    // // 修改眨眼延时时间
    // String prefix_blinkdelay = "bd ";
    // if (incoming_string.startsWith(prefix_blinkdelay))
    // {
    //     String numberPart = incoming_string.substring(prefix_blinkdelay.length());
    //     // 判断是否为数字
    //     if (std::all_of(numberPart.begin(), numberPart.end(), ::isdigit))
    //     {
    //         int value = numberPart.toInt();
    //         ledEyes.eyes_blink_delay_ms = value;
    //         SerialBT.println("修改眨眼延时时间为：" + String(ledEyes.eyes_blink_delay_ms) + "ms");
    //     }
    //     else
    //     {
    //         SerialBT.println("发送 bd + 正整数 以调整眨眼延时时间  " + numberPart);
    //         return;
    //     }
    // }

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

    // 修改是否眨眼标志  "blk on" "blk rand" "blk cst" "blk off"
    String prefix_blinkflag = "blk ";
    if (incoming_string.startsWith(prefix_blinkflag))
    {
        String flag = incoming_string.substring(prefix_blinkflag.length());
        if (flag == "on" || flag == "rand")
        {
            ledEyes.eyes_blink_mode = BLINK_ON_RANDOM;
            SerialBT.println("随机间隔眨眼");
        }
        else if (flag == "cst")
        {
            ledEyes.eyes_blink_mode = BLINK_ON_CONSTANT;
            SerialBT.println("等间隔眨眼");
        }
        else if (flag == "off")
        {
            ledEyes.eyes_blink_mode = BLINK_OFF;
            SerialBT.println("眨眼暂停");
        }
        else
        {
            SerialBT.println("发送 blk on 或 blk off 以开启或关闭眨眼标志位");
            return;
        }
    }

    // 修改是否开启亮度渐变标志  "brig on" "brig off"
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

    // 修改是否开启呼吸灯效果标志："brth on" "brth off"
    String prefix_breathflag = "brth ";
    if (incoming_string.startsWith(prefix_breathflag))
    {
        String flag = incoming_string.substring(prefix_breathflag.length());
        if (flag == "on")
        {
            ledEyes.flag_bri_breathe = true;
            SerialBT.println("呼吸灯效果开启");
        }
        else if (flag == "off")
        {
            ledEyes.flag_bri_breathe = false;
            SerialBT.println("呼吸灯效果关闭");
        }
        else
        {
            SerialBT.println("发送 brth on 或 brth off 以开启或关闭呼吸灯效果");
            return;
        }
    }

    // 修改呼吸灯速度 "brths + 整数"
    String prefix_breathspeed = "brths ";
    if (incoming_string.startsWith(prefix_breathspeed))
    {
        String numberPart = incoming_string.substring(prefix_breathspeed.length());
        // 判断是否为数字
        if (std::all_of(numberPart.begin(), numberPart.end(), ::isdigit))
        {
            int value = numberPart.toInt();
            if (value < 1 || value > 100)
            {
                SerialBT.println("发送 brths + 1~100之间整数 以调整呼吸灯速度  " + numberPart);
                return;
            }
            ledEyes.led_bri_timeshift_T_ms = 20 / float(value) * 1000;  // 周期0.2~20秒
            SerialBT.println("修改呼吸灯周期为：" + String(ledEyes.led_bri_timeshift_T_ms / 1000.0, 2) + "s");
        }
        else
        {
            SerialBT.println("发送 brths + 1~100之间整数 以调整呼吸灯速度  " + numberPart);
            return;
        }
    }

    // 修改眼睛颜色:
    // 命令“clr r g b” 以设置眼睛颜色
    // 命令“clr rst” 以重置眼睛颜色
    // 命令“clr show” 以显示眼睛颜色
    String prefix_clr = "clr ";
    if (incoming_string.startsWith(prefix_clr))
    {
        String colorPart = incoming_string.substring(prefix_clr.length());
        if (colorPart == "rst")
        {
            ledEyes.setLeds2SingleColor(ledEyes.leds_color_l, ledEyes.leds_color_r, ledEyes.led_CRGBcolor_init);
            ledEyes.color_seq_idx = 0;
            SerialBT.println("重置眼睛颜色");
        }
        else if (colorPart == "show")
        {
            // int Rl = ledEyes.leds_color_l[0].r;
            SerialBT.println("当前眼睛颜色为：" + String(ledEyes.leds_color_l[0].r) + " " + String(ledEyes.leds_color_l[0].g) + " " + String(ledEyes.leds_color_l[0].b));
        }
        else if (colorPart == "+")
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
            // int Rl = ledEyes.leds_color_l[0].r;
            SerialBT.println("当前眼睛颜色为：" + String(ledEyes.leds_color_l[0].r) + " " + String(ledEyes.leds_color_l[0].g) + " " + String(ledEyes.leds_color_l[0].b));
        }
        else if (colorPart == "-")
        {
            ledEyes.color_seq_idx -= ledEyes.color_seq_delta_idx;
            if (ledEyes.color_seq_idx >= ledEyes.color24_seq_len)
            {
                ledEyes.color_seq_idx -= ledEyes.color24_seq_len;
            }
            if (ledEyes.color_seq_idx < 0)
            {
                ledEyes.color_seq_idx += ledEyes.color24_seq_len;
            }
            ledEyes.setLeds2SingleColor(ledEyes.leds_color_l, ledEyes.leds_color_r, ledEyes.color24_seq[ledEyes.color_seq_idx]);
            // int Rl = ledEyes.leds_color_l[0].r;
            SerialBT.println("当前眼睛颜色为：" + String(ledEyes.leds_color_l[0].r) + " " + String(ledEyes.leds_color_l[0].g) + " " + String(ledEyes.leds_color_l[0].b));
        }
        else
        {
            int r, g, b;
            int space1 = colorPart.indexOf(" ");
            int space2 = colorPart.lastIndexOf(" ");
            r = colorPart.substring(0, space1).toInt();
            g = colorPart.substring(space1 + 1, space2).toInt();
            b = colorPart.substring(space2 + 1).toInt();
            ledEyes.setLeds2SingleColor(ledEyes.leds_color_l, ledEyes.leds_color_r, CRGB(r, g, b));
            SerialBT.println("设置眼睛颜色RGB为：" + String(r) + " " + String(g) + " " + String(b));
        }
    }

    // 修改颜色变化效果 "cs on" "cs off" "cs inv"
    String prefix_colorshiftflag = "cs ";
    if (incoming_string.startsWith(prefix_colorshiftflag))
    {
        String flag = incoming_string.substring(prefix_colorshiftflag.length());
        if (flag == "on")
        {
            ledEyes.color_shift_mode = COLOR_SHIFT_ON;
            SerialBT.println("颜色变化效果开启，正向切换");
        }
        else if (flag == "off")
        {
            ledEyes.color_shift_mode = COLOR_SHIFT_OFF;
            SerialBT.println("颜色变化效果关闭");
        }
        else if (flag == "inv")
        {
            ledEyes.color_shift_mode = COLOR_SHIFT_ON_INVERSE;
            SerialBT.println("颜色变化效果开启，反向切换");
        }
        else
        {
            SerialBT.println("发送 cs on 或 cs off 以开启或关闭颜色变化效果");
            return;
        }
    }
    
    // 修改颜色切换间隔时间 "csd + 整数" “csd rst”
    String prefix_csd = "csd ";
    if (incoming_string.startsWith(prefix_csd))
    {
        String numberPart = incoming_string.substring(prefix_csd.length());
        // 判断是否为数字
        if (std::all_of(numberPart.begin(), numberPart.end(), ::isdigit))
        {
            int value = numberPart.toInt();
            ledEyes.color_shift_delay_ms = value * 1000;
            SerialBT.println("修改颜色切换间隔时间为：" + String(ledEyes.color_shift_delay_ms / 1000) + "s");
        }
        else if (numberPart == "rst")
        {
            ledEyes.color_shift_delay_ms = ledEyes.COLOR_SHIFT_DELAY_MS_INIT;
            SerialBT.println("重置颜色切换间隔时间为：" + String(ledEyes.color_shift_delay_ms / 1000) + "s");
        }
        else
        {
            SerialBT.println("发送 csd + 正整数 以调整颜色切换间隔时间  " + numberPart);
            return;
        }
    }

    

    // 修改是否颜色变换标志  "ct on" "ct off"
    String prefix_ctflag = "ct ";
    if (incoming_string.startsWith(prefix_ctflag))
    {
        String flag = incoming_string.substring(prefix_ctflag.length());
        if (flag == "on")
        {
            ledEyes.flag_eyes_colortrans = true;
            SerialBT.println("颜色变换标志位开启");
        }
        else if (flag == "off")
        {
            ledEyes.flag_eyes_colortrans = false;
            SerialBT.println("颜色变换标志位关闭");
        }
        else
        {
            SerialBT.println("发送 ct on 或 ct off 以开启或关闭颜色变换标志位");
            return;
        }
    }

    // 修改颜色变换速度
    String prefix_ctspeed = "cts ";
    if (incoming_string.startsWith(prefix_ctspeed))
    {
        String numberPart = incoming_string.substring(prefix_ctspeed.length());
        // 判断是否为数字
        if (std::all_of(numberPart.begin(), numberPart.end(), ::isdigit))
        {
            int value = numberPart.toInt();
            ledEyes.colorTransSpeed = value;
            SerialBT.println("修改颜色变换速度为：" + String(ledEyes.colorTransSpeed));
        }
        else
        {
            SerialBT.println("发送 cts + 正整数 以调整颜色变化速度  " + numberPart);
            return;
        }
    }

    // 组合指令&模式控制
    // "mode dj" 切换蹦迪模式：关闭自动眨眼标志位，开启亮度呼吸灯模式（快速）、开启颜色切换（快速）、切换最大亮度
    // "mode pce" 切换舒缓模式：关闭自动眨眼标志位，开启亮度呼吸灯模式（慢速）、开启颜色切换（慢速）、切换最大亮度
    // "mode nom" 切换普通模式/默认模式：打开自动眨眼标志位，关闭亮度呼吸灯模式、开启颜色切换（慢速）
    String prefix_mode = "mode ";
    if (incoming_string.startsWith(prefix_mode))
    {
        String mode = incoming_string.substring(prefix_mode.length());
        if (mode == "dj")
        {
            ledEyes.flag_eyes_blink = false;
            ledEyes.flag_bri_breathe = true;
            ledEyes.led_bri_timeshift_T_ms = 0.2 * 1000; // 0.2秒
            ledEyes.color_shift_mode = COLOR_SHIFT_ON;
            ledEyes.color_shift_delay_ms = 2 * 1000; // 2秒
            ledEyes.led_brightness = 1.0; // 最大亮度
            SerialBT.println("切换蹦迪模式");
        }
        else if (mode == "pce")
        {
            ledEyes.flag_eyes_blink = false;
            ledEyes.flag_bri_breathe = true;
            ledEyes.led_bri_timeshift_T_ms = 4.0 * 1000; // 4秒
            ledEyes.color_shift_mode = COLOR_SHIFT_ON;
            ledEyes.color_shift_delay_ms = 30 * 1000; // 30秒
            ledEyes.led_brightness = 1.0; // 最大亮度
            SerialBT.println("切换舒缓模式");
        }
        else if (mode == "nom")
        {
            ledEyes.flag_eyes_blink = true;
            ledEyes.flag_bri_breathe = false;
            ledEyes.led_bri_timeshift_T_ms = 5.0 * 1000; // 5秒
            ledEyes.color_shift_mode = COLOR_SHIFT_ON;
            ledEyes.color_shift_delay_ms = 30 * 1000; // 30秒
            ledEyes.led_brightness = 0.5;
            SerialBT.println("切换普通模式");
        }
        else
        {
            SerialBT.println("发送 mode dj、mode pce 或 mode nom 以切换模式");
            return;
        }
    }

    // 控制风扇开关 "fan on" "fan off"
    String prefix_fan = "fan ";
    if (incoming_string.startsWith(prefix_fan))
    {
        String flag = incoming_string.substring(prefix_fan.length());
        if (flag == "on")
        {
            funEnable();
            SerialBT.println("风扇开启");
        }
        else if (flag == "off")
        {
            funDisable();
            SerialBT.println("风扇关闭");
        }
        else
        {
            SerialBT.println("发送 fan on 或 fan off 以开启或关闭风扇");
            return;
        }
    }
    

}


