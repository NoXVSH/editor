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
    void OptMetaSelectObject(UIObject * uiObject, bool select, bool multi = false);
    //  ɾ������
    void OptMetaDeleteObject(UIObject * uiObject);
    //  �������
    void OptMetaInsertObject(UIObject * uiObject, UIObject * insUIObject);
    //  �޸�����
    void OptMetaRenameObject(UIObject * uiObject, const std::string & name);

    /// ���
    //  �������
    void OptMetaAppendComponent(UIObject * uiObject, Component * component);
    //  ɾ�����
    void OptMetaDeleteComponent(UIObject * uiObject, Component * component);

    /// ��Ŀ
    //  �½���Ŀ
    void OptMetaNewProject(const std::string & url);
    //  ����Ŀ
    bool OptMetaOpenProject(const std::string & url);
    //  ������Ŀ
    bool OptMetaSaveProject(const std::string & url);
    //  �ر���Ŀ
    void OptMetaFreeProject();

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