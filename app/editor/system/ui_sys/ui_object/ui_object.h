#pragma once

#include "../include.h"

class UIObject {
public:
    struct UIEventDetails {
        //  ���ذ��µ�״̬��
        static int CheckStateKey();

        struct Base {
            mutable UIObject * mObject;

            Base(UIObject * object = nullptr) : mObject(object)
            {  }
        };

        //  �����¼�
        struct Key : Base {
            //  ֧�ֵ��ȼ���Ӧ
            static std::vector<int> Hotkeys;

            int mKey;
            int mAct;   //  0, 1, 2 => ����, ̧��, ����
            int mState; //  1, 2, 4 => alt, ctrl, shift

            Key(const int act, const int key, UIObject * object = nullptr)
                : Base(object)
                , mKey(key)
                , mAct(act)
                , mState(CheckStateKey())
            { }
        };

        //  ����¼�
        struct Mouse : Base {
            int mKey;           //  0, 1, 2       => ���, �Ҽ�, �м�
            int mAct;           //  0, 1, 2, 3, 4 => ����, ����, ̧��, ����, ˫��
            int mState;         //  1, 2, 4       => alt, ctrl, shift
            glm::vec2 mMouse;   //  �������

            Mouse(const int act, const int key, UIObject * object = nullptr)
                : Base(object)
                , mAct(act)
                , mKey(key)
                , mState(CheckStateKey())
            {
                mMouse.x = ImGui::GetMousePos().x;
                mMouse.y = ImGui::GetMousePos().y;
            }
        };

        //  �˵��¼�
        struct Menu : Base {
            bool mSelect;
            std::string mEdit;
            std::string mPath;

            Menu(const std::string & path, const bool select)
                : mPath(path)
                , mSelect(select)
            { }

            Menu(const std::string & path, const char * edit)
                : mPath(path)
                , mEdit(edit)
            { }

            Menu(const std::string & path, const std::string & edit)
                : mPath(path)
                , mEdit(edit)
            { }
        };

        //  �༭�ı��¼�
        struct EditText : Base {
            std::string mString;

            EditText(const std::string & string, UIObject * object = nullptr)
                : Base(object)
                , mString(string)
            { }
        };

        //  �¼�����
        struct Delegate : Base {
            int mType;      //  0 ��ʼ��, 1 ����

            Delegate(int type) : mType(type)
            {  }
        };
    };

    //  �¼�����
    class UIEventDelegate {
    public:
        virtual bool OnCallEventMessage(UIObject * object, UIEventEnum e, const UIEventDetails::Base & param)
        {
            return false;
        }
    };

public:
    template <class T = UIState>
    T * GetState() { return (T *)_state; }
    
    UIObject * GetObjects(const std::initializer_list<std::string> & list);
    std::vector<UIObject *>   GetObjects(UITypeEnum type) const;
    std::vector<UIObject *> & GetObjects();
    void AddObject(UIObject * child);
    void DelObject(UIObject * child);
    void DelThis();
    void ClearObjects();
    UIObject * GetRoot();
    UIObject * GetParent();
    bool IsVisible() const;

    void ResetLayout();
    void ApplyLayout();
    void Render(float dt, bool visible = true);

    glm::vec4 CalcStretech(DirectEnum direct, const glm::vec2 & offset) const;
    glm::vec2 ToWorldCoord(const glm::vec2 & coord = glm::vec2(0)) const;
    glm::vec4 ToLocalCoord(const glm::vec4 & coord) const;
    glm::vec2 ToLocalCoord(const glm::vec2 & coord) const;
    glm::vec4 ToWorldRect() const;
    glm::vec2 ToLocalCoordFromImGUI() const;

    //  ���¼�ί��, �¼��������ݵ�ί����
    void BindDelegate(UIEventDelegate * delegate);

    //  const ���ذ汾
    UITypeEnum GetType() const
    {
        return _type;
    }

    template <class T = UIState>
    const T * GetState() const
    {
        return const_cast<UIObject *>(this)->GetState<T>();
    }

    const UIObject * GetRoot() const
    {
        return const_cast<UIObject *>(this)->GetRoot();
    }

    const UIObject * GetParent() const
    {
        return const_cast<UIObject *>(this)->GetParent();
    }

    const std::vector<UIObject *> & GetObjects() const
    {
        return const_cast<UIObject *>(this)->GetObjects();
    }

    const UIObject * GetObjects(const std::initializer_list<std::string> & list) const
    {
        return const_cast<UIObject *>(this)->GetObjects(list);
    }

protected:
    UIObject(UITypeEnum type, UIState * state) 
        : _type(type)
        , _state(state)
        , _parent(nullptr)
        , _delegate(nullptr)
    { }

    void AdjustSize();
    void LockPosition();
    virtual bool OnEnter();
    virtual void OnLeave(bool ret);
    virtual void OnRender(float dt);
    virtual void OnResetLayout();
    virtual void OnApplyLayout();

    //  �¼�����
    void DispatchEventK();
    bool DispatchEventK(const UIEventDetails::Key & param);
    void DispatchEventM();
    bool DispatchEventM(const UIEventDetails::Mouse & param);
    virtual bool OnCallEventMessage(UIEventEnum e, const UIEventDetails::Base & param);

private:
    bool CallEventMessage(UIEventEnum e, const UIEventDetails::Base & param);
public:
    bool PostEventMessage(UIEventEnum e, const UIEventDetails::Base & param);

private:
    UITypeEnum                       _type;
    UIState *                        _state;
    UIObject *                       _parent;
    bool                             _visible;       //  ��ǵ�ǰ�ڵ��Ƿ���Ⱦ
    std::vector<UIObject *>          _children;
    std::unique_ptr<UIEventDelegate> _delegate;
};

class UIClassLayout : public UIObject {
public:
    UIClassLayout();

private:
    virtual bool OnEnter() override;
    virtual void OnLeave(bool ret) override;
    virtual void OnResetLayout() override;
    virtual void OnApplyLayout() override;
    virtual void OnRender(float dt) override;

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
    virtual void OnRender(float dt) override;
};

class UIClassImageBox : public UIObject {
public:
    UIClassImageBox();

private:
    virtual void OnRender(float dt) override;
};

class UIClassComboBox : public UIObject {
public:
    UIClassComboBox();

private:
    virtual bool OnEnter() override;
    virtual void OnLeave(bool ret) override;
    virtual void OnRender(float dt) override;
    virtual bool OnCallEventMessage(UIEventEnum e, const UIEventDetails::Base & param) override;
};

class UIClassUICanvas : public UIObject {
public:
    UIClassUICanvas();

private:
    virtual void OnRender(float dt) override;
};

class UIClassGLCanvas : public UIObject {
public:
    UIClassGLCanvas();

private:
    virtual void OnRender(float dt) override;
};