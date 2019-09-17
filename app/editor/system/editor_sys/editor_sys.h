#pragma once

#include "../../include.h"
#include "component/gl_object.h"
#include "component/component.h"

class EditorSys {
    //  ������
public:
    //  ������
    GLObject * mRootObject;
    //  ��ǰѡ�ж���
    std::vector<GLObject *> mSelected;

    //  �ӿ���
public:
    EditorSys();
    //  ��������
    std::string GenerateObjectName(GLObject * object) const;
    //  �������
    bool CheckRename(GLObject * object, const std::string & name) const;
};