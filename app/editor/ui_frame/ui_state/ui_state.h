#pragma once

#include "../base.h"

class UIState {
public:
    static bool OpEqual(const UIState * state, const std::string & name)
    {
        return state->mName == name;
    }

    //  UIƤ��
    struct Skin {
        enum class TypeEnum {
            kDISABLED,              //  ����
            kNORMAL,                //  ����
            kOVER,                  //  ����
            kPRESSED,               //  ����
            kSELECTED_DISABLED,     //  ѡ�н�ֹ
            kSELECTED_NORMAL,       //  ѡ������
            kSELECTED_OVER,         //  ѡ������
            kSELECTED_PRESSED,      //  ѡ�а���
            LENGTH,
        };
        std::string mTexture;
        glm::vec4 mUV[(size_t)TypeEnum::LENGTH];
    };

    //  ���뷽ʽ
    struct Alignment {
        enum class TypeEnum {
            kDEFAULT,               //  ��������
            kSTRETCH,               //  ����
            kCENTER,                //  ����
            kLEFT_TOP,              //  ����ͣ��
            kLEFT_BOTTOM,           //  ����ͣ��
            kLEFT_VSTRETCH,         //  ��ͣ��, ��ֱ����
            kLEFT_VCENTER,          //  ��ͣ��, ��ֱ����
            kRIGHT_TOP,             //  ����ͣ��
            kRIGHT_BOTTOM,          //  ����ͣ��
            kRIGHT_VSTRETCH,        //  ��ͣ��, ��ֱ����
            kRIGHT_VCENTER,         //  ��ͣ��, ��ֱ����
            kHSTRETCH_TOP,          //  ��ͣ��, ˮƽ����
            kHSTRETCH_BOTTOM,       //  ��ͣ��, ˮƽ����
            kHSTRETCH_VSTRETCH,     //  ˮƽ����, ��ֱ����
            kHSTRETCH_VCENTER,      //  ��ֱ����, ˮƽ����
            kHCENTER_TOP,           //  ��ͣ��, ˮƽ����
            kHCENTER_BOTTOM,        //  ��ͣ��, ��ֱ����
            kHCENTER_VSTRETCH,      //  ˮƽ����, ��ֱ����
            kHCENTER_VCENTER,       //  ˮƽ����, ��ֱ����
            LENGTH,
        };
        glm::vec4 mMargin;
        glm::vec4 mOrigin;
        TypeEnum    mType;
    };

public:
    UIState();

    virtual ~UIState()
    { }

    //  ����״̬
    size_t          mID;
    Skin            mSkin;                          //  Ƥ��
    glm::vec4       mMove;                          //  ��λ
    std::string     mName;                          //  ����
    std::string     mTips;                          //  ������ʾ
    std::string     mTitle;                         //  ��ʾ����
    glm::vec4       mColor;                         //  �����ɫ
    bool            mEnabled;                       //  �Ƿ�����
    bool            mVisible;                       //  �Ƿ���ʾ
    bool            mEnabledKey;                    //  �Ƿ���ռ����¼�
    bool            mEnabledMouse;                  //  �Ƿ��������¼�
    Alignment       mAlignment;                     //  ����ģʽ
    UITypeEnum      mUIType;                        //  ��������
    std::map<std::string, std::string> mUserData;   //  �û�����
};

class UIStateWindow : public UIState {
public:
    bool mIsStretch;
    bool mFullScreen;
    UIStateWindow();
};

class UIStateDDContainer : public UIState {
public:
    struct Layout {
        struct LinkEdge {
            UIStateDDContainer * mDDContainer;
            DirectEnum           mEdge;
        };
        std::vector<UIStateDDContainer *> mLinks[(size_t)DirectEnum::LENGTH]; //  ������Panel
        std::vector<LinkEdge>             mEdges[(size_t)DirectEnum::LENGTH]; //  ������Edge
    };

    UIStateDDContainer();

    Layout mLayout;
};

class UIStateButton : public UIState {
public:
};