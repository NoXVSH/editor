#pragma once

#include "../base.h"

//  0. ���������ڵ�
//  1. ���������Ӽ�
//  ������ǩ:
//      __Property
//      __Children
//          MyGUI
//          Widget
//          Property
//          UserData

class UIParser {
public:
    static UIClass * Parse(const std::string & url);
    static UIClass * Parse(const mmc::JsonValue::Value json);
    static void Parse__Property(const mmc::JsonValue::Value json, UIClass * object);
    static void Parse__Children(const mmc::JsonValue::Value json, UIClass * object);
    static UIClass * CreateObject(const int type);
};
