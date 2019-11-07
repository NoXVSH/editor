#pragma once

#include "../include.h"
#include "../../interface/render.h"

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

    std::vector<std::string> MenuBar;   //  �˵���
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

class GLMesh;
class GLObject;
class Component;
class GLProgram;
class GLTexture;
class GLMaterial;

class UIStateGLCanvas : public UIState {
public:
    GLuint mRenderTarget;
    GLuint mRenderTextures[2];
    interface::MatrixStack mMatrixStack;
    std::vector<interface::PostCommand>   mPostCommands;
    std::vector<interface::FowardCommand> mFowardCommands;

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

        float       mScale;
        glm::vec3   mCoord;
        size_t      mOpMode;

        //  ��ʼ��
        Operation()
            : mOpMode(0), mScale(1)
        { }
    } mOperation;

    std::vector<SharePtr<GLMesh>> mMeshBuffer;
    SharePtr<GLProgram> mGLProgramSolidFill;

public:
    UIStateGLCanvas();
    ~UIStateGLCanvas();
};