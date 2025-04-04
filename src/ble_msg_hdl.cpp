#include "ble_msg_hdl.h"

void helpMsgSetup()
{
    ble_helpmsg = "发送 rst 重置\n";
    ble_helpmsg += "发送 bp + 整数 以调整眨眼间隔时间\n";
    ble_helpmsg += "发送 bd + 整数 以调整眨眼延时时间\n";
    ble_helpmsg += "发送 bri + 0~100之间整数 以调整眨眼亮度\n";
    ble_helpmsg += "发送 blk on/off 以开启/关闭眨眼\n";
    ble_helpmsg += "发送 brig on/off 以开启/关闭颜色渐变\n";
    ble_helpmsg += "发送 ct on/off 以开启/关闭颜色变化效果\n";
    ble_helpmsg += "发送 cts + 整数 以调整颜色变化速度\n";
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
            SerialBT.println("重置眼睛颜色");
        }
        else if (colorPart == "show")
        {
            // int Rl = ledEyes.leds_color_l[0].r;
            SerialBT.println("当前眼睛颜色为：" + String(ledEyes.led_CRGBcolor_init.r) + " " + String(ledEyes.led_CRGBcolor_init.g) + " " + String(ledEyes.led_CRGBcolor_init.b));
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


