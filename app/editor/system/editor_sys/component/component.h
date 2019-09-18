#pragma once

#include "gl_object.h"
#include "../property/property.h"
#include "../../tools/parser_tool.h"

class Component {
public:
    struct Property {
        std::string mName;
        size_t      mType;
        void *      mMember;

        Property(): mType(0), mMember(nullptr)
        { }

        Property(const std::string & name, size_t type, void * member)
            : mName(name), mType(type), mMember(member)
        { }
    };

public:
    static Component * Create(const std::string & tag);

public:
	Component()
        : _owner(nullptr)
        , _active(true) { }
	virtual ~Component() { }
    virtual void OnAdd() = 0;
    virtual void OnDel() = 0;
    virtual void OnUpdate(float dt) = 0;

    bool IsActive() const { return _active; }
	void SetActive(bool active) { _active = active; }

    GLObject * GetOwner() { return _owner; }
	void SetOwner(GLObject * owner) { _owner = owner; }

    //  �������
    virtual const std::string & GetName() = 0;
    //  Property�޸�ʱ������
    virtual bool OnModifyProperty(
        const std::any &    value, 
        const std::string & title, 
        const std::any &    backup) = 0;
    //  ����Key, Val, ��ʼ����Ӧ�ĳ�Ա����
    virtual bool ParseProperty(
        const std::string & key, 
        const std::string & val) = 0;
    //  ����Property�б�, ���ڽ���չʾ�޸�
    virtual std::vector<Property> CollectProperty() = 0;

private:
    bool _active;
    GLObject *_owner;
};