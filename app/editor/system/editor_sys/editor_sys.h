#pragma once

#include "../../include.h"
#include "project.h"

class GLObject;
class Component;

class EditorSys {
private:
    UniquePtr<Project>              _project;
    std::vector<SharePtr<GLObject>> _selected;

public:
    /// ����
    //  �������
    void OptInsertObject(const SharePtr<GLObject> & object, const SharePtr<GLObject> & parent);
    //  ѡ�ж���
    void OptSelectObject(const SharePtr<GLObject> & object, bool select, bool multi = false);
    //  �޸�����
    void OptRenameObject(const SharePtr<GLObject> & object, const std::string & name);
    //  ɾ������
    void OptDeleteObject(const SharePtr<GLObject> & object);

    /// ���
    //  �������
    void OptAppendComponent(const SharePtr<GLObject> & object, const SharePtr<Component> & component);
    //  ɾ�����
    void OptDeleteComponent(const SharePtr<GLObject> & object, const SharePtr<Component> & component);

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
    void OpenDialogOpenProject(const std::string & url);
    //  ��Ŀ�Ƿ��
    bool IsOpenProject() const;
    //  ����/����������
    std::string ObjectName(const SharePtr<GLObject> & parent) const;
    bool        ObjectName(const SharePtr<GLObject> & parent, const std::string & name) const;
    //  ��ȡ��ǰѡ�еĶ���
    const std::vector<SharePtr<GLObject>> & GetSelectedObjects() const;
    //  ��ǰ��Ŀ
    const UniquePtr<Project> & GetProject();
};