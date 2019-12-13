#pragma once

#include "../include.h"

//  ������ǩ:
//      __Property
//      __Children

class UIParser {
public:
    //  �ַ���ֵ��������
    enum class StringValueTypeEnum {
        kErr,           //  ��Ч
        kInt,           //  ����
        kBool,          //  ����
        kFlag,          //  ��־
        kFloat,         //  ����
        kString,        //  �ַ���
        kVector2,       //  ����2
        kVector3,       //  ����3
        kVector4,       //  ����4
        kIntList,       //  �����б�
        kFloatList,     //  �����б�
        kStringList,    //  �ַ����б�
        kColor4,        //  ��ɫ
        kAsset,         //  ��Դ
    };

public:
    static SharePtr<UIObject> CreateObject(const int type);
    static SharePtr<UIObject> Parse(const std::string & url);
    static SharePtr<UIObject> Parse(const mmc::Json::Pointer json);
    static void Parse__Property(const mmc::Json::Pointer json, const SharePtr<UIObject> & object);
    static void Parse__Children(const mmc::Json::Pointer json, const SharePtr<UIObject> & object);
    static bool FromStringParse(StringValueTypeEnum type, const std::string & val, const std::string & sep, void * out);
};
