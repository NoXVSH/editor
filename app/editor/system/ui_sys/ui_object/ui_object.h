#pragma once

#include "../include.h"
#include "../ui_event.h"
#include "../ui_state/ui_state.h"

#ifdef GetObject
#undef GetObject
#endif

class UIObject: public std::enable_shared_from_this<UIObject> {
public:
    template <class T = UIState>
    T * GetState() { return (T *)_state.get(); }
    
    SharePtr<UIObject> GetObject(const std::initializer_list<std::string> & list);
    std::vector<SharePtr<UIObject>>   GetObjects(UITypeEnum type);
    std::vector<SharePtr<UIObject>> & GetObjects();
    void AddObject(const SharePtr<UIObject> & object);
    void DelObject(const SharePtr<UIObject> & object);
    void DelObject(size_t index);
    void DelThis();
    void ClearObjects();
    SharePtr<UIObject> GetParent();
    SharePtr<UIObject> GetRoot();
    UITypeEnum GetType();
    bool IsVisible();

    void ResetLayout();
    void ApplyLayout();
    void Render(float dt, bool visible = true);

    glm::vec4 CalcStretech(DirectEnum direct, const glm::vec2 & offset);
    glm::vec2 ToWorldCoord(const glm::vec2 & coord = glm::vec2(0));
    glm::vec4 ToLocalCoord(const glm::vec4 & coord);
    glm::vec2 ToLocalCoord(const glm::vec2 & coord);
    glm::vec2 ToLocalCoordFromImGUI();
    glm::vec4 ToWorldRect();

    //  ���¼�ί��, �¼��������ݵ�ί����
    void BindDelegate(UIEvent::DelegateHandler * delegate);

protected:
    UIObject(UITypeEnum type, UIState * state) 
        : _type(type)
        , _state(state)
    { }

    virtual ~UIObject()
    { }

    void RenderDrag();
    void UpdateSize();
    void UpdateMove();
    virtual bool OnEnter();
    virtual void OnLeave(bool ret);
    virtual void OnResetLayout();
    virtual void OnApplyLayout();

    //  �¼�����
    void DispatchEventKey();
    SharePtr<UIObject> DispatchEventKey(const UIEvent::Key & param);
    void DispatchEventDrag();
    SharePtr<UIObject> DispatchEventDrag(const UIEvent::Drag & param);
    void DispatchEventMouse();
    SharePtr<UIObject> DispatchEventMouse(const UIEvent::Mouse & param);
    virtual bool OnCallEventMessage(UIEventEnum e, const UIEvent::Event & param);

private:
    SharePtr<UIObject> CallEventMessage(UIEventEnum e, const UIEvent::Event & param);
public:
    SharePtr<UIObject> PostEventMessage(UIEventEnum e, const UIEvent::Event & param);

private:
    bool                                _visible;       //  ��ǵ�ǰ�ڵ��Ƿ���Ⱦ
    UITypeEnum                          _type;
    WeakPtr<UIObject>                   _parent;
    UniquePtr<UIState>                  _state;
    std::vector<SharePtr<UIObject>>     _children;
    UniquePtr<UIEvent::DelegateHandler> _delegate;
};

class UIClassLayout : public UIObject {
public:
    UIClassLayout();

private:
    virtual bool OnEnter() override;
    virtual void OnLeave(bool ret) override;
    virtual void OnResetLayout() override;
    virtual void OnApplyLayout() override;

    bool IsCanStretch(DirectEnum edge);
    bool IsCanStretch(DirectEnum edge, const glm::vec2 & offset);
    void HandleStretch();
};

class UIClassTreeBox : public UIObject {
public:
    UIClassTreeBox();

private:
    virtual bool OnEnter() override;
    virtual void OnLeave(bool ret) override;
};

class UIClassTextBox : public UIObject {
public:
    UIClassTextBox();

private:
    virtual bool OnEnter() override;
};

class UIClassImageBox : public UIObject {
public:
    UIClassImageBox();

private:
    virtual bool OnEnter() override;
};

class UIClassComboBox : public UIObject {
public:
    UIClassComboBox();

private:
    virtual bool OnEnter() override;
    virtual void OnLeave(bool ret) override;
    virtual bool OnCallEventMessage(UIEventEnum e, const UIEvent::Event & param) override;
};

class UIClassUICanvas : public UIObject {
public:
    UIClassUICanvas();
};

class UIClassGLCanvas : public UIObject {
public:
    void Post(const UIStateGLCanvas::PreCommand & cmd);
    void Post(const UIStateGLCanvas::PostCommand & cmd);
    glm::mat4 GetMatrixMVP();
    const glm::mat4 & GetMatrixView();
    const glm::mat4 & GetMatrixProj();
    const glm::mat4 & GetMatrixModel();

    virtual bool OnEnter() override;

    UIClassGLCanvas();

private:
    void HandlePostCommands();
    void HandlePreCommands();
    void CollectCommands();
    void HandleCommands();
};