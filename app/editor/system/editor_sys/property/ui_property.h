#pragma once

#include "../../../include.h"
#include "../../ui_sys/ui_object/ui_object.h"

class UIComponentHeader : public UIObject {
public:
    UIComponentHeader(const std::string & name): UIObject(UITypeEnum::kOther, new UIState())
    {
        GetState()->Name = name;
    }

private:
    virtual bool OnEnter() override;
};

template <class T>
class UIPropertyState : public UIState {
public:
    using Handler_t = std::function<bool(
        const std::any & oldValue,
        const std::any & newValue,
        const std::string & title)>;

public:
    UIPropertyState(
        T & value, 
        const std::string & title, 
        const Handler_t & handler)
        : mTitle(title)
        , mValue(value)
        , mOldValue(value)
        , mNewValue(value)
        , mHandler(handler)
    { }

    T           mOldValue;
    T           mNewValue;
    T &         mValue;
    std::string mTitle;
    Handler_t mHandler;
};

template <class T>
class UIPropertyObject : public UIObject {
public:
    using Handler_t = typename UIPropertyState<T>::Handler_t;

protected:
    UIPropertyObject(
        T & value,
        const std::string & title,
        const Handler_t & handler)
        : UIObject(UITypeEnum::kOther, new UIPropertyState<T>(value, title, handler))
    { }

    const std::string & GetTitle() { return GetState<UIPropertyState<T>>()->mTitle; }
    T & GetNewValue() { return GetState<UIPropertyState<T>>()->mNewValue; }
    T & GetOldValue() { return GetState<UIPropertyState<T>>()->mOldValue; }
    T & GetValue() { return GetState<UIPropertyState<T>>()->mValue; }

    bool Modify()
    {
        auto ret = GetState<UIPropertyState<T>>()->mHandler(GetOldValue(), GetNewValue(), GetTitle());
        if (ret) 
        {
            GetValue() = GetNewValue(); 
            GetOldValue() = GetValue();
        }
        else 
        {
            GetNewValue() = GetValue();
        }
        return ret;
    }

    virtual bool OnEnter() override
    {
        if (GetOldValue() != GetValue())
        {
            GetOldValue() = GetValue();
            GetNewValue() = GetValue();
        }

        auto width = ImGui::GetWindowWidth();
        ImGui::Columns(2, nullptr, false);
        ImGui::SetColumnWidth(1, width * 0.8f);
        ImGui::SetColumnOffset(1, width * 0.2f);

        ImGui::Text(GetTitle().c_str());
        ImGui::NextColumn();
        return true;
    }
};

// ---
//  ���� UIPropertyInt
// ---
class UIPropertyInt : public UIPropertyObject<int> {
public:
    UIPropertyInt(
        int & value, 
        const std::string & title, 
        const Handler_t & handler)
        : UIPropertyObject<int>(value, title, handler)
    { }

    virtual bool OnEnter() override;
};

// ---
//  ���� UIPropertyBool
// ---
class UIPropertyBool : public UIPropertyObject<bool> {
public:
    UIPropertyBool(
        bool & value,
        const std::string & title,
        const Handler_t & handler)
        : UIPropertyObject<bool>(value, title, handler)
    { }

    virtual bool OnEnter() override;
};

// ---
//  ���� UIPropertyFloat
// ---
class UIPropertyFloat : public UIPropertyObject<float> {
public:
    UIPropertyFloat(
        float & value,
        const std::string & title,
        const Handler_t & handler)
        : UIPropertyObject<float>(value, title, handler)
    { }

    virtual bool OnEnter() override;
};

// ---
//  ���� UIPropertyString
// ---
class UIPropertyString : public UIPropertyObject<std::string> {
public:
    UIPropertyString(
        std::string & value,
        const std::string & title,
        const Handler_t & handler)
        : UIPropertyObject<std::string>(value, title, handler)
    { }

    virtual bool OnEnter() override;
};

// ---
//  ���� UIPropertyCombo
// ---
class UIPropertyCombo : public UIPropertyObject<size_t> {
public:
    UIPropertyCombo(
        size_t & value,
        const std::string & title,
        const Handler_t & handler,
        const std::vector<std::string> & list)
        : UIPropertyObject<size_t>(value, title, handler), _list(list)
    { }

    virtual bool OnEnter() override;

private:
    const std::vector<std::string> & _list;
};

// ---
//  ���� UIPropertyVector2
// ---
class UIPropertyVector2 : public UIPropertyObject<glm::vec2> {
public:
    UIPropertyVector2(
        glm::vec2 & value,
        const std::string & title,
        const Handler_t & handler)
        : UIPropertyObject<glm::vec2>(value, title, handler)
    { }

    virtual bool OnEnter() override;
};

// ---
//  ���� UIPropertyVector3
// ---
class UIPropertyVector3 : public UIPropertyObject<glm::vec3> {
public:
    UIPropertyVector3(
        glm::vec3 & value,
        const std::string & title,
        const Handler_t & handler)
        : UIPropertyObject<glm::vec3>(value, title, handler)
    { }

    virtual bool OnEnter() override;
};

// ---
//  ���� UIPropertyVector4
// ---
class UIPropertyVector4 : public UIPropertyObject<glm::vec4> {
public:
    UIPropertyVector4(
        glm::vec4 & value,
        const std::string & title,
        const Handler_t & handler)
        : UIPropertyObject<glm::vec4>(value, title, handler)
    { }

    virtual bool OnEnter() override;
};

// ---
//  ���� UIPropertyColor4
// ---
class UIPropertyColor4 : public UIPropertyObject<glm::vec4> {
public:
    UIPropertyColor4(
        glm::vec4 & value,
        const std::string & title,
        const Handler_t & handler)
        : UIPropertyObject<glm::vec4>(value, title, handler)
    { }

    virtual bool OnEnter() override;
};

// ---
//  ���� UIPropertyAsset
// ---
class UIPropertyAsset: public UIPropertyObject<std::string> {
public:
    UIPropertyAsset(
        std::string & value,
        const std::string & title,
        const Handler_t & handler)
        : UIPropertyObject<std::string>(value, title, handler)
    { }

    virtual bool OnEnter() override;
};