#pragma once

#include "../../include.h"

class Project;
class UIObject;
class GLObject;

class EditorSys {
public:
    Project * mProject;

    //  ��ǰѡ�ж���
    std::vector<UIObject *> mSelected;

public:
    EditorSys();

    bool OpenProjectDialog(const std::string & url);
    bool OpenProject(const std::string & url);
    bool SaveProject(const std::string & url);
    void FreeProject();
    bool IsOpenProject() const;
    //  ��������
    std::string ObjectName(GLObject * parent) const;
    //  �޸�����
    bool        ObjectName(GLObject * object, const std::string & name) const;
};