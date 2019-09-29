#pragma once

#include "../include.h"

class UIState {
public:
    virtual ~UIState()
    { }

    bool FromStringParse(const std::string & key, const std::string & val);

    int Align               = 0;        //  ���뷽ʽ
    
    bool IsMulti            = false;    //  ����
    bool IsSelect           = false;    //  ѡ��
    bool IsWindow           = false;    //  ����
    bool IsButton           = false;    //  ��ť
    bool IsShowNav          = false;    //  ����
    bool IsEditBox          = false;    //  �༭
    bool IsCanMove          = false;    //  ���ƶ�
    bool IsShowBorder       = true;     //  ��ʾ�߿�
    bool IsCanStretch       = true;     //  ������
    bool IsFullScreen       = false;    //  ȫ��
    bool IsCanDragMove      = false;    //  ���Ϸ��ƶ�
    bool IsCanDragFree      = false;    //  ���Ϸ��ͷ�
    bool IsShowMenuBar      = false;    //  ��ʾ�˵�
    bool IsShowTitleBar     = false;    //  ��ʾ����
    bool IsShowScrollBar    = false;    //  ��ʾ������

    float BorderNumber      = 1;        //  �߿���

    std::string Name;                   //  ����
    std::string Tips;                   //  ˵��
    std::string LSkin;                  //  Ƥ��

    glm::vec2 StretchMin;               //  ��С�ߴ�
    glm::vec4 Move;                     //  ��λ
    glm::vec4 Move_;                    //  ��һ֡��λ

    std::vector<std::string> MenuBar;   //  �˵���

    std::any UserData;                  //  �û�����
};

class UIStateLayout : public UIState {
public:
    //  �Ϸ���Ϣ
    struct Drag {
        DirectEnum mDirect;
        glm::vec2 mBegWorld;
        glm::vec2 mEndWorld;
        UIObject * mDragObj;
        UIObject * mFreeObj;
    } mDrag;

    //  ������Ϣ
    struct Join {
        std::vector<UIObject *>           mOut;
        std::pair<UIObject *, DirectEnum> mIn;
        Join() {}
    } mJoin[(size_t)DirectEnum::Length];

    //  ��������
    struct StretchFocus {
        UIObject *  mObject;
        DirectEnum  mDirect;
        StretchFocus() : mObject(nullptr) {}
    } mStretchFocus;

    UIStateLayout();
};

class UIStateTreeBox : public UIState {
public:
    UIStateTreeBox();
};

class UIStateTextBox : public UIState {
public:
    std::string mBuffer;

    UIStateTextBox();
};

class UIStateImageBox : public UIState {
public:
    UIStateImageBox();
};

class UIStateComboBox : public UIState {
public:
    std::string mSelected;

    UIStateComboBox();
};

class UIStateUICanvas : public UIState {
public:
    UIStateUICanvas();
};

class UIStateGLCanvas : public UIState {
public:
    UIStateGLCanvas();
};
