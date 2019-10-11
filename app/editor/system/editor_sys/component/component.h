#pragma once

#include "gl_object.h"
#include "../../interface/serializer.h"

class UIObject;
class UIObjectGLCanvas;

class Component
    : public Interface::Serializer 
    , public std::enable_shared_from_this<Component> {
public:
    enum StateEnum {
        kActive = 1 << 0,   //  ����
        kInsertCP = 1 << 1,   //  ֧�ֲ�����Ƶ�
        kDeleteCP = 1 << 2,   //  ֧��ɾ�����Ƶ�
        kModifyCP = 1 << 3,   //  ֧���޸Ŀ��Ƶ�
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

	Component(): _owner(nullptr), _state(kActive) { }
	virtual ~Component() { }
    virtual void OnAdd() = 0;
    virtual void OnDel() = 0;
    virtual void OnUpdate(UIObjectGLCanvas * canvas, float dt) = 0;

    bool IsActive() { return _state & kActive; }
	void SetActive(bool active) { ModifyState(kActive, active); }

    void ModifyState(StateEnum state, bool add)
    {
        if (add) _state |=  state;
        else     _state &= ~state;
    }

    SharePtr<GLObject> GetOwner() { return _owner->shared_from_this(); }
	void SetOwner(GLObject * owner) { _owner = owner; }

    //  �������
    virtual const std::string & GetName() = 0;
    //  Property�޸�ʱ������
    virtual bool OnModifyProperty(const std::any & value, 
                                  const std::any & backup,
                                  const std::string & title) = 0;
    std::vector<SharePtr<UIObject>> CreateUIPropertys();

    const std::vector<glm::vec2> & GetTrackPoints();
    void ModifyTrackPoint(const size_t index, const glm::vec2 & point);
    void InsertTrackPoint(const size_t index, const glm::vec2 & point);
    void DeleteTrackPoint(const size_t index, const glm::vec2 & point);

protected:
    virtual std::vector<Property> CollectProperty() = 0;
    virtual void OnModifyTrackPoint(const size_t index, const glm::vec2 & point) {};
    virtual void OnInsertTrackPoint(const size_t index, const glm::vec2 & point) {};
    virtual void OnDeleteTrackPoint(const size_t index, const glm::vec2 & point) {};

protected:
    size_t              _state;
    GLObject *          _owner;
    std::vector<glm::vec2> _trackPoints;
};