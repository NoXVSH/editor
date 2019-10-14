#pragma once

#include "../../include.h"
#include "project.h"

class UIObject;
class GLObject;
class Component;

class EditorSys {
private:
    UniquePtr<Project>              _project;
    std::vector<SharePtr<UIObject>> _selected;

public:
//  ����ԭʼ�ӿ�
    /// ����
    //  �������
    void OptInsertObject(const SharePtr<UIObject> & uiObject, const SharePtr<UIObject> & parentUIObject);
    //  ѡ�ж���
    void OptSelectObject(const SharePtr<UIObject> & uiObject, bool select, bool multi = false);
    //  �޸�����
    void OptRenameObject(const SharePtr<UIObject> & uiObject, const std::string & name);
    //  ɾ������
    void OptDeleteObject(const SharePtr<UIObject> & uiObject);

    /// ���
    //  �������
    void OptAppendComponent(const SharePtr<UIObject> & uiObject, const SharePtr<Component> & component);
    //  ɾ�����
    void OptDeleteComponent(const SharePtr<UIObject> & uiObject, const SharePtr<Component> & component);

    /// ��Ŀ
    //  �½���Ŀ
    void OptNewProject(const std::string & url);
    //  ����Ŀ
    void OptOpenProject(const std::string & url);
    //  ������Ŀ
    void OptSaveProject(const std::string & url);
    //  �ر���Ŀ
    void OptFreeProject();

//  ���߽ӿ�
    void OpenDialogNewProject(const std::string & url);
    bool OpenDialogOpenProject(const std::string & url);
    //  ��Ŀ�Ƿ��
    bool IsOpenProject() const;
    //  ����/����������
    std::string ObjectName(const SharePtr<GLObject> & parent) const;
    std::string ObjectName(const SharePtr<UIObject> & parent) const;
    bool        ObjectName(const SharePtr<UIObject> & parent, const std::string & name) const;
    //  ��ȡ��ǰѡ�еĶ���
    const std::vector<SharePtr<UIObject>> & GetSelectedObjects() const;
    //  ��ǰ��Ŀ
    const UniquePtr<Project> & GetProject();
};