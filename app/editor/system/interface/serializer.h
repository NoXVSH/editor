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
    virtual void EncodeBinary(std::ofstream & os) = 0;
    virtual void DecodeBinary(std::ifstream & is) = 0;
};
}