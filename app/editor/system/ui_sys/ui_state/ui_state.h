#pragma once

#include "../include.h"
#include "../../interface/render.h"
#include "../../interface/serializer.h"

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
    bool IsSameline         = false;    //  ������
    bool IsShowBorder       = true;     //  ��ʾ�߿�
    bool IsCanStretch       = true;     //  ������
    bool IsFullScreen       = false;    //  ȫ��
    bool IsEditReturn       = false;    //  �༭���س�
    bool IsCanDragMove      = false;    //  ���Ϸ��ƶ�
    bool IsCanDragFree      = false;    //  ���Ϸ��ͷ�
    bool IsShowMenuBar      = false;    //  ��ʾ�˵�
    bool IsShowTitleBar     = false;    //  ��ʾ����
    bool IsShowScrollBar    = false;    //  ��ʾ������

    float BorderNumber      = 1;        //  �߿���

    std::string Name;                   //  ����
    std::string Tips;                   //  ˵��
    std::string LSkin;                  //  Ƥ��
    std::string LSkin_;                  // Ƥ��

    glm::vec2 StretchMin;               //  ��С�ߴ�
    glm::vec4 Move;                     //  ��λ
    glm::vec4 Move_;                    //  ��λ
    glm::vec4 Color = glm::vec4(1);     //  ��ɫ

    std::vector<std::string> MenuBar;   //  �˵���

    //  �������
    bool IsFirstRender      = true;     //  �Ƿ��һ����Ⱦ
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

    //  ��꽹��
    struct MouseFocus {
        WeakPtr<UIObject> mFocus;
        WeakPtr<UIObject> mDown;
        int               mKey;
    } mMouseFocus;

    //  ���콹��
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
    SharePtr<GLTexture> mSkinTex;

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
    struct TargetCommand {
        std::vector<interface::PostCommand>     mPostCommands;      //  ����
        std::vector<interface::FowardCommand>   mFowardCommands;    //  ����
        SharePtr<GLImage>                       mRenderTextures[2];
    };

public:
    GLuint mRenderTarget;
    SharePtr<GLImage> mRenderTextures[2];
    interface::MatrixStack    mMatrixStack;
    std::stack<TargetCommand>  mCommandStack;
    std::vector<TargetCommand> mCommandArray;

    //  �༭�������
    struct Operation {
        //  ����ģʽ
        enum OpModeEnum {
            kDrag = 0x1,        //  ��ק
            kEdit = 0x2,        //  �༭
            kSelect = 0x4,      //  ѡ��
        };

        //  ����Ķ���
        SharePtr<GLObject>  mEditObject;
        SharePtr<Component> mEditComponent;
        size_t              mEditTrackPoint;

        glm::vec4                       mSelectRect;
        std::vector<SharePtr<GLObject>> mSelectObjects;

        size_t      mOpMode;
        float       mViewScale;
        glm::vec3   mViewCoord;

        //  ��ʼ��
        Operation(): mOpMode(0), mViewScale(1)
        { }
    } mOperation;

    std::vector<SharePtr<GLMesh>> mMeshBuffer;
    SharePtr<GLProgram>   mGLProgramSolidFill;

public:
    UIStateGLCanvas();
    ~UIStateGLCanvas();
};