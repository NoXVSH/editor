#pragma once

#include "gl_object.h"
#include "../../interface/serializer.h"

class UIObject;

class Component
    : public Interface::Serializer 
    , public std::enable_shared_from_this<Component> {
public:
    enum StatusEnum {
        kActive = 1 << 0,   //  ����
        kInsert = 1 << 1,   //  ֧�ֲ������
        kDelete = 1 << 2,   //  ֧��ɾ������
        kModify = 1 << 3,   //  ֧���޸Ĳ���
    };

    struct Property {
        Interface::Serializer::StringValueTypeEnum mType;
        std::string                                mName;
        void *                                     mMember;

        Property()
            : mType(Interface::Serializer::StringValueTypeEnum::kERR)
            , mMember(nullptr)
        { }

        Property(
            Interface::Serializer::StringValueTypeEnum type, 
            const std::string & name, void * member)
            : mType(type), mName(name), mMember(member)
        { }
    };

public:
    //  �������
    static SharePtr<Component> Create(const std::string & name);

public:
	Component(): _status(kActive) { }
	virtual ~Component() { }
    virtual void OnAdd() = 0;
    virtual void OnDel() = 0;
    virtual void OnUpdate(float dt) = 0;

    bool IsActive() const { return _status & kActive; }
	void SetActive(bool active) 
    {  
        if (active)
            _status |=  kActive;
        else
            _status &= ~kActive;
    }

    SharePtr<GLObject> GetOwner() { return _owner->shared_from_this(); }
	void SetOwner(GLObject * owner) { _owner = owner; }

    //  �������
    virtual const std::string & GetName() = 0;
    //  Property�޸�ʱ������
    virtual bool OnModifyProperty(
        const std::any & value, 
        const std::any & backup,
        const std::string & title) = 0;
    //  ����Property�б�, ���ڽ���չʾ�޸�
    virtual std::vector<Property> CollectProperty() = 0;
    std::vector<SharePtr<UIObject>> CreateUIPropertys();

private:
    size_t              _status;
    GLObject *          _owner;
};