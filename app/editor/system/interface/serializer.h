#pragma once

#include "../../include.h"

namespace interface {

//  �ṩ���л�����������
class Serializer {
public:
    //  �ַ���ֵ��������
    enum class StringValueTypeEnum {
        kErr,           //  ��Ч
        kInt,           //  ����
        kBool,          //  ����
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

    static bool FromStringParse(
        StringValueTypeEnum type, 
        const std::string & val, 
        const std::string & sep, 
        void * out);

public:
    virtual void EncodeBinary(std::ofstream & os) = 0;
    virtual void DecodeBinary(std::ifstream & is) = 0;
};

#define FROM_STRING_PARSE_VARIABLE(type, key, val, sep, K, out) if (key == K) return interface::Serializer::FromStringParse(type, val, sep, &out)

}