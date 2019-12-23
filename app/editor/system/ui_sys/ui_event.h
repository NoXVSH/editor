#pragma once

#include "include.h"

class UIEvent {
public:
    //  ���ذ��µ�״̬��
    static int CheckStateKey();

    struct Event {
        mutable SharePtr<UIObject> mObject;

        Event(const SharePtr<UIObject> & object = nullptr) : mObject(object) { }
    };

    //  �����¼�
    struct Key : Event {
        //  ֧�ֵ��ȼ���Ӧ
        static std::vector<int> Hotkeys;

        int mKey;
        int mAct;   //  0, 1, 2 => ����, ̧��, ����
        int mState; //  1, 2, 4 => alt, ctrl, shift

        Key(const int act, const int key, const SharePtr<UIObject> & object = nullptr)
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
        int mWheel;         //  ���ֹ���
        glm::vec2 mDelta;   //  ����ƫ��
        glm::vec2 mMouse;   //  �������
        glm::vec2 mFirst;   //  ��ʼ����

        Mouse(const int act, const int key, const SharePtr<UIObject> & object = nullptr)
            : Event(object)
            , mAct(act)
            , mKey(key)
            , mState(CheckStateKey())
        {
            mMouse.x = ImGui::GetMousePos().x;
            mMouse.y = ImGui::GetMousePos().y;
            mDelta = ImGui::GetIO().MouseDelta;
            mWheel = (int)ImGui::GetIO().MouseWheel;
            mFirst = ImGui::GetIO().MouseDownPoints[mKey];
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
            , mSelect(false)
        { }

        Menu(const std::string & path, const std::string & edit)
            : mPath(path)
            , mEdit(edit)
            , mSelect(false)
        { }
    };

    //  �Ϸ��¼�
    struct Drag : Event {
        DirectEnum mDirect;
        SharePtr<UIObject> mDragObj;
        SharePtr<UIObject> mFreeObj;
        glm::vec2 mBegWorld;
        glm::vec2 mEndWorld;
        int mAct;               //  0 ����, 1 �϶�, 2 �ͷ�

        Drag(int act, const glm::vec2 & beg, 
            const SharePtr<UIObject> & dragObj = nullptr,
            const SharePtr<UIObject> & freeObj = nullptr,
            const DirectEnum direct = DirectEnum::kNone) 
            : mAct(act)
            , mBegWorld(beg)
            , mEndWorld(beg)
            , mDragObj(dragObj)
            , mFreeObj(freeObj)
            , mDirect(direct)
        {
            if (mAct == 1 || mAct == 2)
            {
                mEndWorld = ImGui::GetMousePos();
            }
        }
    };

    //  �༭�ı��¼�
    struct Edit : Event {
        std::string mString;

        Edit(const std::string & string, const SharePtr<UIObject> & object = nullptr)
            : Event(object)
            , mString(string)
        { }
    };

    //  ��ʼ���¼�
    struct Init : Event {
        std::any mParam;

        Init(const std::any & param) : mParam(param)
        {  }
    };

    //  �¼�����
    class DelegateHandler {
    public:
        virtual bool OnCallEventMessage(UIEventEnum type, const Event & event) = 0;

        SharePtr<UIObject> GetOwner()
        {
            ASSERT_LOG(!mOwner.expired(), "");
            return mOwner.lock();
        }

        void SetOwner(const SharePtr<UIObject> & owner)
        {
            mOwner = owner;
        }

        virtual ~DelegateHandler() { }
    private:
        WeakPtr<UIObject> mOwner;
    };
};