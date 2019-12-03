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

    void NewRecord();

private:
    std::map<Res *, UIObject *> mRes2Obj;
    std::map<UIObject *, Res *> mObj2Res;
    //  �ؼ�
    SharePtr<UIObjectLayout> mListBox;
    SharePtr<UIObjectLayout> mTypeBox;
    SharePtr<UIObjectLayout> mRefsBox;
    SharePtr<UIObjectTextBox> mSearchBox;
    //  ����
    std::string                     mPreSearch;
    std::function<void(Res::Ref *)> mOptSelect;
};
