#pragma once

#include "../../base.h"

class EventSys {
public:
    //  �¼�ö��
    enum class TypeEnum {
        //  project
        kOpenProject,       //  {}
        kSaveProject,       //  {}
        kFreeProject,       //  {}

        //  gl object
        kSelectObject,    //  { GLOBject ѡ�еĽڵ�, bool �Ƿ�ѡ��, bool �Ƿ��ѡ }
        kDeleteObject,    //  { GLOBject ɾ���Ľڵ� }
        kInsertObject,    //  { GLObject �����Ľڵ�  }
        kRenameObject,    //  { GLOBject ѡ�еĽڵ�, string ������ }
        kStateObject,     //  { GLOBject ѡ�еĽڵ�, uint ��״̬, uint ��״̬ }
        kMoveObject,      //  { GLOBject ѡ�еĽڵ�, GLObject ���ڵ�, uint λ�� }

        //  component
        kDeleteComponent,   //  { GLOBject ѡ�еĽڵ�, Component ѡ�е���� }
        kAppendComponent,   //  { GLOBject ѡ�еĽڵ�, Component ��������� }

        //  resource
        kSetResType,        //  { Res ѡ�е���Դ, Res::TypeEnum ���� }
        kRenameRes,         //  { Res ѡ�е���Դ, string ��·��, string ��·�� }
        kDeleteRes,         //  { Res ѡ�е���Դ, id ѡ�е���Դ, string ��·�� }
    };

    using func_t = std::function<void(TypeEnum type, const std::any &)>;


    //  ������
    class Listener {
    public:
        Listener(): mOwner(nullptr)
        { }

        ~Listener()
        {
            ASSERT_LOG(_listens.empty() || mOwner != nullptr, "");
            for (auto listen : _listens)
            {
                mOwner->Del(listen);
            }
        }

        void Add(TypeEnum id, const func_t & func, EventSys * owner = nullptr)
        {
            if (owner != nullptr) { mOwner = owner; }
            ASSERT_LOG(mOwner != nullptr, "");
            _listens.push_back(mOwner->Add(id, func));
        }

    private:
        EventSys * mOwner;
        std::vector<size_t> _listens;
    };

    //  �¼�����
    struct Event {
        size_t mID;
        func_t mFunc;
        Event(size_t id, const func_t & func): mID(id), mFunc(func)
        { }

        bool operator ==(size_t id) const { return mID == id; }
    };

private:
    static size_t s_coundID;

public:
    EventSys()
    { }

    ~EventSys()
    { }

    size_t Add(TypeEnum type, const func_t & func)
    {
        auto insert = _events.insert(std::make_pair(type, std::vector<Event>()));
        insert.first->second.emplace_back(EventSys::s_coundID, func);
        return EventSys::s_coundID++;
    }

    void Del(size_t id)
    {
        for (auto & events : _events)
        {
            auto it = std::find(events.second.begin(),
                                events.second.end(), id);
            if (it != events.second.end())
            {
                events.second.erase(it);
            }
        }
    }

    void Post(TypeEnum type, std::any param)
    {
        auto it = _events.find(type);
        if (it != _events.end())
        {
            for (auto & e : it->second)
            {
                e.mFunc(type, param);
            }
        }
    }

private:
    std::map<TypeEnum, std::vector<Event>> _events;
};

