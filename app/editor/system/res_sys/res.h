#pragma once

#include "../../include.h"
#include "../interface/serializer.h"

class Res {
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
        Ref(Res * owner);
        ~Ref();

        Ref * Clone();
        bool IsModify();
        void SetModify();

        template <class T>
        T Load()
        {
            ASSERT_LOG(_owner != nullptr, "");
            _modify = false;
            return std::any_cast<T>(_owner->Load());
        }

    private:
        Ref(const Ref & other) = delete;
        Ref & operator=(const Ref & other) = delete;

    private:
        Res * _owner;
        bool  _modify;
    };

public:
    Res(uint id);
    ~Res();

    std::any Load();

    Ref* AppendRef();
    void DeleteRef(Ref * ref);

    void SetType(TypeEnum type);
    TypeEnum GetType();

    void BindMeta(uint                val);
    void BindMeta(const std::string & val);

    std::string GetPath();
    uint GetRefCount();
    uint GetID();

private:
    void WakeRefs();

private:
    uint                _id;
    TypeEnum            _type;  //  ��ǩ
    std::vector<Ref *>  _refs;  //  ���ü�
    std::string         _metas; //  �ַ�Ԫ����
    uint                _metai; //  ��ֵԪ����
};