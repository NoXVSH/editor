#pragma once

#include "serializer.h"

class GLObject;

class Res : public Serializer {
public:
    enum TypeEnum {
        kNull,      //  ��
        kTxt,       //  �ı�
        kImg,       //  ͼƬ
        kMap,       //  ��ͼ
        kFnt,       //  ����
        kObj,       //  ����
        kVar,       //  ����
        kBlueprint, //  ��ͼ
        Length,
    };

    static const std::string sTypeString[TypeEnum::Length];

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

        std::string Path()
        {
            ASSERT_LOG(_owner != nullptr, "");
            return _owner->Path();
        }

        template <class T>
        SharePtr<T> Instance()
        {
            ASSERT_LOG(_owner != nullptr, "");
            Modify(false);
            return _owner->Instance<T>();
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

    //  ʵ��������
    template <class T>
    SharePtr<T> Instance()
    {
        static_assert(false);
    }

    template <>
    SharePtr<GLObject> Instance();

    std::string Path();
    uint GetRefCount();
    uint GetID();

    void WakeRefs();
    Ref * AppendRef();
    Ref * AppendRef(Ref * ref);
    void  DeleteRef(Ref * ref);

    TypeEnum Type();
    TypeEnum Type(TypeEnum type);

    const        std::string & TypeString();
    static const std::string & TypeString(uint type);

    template <class T>
    void BindMeta(const T & val)
    {
        if (!_meta.has_value() || std::any_cast<T &>(_meta) != val)
        {
            _meta = val; WakeRefs();
        }
    }

    virtual void EncodeBinary(std::ostream & os, Project * project) override;
    virtual void DecodeBinary(std::istream & is, Project * project) override;

private:
    uint                mID;
    TypeEnum            mType;      //  ����
    std::any            mMeta;      //  Ԫ����
    std::vector<Ref *>  mRefs;      //  �����б�
    Project *           mOwner;     //  ��Ŀ����
};