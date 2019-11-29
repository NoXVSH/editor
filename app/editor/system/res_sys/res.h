#pragma once

#include "../../include.h"

class Res {
public:
    enum TagEnum {
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

    void SetTag(TagEnum tag);
    TagEnum GetTag();

    std::string GetPath();

private:
    uint                _id;
    TagEnum             _tag;   //  ��ǩ
    std::vector<Ref *>  _refs;  //  ���ü�
    std::string         _metas; //  �ַ�Ԫ����
    uint                _metai; //  ��ֵԪ����
};