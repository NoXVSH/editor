#pragma once

#include "../../../../include.h"
#include "../../../cfg_sys/cfg_sys.h"
#include "../../../ui_sys/ui_sys.h"
#include "../../../ui_sys/ui_menu.h"
#include "../../../ui_sys/ui_parse/ui_parse.h"
#include "../../../ui_sys/ui_state/ui_state.h"
#include "../../../ui_sys/ui_object/ui_object.h"
#include "../../editor_sys.h"

class UIEventDelegateMainObjList : public UIObject::UIEventDelegate {
public:
    virtual bool OnCallEventMessage(UIObject * object, UIEventEnum e, const UIObject::UIEventDetails::Base & param) override;
};

class UIEventDelegateMainResList : public UIObject::UIEventDelegate {
public:
    virtual bool OnCallEventMessage(UIObject * object, UIEventEnum e, const UIObject::UIEventDetails::Base & param) override;
};

class UIEventDelegateMainComList : public UIObject::UIEventDelegate {
public:
    virtual bool OnCallEventMessage(UIObject * object, UIEventEnum e, const UIObject::UIEventDetails::Base & param) override;
};

class UIEventDelegateMainStage : public UIObject::UIEventDelegate {
public:
    virtual bool OnCallEventMessage(UIObject * object, UIEventEnum e, const UIObject::UIEventDetails::Base & param) override;
};