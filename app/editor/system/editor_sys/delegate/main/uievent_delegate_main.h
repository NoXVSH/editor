#pragma once

#include "../../../../include.h"
#include "../../component/component.h"
#include "../../../event_sys/event_sys.h"
#include "../../../cfg_sys/cfg_sys.h"
#include "../../../ui_sys/ui_sys.h"
#include "../../../ui_sys/ui_menu.h"
#include "../../../ui_sys/ui_parse/ui_parse.h"
#include "../../../ui_sys/ui_state/ui_state.h"
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
    virtual bool OnCallEventMessage(UIEventEnum e, const UIEvent::Event & param, UIObject * object) override;

private:
    void OnEvent(EventSys::TypeEnum type, const std::any & param);
    void OnEventFreeProject();
};

//  ��Դ�б�
class UIEventDelegateMainResList : public UIEventDelegateMain {
public:
    virtual bool OnCallEventMessage(UIEventEnum e, const UIEvent::Event & param, UIObject * object) override;
};

//  ����б�
class UIEventDelegateMainComList : public UIEventDelegateMain {
public:
    virtual bool OnCallEventMessage(UIEventEnum e, const UIEvent::Event & param, UIObject * object) override;

private:
    void OnEvent(EventSys::TypeEnum type, const std::any & param);
    void OnEventSelectObject(UIObject * uiObject, GLObject * glObject, bool select);
};

//  ��̨
class UIEventDelegateMainStage : public UIEventDelegateMain {
public:
    virtual bool OnCallEventMessage(UIEventEnum e, const UIEvent::Event & param, UIObject * object) override;
};

//  ȫ��
class UIEventDelegateMainGlobal : public UIEventDelegateMain {
public:
    virtual bool OnCallEventMessage(UIEventEnum e, const UIEvent::Event & param, UIObject * object) override;
};