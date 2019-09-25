#include "uievent_delegate_main.h"

bool UIEventDelegateMainObjList::OnCallEventMessage(UIEventEnum e, const UIEvent::Event & param, UIObject * object)
{
    UIEventDelegateMain::OnCallEventMessage(e, param, object);

    if (UIEventEnum::kDelegate == e)
    {
        auto & arg = (const UIEvent::Delegate &)param;
        if (arg.mType == 0)
        {
            _listener.Add(EventSys::TypeEnum::kOpenProject, std::bind(
                &UIEventDelegateMainObjList::OnEvent, this,
                std::placeholders::_1,
                std::placeholders::_2));
            _listener.Add(EventSys::TypeEnum::kFreeProject, std::bind(
                &UIEventDelegateMainObjList::OnEvent, this,
                std::placeholders::_1,
                std::placeholders::_2));
            _listener.Add(EventSys::TypeEnum::kSelectObject, std::bind(
                &UIEventDelegateMainObjList::OnEvent, this,
                std::placeholders::_1,
                std::placeholders::_2));
        }
    }

    if (!Global::Ref().mEditorSys->IsOpenProject())
    {
        return false;
    }

    switch (e)
    {
    case UIEventEnum::kMouse:
        {
            auto & mouse = (const UIEvent::Mouse &)param;
            if (mouse.mKey == 1 && mouse.mAct == 3)
            {
                object->GetState()->Pointer = Global::Ref().mEditorSys->GetProject()->GetRoot();

                std::vector<std::string> buffer;
                if (mouse.mObject == object)
                {
                    buffer.push_back("Add Object");
                }
                else
                {
                    auto & name = mouse.mObject->GetState()->Name;
                    buffer.push_back(        "Add Object");
                    buffer.push_back(        "Del Object");
                    buffer.push_back(SFormat("Rename Object/{0}~", name));

                    std::transform(
                        std::begin(Global::Ref().mCfgSys->At("res/cfg/editor/component.json", "Order")),
                        std::end(Global::Ref().mCfgSys->At("res/cfg/editor/component.json", "Order")),
                        std::back_inserter(buffer), [](const auto & pair) { return "Add Component/" + pair.mValue->ToString(); });
                }
                UIMenu::PopMenu(mouse.mObject, buffer);
            }

            if ((mouse.mKey == 0 || mouse.mKey == 1) && 
                (mouse.mAct == 2 || mouse.mAct == 3) && 
                mouse.mObject != object)
            {
                Global::Ref().mEditorSys->OptSelectObject(mouse.mObject, true);
            }
        }
        break;
    case UIEventEnum::kMenu:
        {
            auto menu = (const UIEvent::Menu &)param;
            if (menu.mPath == "Add Object")
            {
                auto name = Global::Ref().mEditorSys->ObjectName(menu.mObject);

                auto newGLObject = new GLObject();
                newGLObject->SetName(name);

                auto raw = mmc::JsonValue::Hash();
                raw->Insert(mmc::JsonValue::List(), "__Children");
                raw->Insert(mmc::JsonValue::Hash(), "__Property");
                raw->Insert(mmc::JsonValue::FromValue("2"), "__Property", "Type");
                raw->Insert(mmc::JsonValue::FromValue("0"), "__Property", "Align");
                raw->Insert(mmc::JsonValue::FromValue(name), "__Property", "Name");
                auto newUIObject = UIParser::Parse(raw);
                newUIObject->GetState()->Pointer = newGLObject;

                Global::Ref().mEditorSys->OptInsertObject(newUIObject, menu.mObject);

            }
            else if (menu.mPath == "Del Object")
            {
                Global::Ref().mEditorSys->OptDeleteObject(menu.mObject);
            }
            else if (
                menu.mPath.at(0) == 'R' && menu.mPath.at(1) == 'e' && 
                menu.mPath.at(2) == 'n' && menu.mPath.at(3) == 'a' &&
                menu.mPath.at(4) == 'm' && menu.mPath.at(5) == 'e')
            {
                Global::Ref().mEditorSys->OptRenameObject(menu.mObject, menu.mEdit);
            }
            else
            {
                auto name = menu.mPath.substr(std::strlen("Add Component/"));
                Global::Ref().mEditorSys->OptAppendComponent(menu.mObject, Component::Create(name));
            }
            std::cout
                << "Menu Key: " << menu.mPath << ' '
                << "Menu Edit: " << menu.mEdit << std::endl;
        }
        break;
    }
    return true;
}

void UIEventDelegateMainObjList::OnEvent(EventSys::TypeEnum type, const std::any & param)
{
    switch (type)
    {
    case EventSys::TypeEnum::kOpenProject:
        OnEventOpenProject();
        break;
    case EventSys::TypeEnum::kFreeProject:
        OnEventFreeProject();
        break;
    case EventSys::TypeEnum::kSelectObject:
        auto & value = std::any_cast<const std::tuple<UIObject *, GLObject *, bool, bool> &>(param);
        OnEventSelectObject(std::get<0>(value), std::get<1>(value), std::get<2>(value), std::get<3>(value));
        break;
    }
}

