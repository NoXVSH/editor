#pragma once

#include "../ui_delegate_base.h"

//  ��Դ�б��ʽ:
//      Type|Path [> Word0 Word1]

//  �����ؼ��ָ�ʽ:
//      Type0 Type1|Word0 Word1

class UIDelegateExplorer : public UIDelegateBase {
private:
    virtual bool OnCallEventMessage(UIEventEnum e, const UIEvent::Event & param, const SharePtr<UIObject> & object) override;

    //  �������
    void ListRefresh();
    void ListClick1();
    void ListClick2();
    void ListRClick();

    //  ��Դ����
    void RefReference();
    void ResSetType();
    void ResRename();
    void ResDelete();

    //  ��ʼ��id2obj, obj2id
    void Init();

    //  Serach
    //      Type0 Type1|Word0 Word1

    //  Menu
    //      Delete
    //      Rename
    //      SetType
    //      Reference

    //  Show
    //      Type|Path > Word0 Word1

private:
    std::map<uint, UIObject *> _id2obj;
    std::map<UIObject *, uint> _obj2id;
};
