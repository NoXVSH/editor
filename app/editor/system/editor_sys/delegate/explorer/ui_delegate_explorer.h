#pragma once

#include "../ui_delegate_base.h"

//  ��Դ�б��ʽ:
//      Type|Path [> Word0 Word1]

//  �����ؼ��ָ�ʽ:
//      Type0 Type1|Word0 Word1

class UIDelegateExplorer : public UIDelegateBase {
public:
    static const glm::vec4 sTypeColors[Res::Length];
    using SelectFn_t = std::function<void (Res::Ref)>;
    using InitArgs_t = std::tuple<std::string, SelectFn_t>;

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
    virtual bool OnCallEventMessage(UIEventEnum e, const UIEvent::Event & param) override;
    bool OnEventMouse(const UIEvent::Mouse & param);
    bool OnEventEdit(const UIEvent::Edit & param);
    bool OnEventMenu(const UIEvent::Menu & param);
    bool OnEventInit(const UIEvent::Init & param);

    void OnEvent(EventSys::TypeEnum type, const std::any & param);

    //  �������
    void ListRefresh();
    void ListClick1(const SharePtr<UIObject> & object);
    void ListClick2(const SharePtr<UIObject> & object);
    void ListRClick(const SharePtr<UIObject> & object);
    void ResSetType(const SharePtr<UIObject> & object);

    void NewRecord(const SearchItem & item);
    void NewSearch(const SearchStat & search);
    void NewSearch(const std::string & search);

    //  ������Ŀ�ַ���/��ɫ
    std::tuple<std::string, glm::vec4> GenItemPathAndColor(Res * res);

private:
    //  �ؼ�
    SharePtr<UIObjectLayout> mListLayout;
    SharePtr<UIObjectLayout> mTypeLayout;
    SharePtr<UIObjectLayout> mRefsLayout;
    SharePtr<UIObjectTextBox> mSearchText;
    //  ����
    SelectFn_t               mOptSelect;
    std::string              mPreSearch;
    std::vector<std::string> mLimitType;
    //  �ڲ�״̬
    std::vector<SearchItem> mSearchItems;       //  �������
    SearchStat              mSearchStat;        //  ����״̬
    SharePtr<UIObject>      mLastSelect;        //  ���ѡ��
    Project *               mProject;           //  ��ǰ��Ŀ

    std::map<Res *, SharePtr<UIObject>> mRes2Obj;
    std::map<SharePtr<UIObject>, Res *> mObj2Res;
};
