#include "ui_menu.h"
#include "ui_class/ui_class.h"

UIMenu::Popup UIMenu::s_popup;

//  _TODO
//      һ��ȫ������
std::vector<UIMenu::MenuItem> UIMenu::MenuItem::Parse(const std::string & parent, const std::vector<std::string> & list)
{
    std::vector<MenuItem> result;
    for (const auto & str : list)
    {
        auto disabled = false;
        auto selected = false;
        auto editing = false;
        auto leaf     = false;

        //  �Ƿ�Ҷ�ӽڵ�, �Ƿ��ѹ�ѡ, �Ƿ��ѽ���
        auto pos = str.find_first_of('/');
        if (pos == std::string::npos)
        {
            leaf = true;
            pos = str.find_first_of('@');
            if (pos != std::string::npos) { selected = true; }
            pos = str.find_first_of('!');
            if (pos != std::string::npos) { disabled = true; }
            pos = str.find_first_of('~');
            if (pos != std::string::npos) { editing = true; }

            if (pos != std::string::npos && str.at(pos) == '!') { --pos; }
            if (pos != std::string::npos && str.at(pos) == '@') { --pos; }
            if (pos != std::string::npos && str.at(pos) == '~') { --pos; }
            if (pos != std::string::npos) { ++pos; }
        }

        auto name = str.substr(0, pos);

        //  �Ƿ�ͬһ�����ڵ�
        if (result.empty() || result.back().mName != name)
        {
            result.emplace_back();
        }

        auto & item  = result.back();
        item.mName = std::move(name);
        item.mEditing = editing;
        item.mSelected = selected;
        item.mDisabled = disabled;
        item.mPath = !parent.empty()?parent +'/'+ item.mName:item.mName;
        if (!leaf) { item.mChildren.emplace_back(str.substr(pos + 1)); }
    }
    return std::move(result);
}

void UIMenu::BarMenu(UIClass * object, const std::vector<std::string> & list)
{
    ImGui::BeginMenuBar();
    RenderMenu(object, MenuItem::Parse("", list));
    ImGui::EndMenuBar();
}

void UIMenu::PopMenu(UIClass * object, const std::vector<std::string>& list)
{
    s_popup.mObject = object;
    s_popup.mMouse.x = ImGui::GetMousePos().x;
    s_popup.mMouse.y = ImGui::GetMousePos().y;
    s_popup.mItems = std::move(MenuItem::Parse("", list));
    ImGui::OpenPopup(s_popup.mItems.at(0).mName.c_str());
}

void UIMenu::RenderPopup()
{
    if (s_popup.mObject != nullptr)
    {
        ImGui::SetCursorScreenPos(ImVec2(s_popup.mMouse.x,s_popup.mMouse.y));
        if (ImGui::BeginPopup(s_popup.mItems.at(0).mName.c_str()))
        {
            RenderMenu(s_popup.mObject, s_popup.mItems);
            ImGui::EndPopup();
        }
        else
        {
            s_popup.mObject = nullptr;
        }
    }
}

void UIMenu::RenderMenu(UIClass * object, const std::vector<MenuItem> & items)
{
    for (auto & item : items)
    {
        if (item.mChildren.empty())
        {
            if (item.mEditing)
            {
                std::string buffer = item.mName;
                if (ImGui::InputText(("##" + item.mName).c_str(), buffer.data(), buffer.size(), 
                    ImGuiInputTextFlags_EnterReturnsTrue | 
                    ImGuiInputTextFlags_CallbackResize | 
                    ImGuiInputTextFlags_AutoSelectAll,
                    &UIMenu::OnResizeBuffer, &buffer))
                {
                    ImGui::CloseCurrentPopup();
                    object->PostEventMessage(UIEventEnum::kMENU, UIClass::UIEventDetails::Menu(item.mPath, buffer.c_str()));
                }
            }
            else
            {
                if (ImGui::MenuItem(item.mName.c_str(), nullptr, item.mSelected, !item.mDisabled))
                {
                    object->PostEventMessage(UIEventEnum::kMENU, UIClass::UIEventDetails::Menu(item.mPath, item.mSelected));
                }
            }
        }
        else
        {
            if (ImGui::BeginMenu(item.mName.c_str(), !item.mDisabled))
            {
                RenderMenu(object, MenuItem::Parse(item.mPath, item.mChildren));
                ImGui::EndMenu();
            }
        }
    }
}

int UIMenu::OnResizeBuffer(ImGuiInputTextCallbackData * data)
{
    auto value = (std::string *)data->UserData;
    value->resize(data->BufTextLen);
    data->Buf = value->data();
    return value->capacity();
}
