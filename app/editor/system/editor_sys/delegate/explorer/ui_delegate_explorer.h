#pragma once

#include "../ui_delegate_base.h"

//  ��Դ�б��ʽ:
//      Type|Path [> Word0 Word1]

//  �����ؼ��ָ�ʽ:
//      Type0 Type1|Word0 Word1

class UIDelegateExplorer : public UIDelegateBase {
public:
    using SelectFunc_t  = std::function<void (Res::Ref *)>;
    using InitParam_t   = std::tuple<std::string, SelectFunc_t>;

    //  ������Ϣ
    struct SearchStat {
        std::vector<std::string> mTypes;
        std::vector<std::string> mWords;
    };

    //  �������
    struct SearchItem {
        SearchItem(Res * res = nullptr, uint type = 0)
            : mRes(res), mType(type) { }
        Res * mRes;
        uint mType;
        std::vector<uint> mWords;
    };

private:
    virtual bool OnCallEventMessage(UIEventEnum e, const UIEvent::Event & param, const SharePtr<UIObject> & object) override;
    bool OnEventMouse(const UIEvent::Mouse & param);
    bool OnEventMenu(const UIEvent::Menu & param);
    bool OnEventInit(const UIEvent::Init & param);

    //  �������
    void ListRefresh();
    void ListClick1(const SharePtr<UIObject> & object);
    void ListClick2(const SharePtr<UIObject> & object);
    void ListRClick(const SharePtr<UIObject> & object);

    //  ��Դ����
    void ResSetType(const SharePtr<UIObject> & object, const Res::TypeEnum type);
    void ResRename(const SharePtr<UIObject> & object, const std::string & name);
    void ResDelete(const SharePtr<UIObject> & object);

    void NewRecord(Res * res);
    void NewSearch(const std::string & search);
    void NewSearch(const SearchStat  & search);

private:
    //  �ؼ�
    SharePtr<UIObjectLayout> mListLayout;
    SharePtr<UIObjectLayout> mTypeLayout;
    SharePtr<UIObjectLayout> mRefsLayout;
    SharePtr<UIObjectTextBox> mSearchText;
    //  ����
    std::string                     mPreSearch;
    std::vector<std::string>        mLimitType;
    std::function<void(Res::Ref *)> mOptSelect;
    //  �ڲ�״̬
    std::vector<SearchItem> mSearchItems;       //  �������
    SearchStat              mSearchStat;        //  ����״̬
    SharePtr<UIObject>      mLastSelect;        //  ���ѡ��
    Project *               mProject;           //  ��ǰ��Ŀ

    std::map<Res *, SharePtr<UIObject>> mRes2Obj;
    std::map<SharePtr<UIObject>, Res *> mObj2Res;
};
