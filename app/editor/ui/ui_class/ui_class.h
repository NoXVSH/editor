#pragma once

#include "../base.h"

class UIClass {
public:
    struct EventDetails {
        //  ���ذ��µ�״̬��
        static int CheckStateKey();

        //  ��������
        static int CheckMouseKey(UIEventEnum e, bool repeat);

        //  ���ؼ��̰��¼�
        static int CheckKeyPressKey();
    };

    //  �����¼�
    struct EventKey {
        int mKey;
        int mAct;   //  0, 1, 2 => ����, ̧��, ��������
        int mState; //  1, 2, 4 => alt, ctrl, shift
        EventKey() { memset(this, sizeof(EventKey), 0); }
    };

    //  ����¼�
    struct EventMouse {
        int mKey;   //  0, 1, 2          => ���, �Ҽ�, �м�
        int mAct;   //  0, 1, 2, 3, 4, 5 => ����, ����, ̧��, ˫��, ����, �����ӳ�
        int mState; //  1, 2, 4          => alt, ctrl, shift
        EventMouse() { memset(this, sizeof(EventMouse), 0); }
    };

    //  �༭�ı��¼�
    struct EventEditText {
        std::string mText;
    };

public:
    template <class T = UIState>
    T * GetState()
    {
        return (T *)_state;
    }

    UITypeEnum GetType() const;

    std::vector<UIClass *>   GetChildren(UITypeEnum type);
    std::vector<UIClass *> & GetChildren();
    void AddChild(UIClass * child);
    void DelChild(UIClass * child);
    void ClearChild();
    UIClass * GetRoot();
    UIClass * GetParent();

    void Render(float dt);
    void ResetLayout();
    void ApplyLayout();

    glm::vec4 CalcStretech(DirectEnum direct, const glm::vec2 & offset);
    glm::vec2 ToWorldCoord(const glm::vec2 & coord = glm::vec2(0));
    glm::vec4 ToLocalCoord(const glm::vec4 & coord);
    glm::vec2 ToLocalCoord(const glm::vec2 & coord);

protected:
    UIClass(UITypeEnum type, UIState * state) 
        : _type(type)
        , _state(state)
        , _parent(nullptr)
    { }

    void LockPosition();
    virtual bool OnEnter();
    virtual void OnLeave(bool ret);
    virtual void OnResetLayout();
    virtual void OnApplyLayout();
    virtual void OnRender(float dt) = 0;

    int PostEventMessage(UIEventEnum e, UIClass * object);
    int CallEventMessage(UIEventEnum e, UIClass * object, const std::any & param);

private:
    UITypeEnum             _type;
    UIState *              _state;
    UIClass *              _parent;
    std::vector<UIClass *> _children;
};

class UIClassTree : public UIClass {
public:
    UIClassTree(UIState * state): UIClass(UITypeEnum::kTREE, state)
    { }

private:
    virtual bool OnEnter() override;
    virtual void OnLeave(bool ret) override;
    virtual void OnRender(float dt) override;
};

class UIClassImage : public UIClass {
public:
    UIClassImage(UIState * state): UIClass(UITypeEnum::kIMAGE, state)
    { }

private:
    virtual void OnRender(float dt) override;
};

class UIClassLayout : public UIClass {
public:
    UIClassLayout(UIState * state): UIClass(UITypeEnum::kLAYOUT, state)
    { }

private:
    virtual bool OnEnter() override;
    virtual void OnLeave(bool ret) override;
    virtual void OnResetLayout() override;
    virtual void OnApplyLayout() override;
    virtual void OnRender(float dt) override;

    bool IsCanDrag(DirectEnum edge);
    bool IsCanDrag(DirectEnum edge, const glm::vec2 & offset);
};

class UIClassTextBox : public UIClass {
public:
    UIClassTextBox(UIState * state) : UIClass(UITypeEnum::kTEXTBOX, state)
    { }

private:
    virtual void OnRender(float dt) override;
};

class UIClassComboBox : public UIClass {
public:
    UIClassComboBox(UIState * state) : UIClass(UITypeEnum::kCOMBOBOX, state)
    { }

private:
    virtual bool OnEnter() override;
    virtual void OnLeave(bool ret) override;
    virtual void OnRender(float dt) override;
};

class UIClassUICanvas : public UIClass {
public:
    UIClassUICanvas(UIState * state) : UIClass(UITypeEnum::kUICONVAS, state)
    { }

private:
    virtual void OnRender(float dt) override;
};

class UIClassGLCanvas : public UIClass {
public:
    UIClassGLCanvas(UIState * state) : UIClass(UITypeEnum::kGLCONVAS, state)
    { }

private:
    virtual void OnRender(float dt) override;
};