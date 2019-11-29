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

    std::any Instance();
    std::string Path();
    uint GetRefCount();
    uint GetID();
    
    Ref * AppendRef();
    void  DeleteRef(Ref * ref);

    TypeEnum Type();
    TypeEnum Type(TypeEnum type);

    void BindMeta(const uint          val);
    void BindMeta(const std::string & val);

private:
    void WakeRefs();

private:
    uint                _id;
    TypeEnum            _type;  //  ����
    std::vector<Ref *>  _refs;  //  �����б�
    std::string         _metas; //  �ַ�Ԫ����
    uint                _metai; //  ��ֵԪ����
};