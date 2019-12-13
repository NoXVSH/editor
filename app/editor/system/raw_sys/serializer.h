#pragma once

#include "../../include.h"

class Project;

//  �ṩ���л�����������
class Serializer {
public:
    virtual void EncodeBinary(std::ostream & os, Project * project) = 0;
    virtual void DecodeBinary(std::istream & is, Project * project) = 0;
};
