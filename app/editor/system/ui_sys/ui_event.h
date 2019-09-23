#pragma once

#include "include.h"

class UIEvent {
public:
    //  ���ذ��µ�״̬��
    static int CheckStateKey();

    struct Event {
        mutable UIObject * mObject;

        Event(UIObject * object = nullptr) : mObject(object)
        {  }
    };

    //  �����¼�
    struct Key : Event {
        //  ֧�ֵ��ȼ���Ӧ
        static std::vector<int> Hotkeys;

        int mKey;
        int mAct;   //  0, 1, 2 => ����, ̧��, ����
        int mState; //  1, 2, 4 => alt, ctrl, shift

        Key(const int act, const int key, UIObject * object = nullptr)
            : Event(object)
            , mKey(key)
            , mAct(act)
            , mState(CheckStateKey())
        { }
    };

    //  ����¼�
    struct Mouse : Event {
        int mKey;           //  0, 1, 2       => ���, �Ҽ�, �м�
        int mAct;           //  0, 1, 2, 3, 4 => ����, ����, ̧��, ����, ˫��
        int mState;         //  1, 2, 4       => alt, ctrl, shift
        glm::vec2 mMouse;   //  �������

        Mouse(const int act, const int key, UIObject * object = nullptr)
            : Event(object)
            , mAct(act)
            , mKey(key)
            , mState(CheckStateKey())
        {
            mMouse.x = ImGui::GetMousePos().x;
            mMouse.y = ImGui::GetMousePos().y;
        }
    };

    //  �˵��¼�
    struct Menu : Event {
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
    struct EditText : Event {
        std::string mString;

        EditText(const std::string & string, UIObject * object = nullptr)
            : Event(object)
            , mString(string)
        { }
    };

    //  �¼�����
    struct Delegate : Event {
        int mType;      //  0 ��ʼ��, 1 ����

        Delegate(int type) : mType(type)
        {  }
    };

    //  �¼�����
    class DelegateHandler {
    public:
        virtual bool OnCallEventMessage(UIEventEnum type, const Event & event, UIObject * object)
        {
            if (type == UIEventEnum::kDELEGATE)
            {
                auto & delegate = (const Delegate &)event;
                if (delegate.mType == 0)
                {
                    _onwer = delegate.mObject;
                }
            }
            return false;
        }

        UIObject * GetOnwer()
        {
            return _onwer;
        }
    private:
        UIObject * _onwer;
    };
};