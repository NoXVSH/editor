#pragma once

#include "../../include.h"
#include "../interface/serializer.h"

class GLObject;
class Project;

class Res : public interface::Serializer {
public:
    enum TypeEnum {
        kNull,      //  ��
        kTxt,       //  �ı�
        kImg,       //  ͼƬ
        kMap,       //  ��ͼ
        kFont,      //  ����
        kObj,       //  ����
        kVar,       //  ����
        kBlueprint, //  ��ͼ
    };

    class Ref {
    public:
        static bool Check(Ref *& ref)
        {
            if (!ref->Check())
            {
                ref = nullptr;
            }
            return ref != nullptr;
        }

        Ref(Res * owner);
        ~Ref();

        Ref * Clone();
        bool Modify();
        bool Modify(bool modify);

        template <class T>
        T Instance()
        {
            ASSERT_LOG(_owner != nullptr, "");
            Modify(false);
            auto instance = _owner->Instance();
            return  std::any_cast<T>(instance);
        }

        bool Check()
        {
            return _owner != nullptr;
        }

        void EncodeBinary(Project * project, std::ofstream & os);
        void DecodeBinary(Project * project, std::ifstream & is);

    private:
        friend class Res;
        Ref(const Ref & other) = delete;
        Ref & operator=(const Ref & other) = delete;

    private:
        Res * _owner;
        bool  _modify;
    };

public:
    Res(Project * owner, uint id = ~0);
    ~Res();

    std::any Instance();
    std::string Path();
    uint GetRefCount();
    uint GetID();

    void WakeRefs();
    Ref * AppendRef();
    Ref * AppendRef(Ref * ref);
    void  DeleteRef(Ref * ref);

    TypeEnum Type();
    TypeEnum Type(TypeEnum type);

    template <class T>
    void BindMeta(const T & val)
    {
        if (!_meta.has_value() || std::any_cast<T &>(_meta) != val)
        {
            _meta = val; WakeRefs();
        }
    }

    virtual void EncodeBinary(std::ofstream & os) override;
    virtual void DecodeBinary(std::ifstream & is) override;

private:
    uint                _id;
    TypeEnum            _type;      //  ����
    std::any            _meta;      //  Ԫ����
    std::vector<Ref *>  _refs;      //  �����б�
    Project *           _owner;     //  ��Ŀ����
};