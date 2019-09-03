#pragma once

#include "../base.h"

class UIClass {
public:
    template <class T>
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

    virtual bool OnEnter();
    virtual void OnLeave();
    virtual void OnResetLayout();
    virtual void OnApplyLayout();
    virtual void OnRender(float dt) = 0;

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
    virtual void OnLeave() override;
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
    virtual void OnLeave() override;
    virtual void OnResetLayout() override;
    virtual void OnApplyLayout() override;
    virtual void OnRender(float dt) override;

    bool IsCanDrag(DirectEnum edge);
    bool IsCanDrag(DirectEnum edge, const glm::vec2 & offset);
};

class UIClassEditBox : public UIClass {
public:
    UIClassEditBox(UIState * state) : UIClass(UITypeEnum::kEDITBOX, state)
    { }

private:
    virtual void OnRender(float dt) override;
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