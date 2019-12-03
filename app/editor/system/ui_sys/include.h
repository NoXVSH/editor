#pragma once

#include "../../include.h"

//  ��չImVec
#ifndef IM_VEC2_CLASS_EXTRA
#define IM_VEC2_CLASS_EXTRA                                 \
    ImVec2(const glm::vec2 & val): x(val.x), y(val.y)       \
    { }                                                     \
    ImVec2 & operator=(const glm::vec2 & val)               \
    {                                                       \
        x = val.x;                                          \
        y = val.y;                                          \
        return *this;                                       \
    }                                                       \
    operator glm::vec2() const                              \
    {                                                       \
        return glm::vec2(x, y);                             \
    }                                                       
#endif

#ifndef IM_VEC4_CLASS_EXTRA
#define IM_VEC4_CLASS_EXTRA                                 \
    ImVec4(const glm::vec3 & val)                           \
        : x(val.x), y(val.y)                                \
        , z(val.z), w(0)                                    \
    { }                                                     \
    ImVec4(const glm::vec4 & val)                           \
        : x(val.x), y(val.y)                                \
        , z(val.z), w(val.w)                                \
    { }                                                     \
    ImVec4 & operator=(const glm::vec3 & val)               \
    {                                                       \
        x = val.x;                                          \
        y = val.y;                                          \
        z = val.z;                                          \
        return *this;                                       \
    }                                                       \
    ImVec4 & operator=(const glm::vec4 & val)               \
    {                                                       \
        x = val.x;                                          \
        y = val.y;                                          \
        z = val.z;                                          \
        w = val.w;                                          \
        return *this;                                       \
    }                                                       \
    operator glm::vec3() const                              \
    {                                                       \
        return glm::vec3(x, y, z);                          \
    }                                                       \
    operator glm::vec4() const                              \
    {                                                       \
        return glm::vec4(x, y, z, w);                       \
    }                                                       
#endif

#include "imgui.h"

class UIState;
class UIParser;
class UIObject;

enum class DirectEnum {
    kU, kD, kL, kR, Length, kNone,
};

// ---
//  �ؼ��б�
// ---
enum class UITypeEnum {
    kOther,                 //  ����
    kLayout,                //  ����
    kTreeBox,               //  ���ο�
    kTextBox,               //  �ı���
    kImageBox,              //  ͼƬ��
    kComboBox,              //  ������
    kUICanvas,              //  UI����
    kGLCanvas,              //  GL����
};

// ---
//  �ؼ�����
// ---
enum class UIAlignEnum {
    kDefault,              //  ��������
    kClingT = 1 << 1,      //  ����
    kClingB = 1 << 2,      //  ����
    kClingL = 1 << 3,      //  ����
    kClingR = 1 << 4,      //  ����
    kCenterH = 1 << 5,     //  ˮƽ����
    kCenterV = 1 << 6,     //  ��ֱ����
    kStretchH = 1 << 7,    //  ˮƽ����
    kStretchV = 1 << 8,    //  ��ֱ����
    Length,
};

// ---
//  �ı�����
// ---
enum class UITextAlignEnum {
    kClingT = 1 << 1,      //  ����
    kClingB = 1 << 2,      //  ����
    kClingL = 1 << 3,      //  ����
    kClingR = 1 << 4,      //  ����
    kCenterH = 1 << 5,     //  ˮƽ����
    kCenterV = 1 << 6,     //  ��ֱ����
};

// ---
//  �ؼ��¼�
// ---
enum class UIEventEnum {
    kKey,               //  ����
    kMouse,             //  ���
    kEdit,              //  �༭
    kMenu,              //  �˵�
    kDrag,              //  �Ϸ�
    kInit,              //  ����
};

//  ת��ΪImGuiID
template <class T>
std::string ImID(const T & v)
{
    if constexpr (std::is_pointer_v<T> || std::is_arithmetic_v<T>)
    {
        return SFormat("##{0}", (size_t)v);
    }
    else
    {
        return SFormat("{0}##{1}", v, (size_t)&v);
    }
}

//  ImGui ���ߺ���
namespace imgui_tools {
    static int OnResizeBuffer(ImGuiInputTextCallbackData * data)
    {
        auto value = (std::string *)data->UserData;
        value->resize(data->BufTextLen);
        data->Buf = value->data();
        return value->capacity();
    }
}
