#pragma once

#include "../../include.h"

class Project;
class UIObject;
class GLObject;
class Component;

class EditorSys {
private:
    Project *               _project;
    std::vector<UIObject *> _selected;

public:
    EditorSys();

//  ����ԭʼ�ӿ�
    /// ����
    //  ѡ�ж���
    void OptSelectObject(UIObject * uiObject, bool select, bool multi = false);
    //  ɾ������
    void OptDeleteObject(UIObject * uiObject);
    //  �������
    void OptInsertObject(UIObject * uiObject, UIObject * insUIObject);
    //  �޸�����
    void OptRenameObject(UIObject * uiObject, const std::string & name);

    /// ���
    //  �������
    void OptAppendComponent(UIObject * uiObject, Component * component);
    //  ɾ�����
    void OptDeleteComponent(UIObject * uiObject, Component * component);

    /// ��Ŀ
    //  �½���Ŀ
    void OptNewProject(const std::string & url);
    //  ����Ŀ
    bool OptOpenProject(const std::string & url);
    //  ������Ŀ
    bool OptSaveProject(const std::string & url);
    //  �ر���Ŀ
    void OptFreeProject();

//  ���߽ӿ�
    void OpenDialogNewProject(const std::string & url);
    bool OpenDialogOpenProject(const std::string & url);
    //  ��Ŀ�Ƿ��
    bool IsOpenProject() const;
    //  ���ɿ�������
    std::string ObjectName(GLObject * parent) const;
    std::string ObjectName(UIObject * parent) const;
    //  ����������
    bool        ObjectName(UIObject * parent, const std::string & name) const;
    //  ��ȡ��ǰѡ�еĶ���
    const std::vector<UIObject *> & GetSelectedObjects() const;
    //  ��ǰ��Ŀ
    Project * GetProject();
};