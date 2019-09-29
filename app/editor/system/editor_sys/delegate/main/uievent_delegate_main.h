#pragma once

#include "../../../../include.h"
#include "../../component/component.h"
#include "../../../event_sys/event_sys.h"
#include "../../../cfg_sys/cfg_sys.h"
#include "../../../ui_sys/ui_sys.h"
#include "../../../ui_sys/ui_menu.h"
#include "../../../ui_sys/ui_parse/ui_parse.h"
#include "../../../ui_sys/ui_object/ui_object.h"
#include "../../property/ui_property.h"
#include "../../editor_sys.h"
#include "../../project.h"

class UIEventDelegateMain : public UIEvent::DelegateHandler {
protected:
    EventSys::Listener _listener;
};

//  �����б�
class UIEventDelegateMainObjList : public UIEventDelegateMain {
public:
    virtual bool OnCallEventMessage(UIEventEnum e, const UIEvent::Event & param, const SharePtr<UIObject> & object) override;

private:
    //  �ر���Ŀ
    //  ����Ŀ
    void OnEvent(EventSys::TypeEnum type, const std::any & param);
    void OnEventOpenProject();
    void OnEventFreeProject();
    void OnEventSelectObject(const SharePtr<UIObject> & uiObject, const SharePtr<GLObject> & glObject, bool select, bool multi);
};

//  ��Դ�б�
class UIEventDelegateMainResList : public UIEventDelegateMain {
public:
    virtual bool OnCallEventMessage(UIEventEnum e, const UIEvent::Event & param, const SharePtr<UIObject> & object) override;
};

//  ����б�
class UIEventDelegateMainComList : public UIEventDelegateMain {
public:
    virtual bool OnCallEventMessage(UIEventEnum e, const UIEvent::Event & param, const SharePtr<UIObject> & object) override;

private:
    //  ѡ�����
    //  �������
    void OnEvent(EventSys::TypeEnum type, const std::any & param);
    void OnEventSelectObject(const SharePtr<UIObject> & uiObject, const SharePtr<GLObject> & glObject, bool select, bool multi);
    void OnEventAppendComponent(const SharePtr<UIObject> & uiObject, const SharePtr<GLObject> & glObject, const SharePtr<Component> & component);
    void OnEventDeleteComponent(const SharePtr<UIObject> & uiObject, const SharePtr<GLObject> & glObject, const SharePtr<Component> & component);
};

//  ��̨
class UIEventDelegateMainStage : public UIEventDelegateMain {
public:
    virtual bool OnCallEventMessage(UIEventEnum e, const UIEvent::Event & param, const SharePtr<UIObject> & object) override;
};

//  ȫ��
class UIEventDelegateMainGlobal : public UIEventDelegateMain {
public:
    virtual bool OnCallEventMessage(UIEventEnum e, const UIEvent::Event & param, const SharePtr<UIObject> & object) override;
};