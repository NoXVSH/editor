#pragma once

#include "../base.h"

//  0. ���������ڵ�
//  1. ���������Ӽ�
//  ������ǩ:
//      __Property
//      __Children
//          MyGUI
//          Weiget
//          Property
//          UserData

class UIParser {
public:
    static UIClass * Parse(const std::string & url);
    static UIClass * Parse(const std::string & tag, const mmc::JsonValue::Value json);
    static void Parse__Property(const mmc::JsonValue::Value json, UIClass * object);
    static void Parse__Children(const mmc::JsonValue::Value json, UIClass * object);
    static void ParseCustomData(const mmc::JsonValue::Value json, UIState & state);
    static UIClass * CreateObject(UITypeEnum type);
};