void UIEventDelegateMainObjList::OnEventOpenProject()
{
    std::function<void (UIObject * root, const std::vector<GLObject *> & objects)> InitObjectTree;
    InitObjectTree = [&InitObjectTree](UIObject * root, const std::vector<GLObject *> & objects)
    {
        for (auto object : objects)
        {
            auto raw = mmc::JsonValue::Hash();
            raw->Insert(mmc::JsonValue::List(), "__Children");
            raw->Insert(mmc::JsonValue::Hash(), "__Property");
            raw->Insert(mmc::JsonValue::FromValue("2"), "__Property", "Type");
            raw->Insert(mmc::JsonValue::FromValue("0"), "__Property", "Align");
            raw->Insert(mmc::JsonValue::FromValue(object->GetName()), "__Property", "Name");
            auto newUIObject = UIParser::Parse(raw);
            root->AddObject(newUIObject);
            newUIObject->GetState()->Pointer = object;

            InitObjectTree(newUIObject, object->GetObjects());
        }
    };

    auto project = Global::Ref().mEditorSys->GetProject();
    InitObjectTree(GetOnwer(), project->GetRoot()->GetObjects());
}

void UIEventDelegateMainObjList::OnEventFreeProject()
{
    GetOnwer()->ClearObjects();
}

void UIEventDelegateMainObjList::OnEventSelectObject(UIObject * uiObject, GLObject * glObject, bool select, bool multi)
{
    uiObject->GetState()->IsSelect = select;
}

bool UIEventDelegateMainResList::OnCallEventMessage(UIEventEnum e, const UIEvent::Event & param, UIObject * object)
{
    UIEventDelegateMain::OnCallEventMessage(e, param, object);
    return true;
}

bool UIEventDelegateMainComList::OnCallEventMessage(UIEventEnum e, const UIEvent::Event & param, UIObject * object)
{
    UIEventDelegateMain::OnCallEventMessage(e, param, object);

    if (UIEventEnum::kDelegate == e)
    {
        auto & arg = (const UIEvent::Delegate &)param;
        if (arg.mType == 0)
        {
            _listener.Add(EventSys::TypeEnum::kSelectObject, std::bind(
                &UIEventDelegateMainComList::OnEvent, this, 
                std::placeholders::_1, 
                std::placeholders::_2));
            _listener.Add(EventSys::TypeEnum::kAppendComponent, std::bind(
                &UIEventDelegateMainComList::OnEvent, this,
                std::placeholders::_1,
                std::placeholders::_2));
            _listener.Add(EventSys::TypeEnum::kDeleteComponent, std::bind(
                &UIEventDelegateMainComList::OnEvent, this,
                std::placeholders::_1,
                std::placeholders::_2));
        }
    }
    return false;
}

void UIEventDelegateMainComList::OnEvent(EventSys::TypeEnum type, const std::any & param)
{
    switch (type)
    {
    case EventSys::TypeEnum::kSelectObject:
        {
            auto & value = std::any_cast<const std::tuple<UIObject *, GLObject *, bool, bool> &>(param);
            OnEventSelectObject(std::get<0>(value), std::get<1>(value), std::get<2>(value), std::get<3>(value));
        }
        break;
    case EventSys::TypeEnum::kAppendComponent:
        {
            auto & value = std::any_cast<const std::tuple<UIObject *, GLObject *, Component *> &>(param);
            OnEventAppendComponent(std::get<0>(value), std::get<1>(value), std::get<2>(value));
        }
        break;
    case EventSys::TypeEnum::kDeleteComponent:
        {
            auto & value = std::any_cast<const std::tuple<UIObject *, GLObject *, Component *> &>(param);
            OnEventDeleteComponent(std::get<0>(value), std::get<1>(value), std::get<2>(value));
        }
        break;
    }
}

void UIEventDelegateMainComList::OnEventSelectObject(UIObject * uiObject, GLObject * glObject, bool select, bool multi)
{
    GetOnwer()->ClearObjects();

    if (select)
    {
        for (auto component : glObject->GetComponents())
        {
            auto header = new UIComponentHeader(component->GetName());
            GetOnwer()->AddObject(header);

            for (auto property : Component::BuildUIPropertys(component))
            {
                header->AddObject(property);
            }
        }
    }
}

void UIEventDelegateMainComList::OnEventAppendComponent(UIObject * uiObject, GLObject * glObject, Component * component)
{
    auto header = new UIComponentHeader(component->GetName());
    GetOnwer()->AddObject(header);

    for (auto property : Component::BuildUIPropertys(component))
    {
        header->AddObject(property);
    }
}

void UIEventDelegateMainComList::OnEventDeleteComponent(UIObject * uiObject, GLObject * glObject, Component * component)
{
    auto & components = component->GetOwner()->GetComponents();
    auto it = std::find(components.begin(), components.end(), component);
    GetOnwer()->DelObject(std::distance(components.begin(), it));
}

bool UIEventDelegateMainStage::OnCallEventMessage(UIEventEnum e, const UIEvent::Event & param, UIObject * object)
{
    UIEventDelegateMain::OnCallEventMessage(e, param, object);
 
    return true;
}

bool UIEventDelegateMainGlobal::OnCallEventMessage(UIEventEnum e, const UIEvent::Event & param, UIObject * object)
{
    UIEventDelegateMain::OnCallEventMessage(e, param, object);
    
    if (e == UIEventEnum::kMenu)
    {
        auto & menu = (const UIEvent::Menu &)param;
        if (menu.mPath == "Menu/New Project")
        {
            Global::Ref().mEditorSys->OptNewProject("1.proj");
        }
        else if (menu.mPath == "Menu/Open Project")
        {
            Global::Ref().mEditorSys->OptOpenProject("1.proj");
        }
        else if (menu.mPath == "Menu/Save Project")
        {
            Global::Ref().mEditorSys->OptSaveProject("");
        }
        else if (menu.mPath == "Menu/Free Project")
        {
            Global::Ref().mEditorSys->OptFreeProject();
        }
        return true;
    }
    return false;
}
