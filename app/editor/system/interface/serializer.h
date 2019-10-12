#pragma once

#include "../../include.h"

namespace interface {

//  �ṩ���л�����������
class Serializer {
public:
    //  �ַ���ֵ��������
    enum class StringValueTypeEnum {
        kERR,           //  ��Ч
        kINT,           //  ����
        kBOOL,          //  ����
        kFLOAT,         //  ����
        kSTRING,        //  �ַ���
        kVEC2,          //  ����2
        kVEC3,          //  ����3
        kVEC4,          //  ����4
        kINT_LIST,      //  �����б�
        kFLOAT_LIST,    //  �����б�
        kSTRING_LIST,   //  �ַ����б�
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