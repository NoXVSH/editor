#pragma once

#include "../../include.h"
#include "project.h"

class Res;
class GLObject;
class Component;

class EditorSys {
private:
    UniquePtr<Project>              _project;
    std::vector<SharePtr<GLObject>> _selected;

public:
    /// ����
    //  �������
    void OptInsertObject(SharePtr<GLObject> object, SharePtr<GLObject> parent);
    //  ѡ�ж���
    void OptSelectObject(SharePtr<GLObject> object, bool select, bool multi = false);
    void OptSelectObject(uint objectID, bool select, bool multi = false);
    //  �޸�����
    void OptRenameObject(SharePtr<GLObject> object, const std::string & name);
    void OptRenameObject(uint objectID, const std::string & name);
    //  ɾ������
    void OptDeleteObject(SharePtr<GLObject> object);
    void OptDeleteObject(uint objectID);
    //  �޸�״̬
    void OptStateAddObject(SharePtr<GLObject> object, uint state);
    void OptStateAddObject(uint objectID, uint state);
    void OptStateSubObject(SharePtr<GLObject> object, uint state);
    void OptStateSubObject(uint objectID, uint state);
    void OptStateObject(SharePtr<GLObject> object, uint state);
    void OptStateObject(uint objectID, uint state);
    //  �ƶ�����
    void OptMoveObject(SharePtr<GLObject> object, SharePtr<GLObject> target, int pos);  //  0, 1, 2 => ����, ǰ��, ����
    void OptMoveObject(uint objectID, SharePtr<GLObject> target, int pos);              //  0, 1, 2 => ����, ǰ��, ����
    void OptMoveObject(SharePtr<GLObject> object, uint targetID, int pos);              //  0, 1, 2 => ����, ǰ��, ����
    void OptMoveObject(uint objectID, uint targetID, int pos);                          //  0, 1, 2 => ����, ǰ��, ����

    /// ���
    //  �������
    void OptAppendComponent(const SharePtr<GLObject> & object, const SharePtr<Component> & component);
    void OptAppendComponent(uint id, const SharePtr<Component> & component);
    //  ɾ�����
    void OptDeleteComponent(const SharePtr<GLObject> & object, const SharePtr<Component> & component);
    void OptDeleteComponent(uint id, const SharePtr<Component> & component);

    /// ��Ŀ
    //  �½���Ŀ
    void OptNewProject(const std::string & url);
    //  ����Ŀ
    void OptOpenProject(const std::string & url);
    //  ������Ŀ
    void OptSaveProject(const std::string & url);
    //  �ر���Ŀ
    void OptFreeProject();

    /// ��Դ
    //  ɾ����Դ
    void OptDeleteRes(uint id);
    void OptDeleteRes(Res * res);
    //  �޸���Դ
    void OptModifyRes(uint id, const std::string & url);
    void OptModifyRes(Res * res, const std::string & url);
    //  ������Դ����
    void OptSetResType(uint id, uint type);
    void OptSetResType(Res * res, uint type);

//  ���߽ӿ�
    void OpenDialogNewProject(const std::string & url);
    void OpenDialogOpenProject(const std::string & url);
    //  ��Ŀ�Ƿ��
    bool IsOpenProject();
    //  ��ǰ��Ŀ
    const UniquePtr<Project> & GetProject();
    //  ����/����������
    std::string ObjectName(const SharePtr<GLObject> & parent);
    std::string ObjectName(uint id);
    bool        ObjectName(const SharePtr<GLObject> & parent, const std::string & name);
    bool        ObjectName(uint id, const std::string & name);
    //  ��ȡ��ǰѡ�еĶ���
    const std::vector<SharePtr<GLObject>> & GetSelectedObjects();
};