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

class UIObject;
class UIState;
class UISys;

enum class DirectEnum {
    kU, kD, kL, kR, LENGTH
};

// ---
//  �ؼ��б�
// ---
enum class UITypeEnum {
    kOTHER,                 //  ����
    kLAYOUT,                //  ����
    kTREEBOX,               //  ���ο�
    kTEXTBOX,               //  �ı���
    kIMAGEBOX,              //  ͼƬ��
    kCOMBOBOX,              //  ������
    kUICONVAS,              //  UI����
    kGLCONVAS,              //  GL����
};

// ---
//  �ؼ�����
// ---
enum class UIAlignEnum {
    kDEFAULT,               //  ��������
    kCLING_T = 1 << 1,      //  ����
    kCLING_B = 1 << 2,      //  ����
    kCLING_L = 1 << 3,      //  ����
    kCLING_R = 1 << 4,      //  ����
    kCENTER_H = 1 << 5,     //  ˮƽ����
    kCENTER_V = 1 << 6,     //  ��ֱ����
    kSTRETCH_H = 1 << 7,    //  ˮƽ����
    kSTRETCH_V = 1 << 8,    //  ��ֱ����
    LENGTH,
};

// ---
//  �ı�����
// ---
enum class UITextAlignEnum {
    kCLING_T = 1 << 1,      //  ����
    kCLING_B = 1 << 2,      //  ����
    kCLING_L = 1 << 3,      //  ����
    kCLING_R = 1 << 4,      //  ����
    kCENTER_H = 1 << 5,     //  ˮƽ����
    kCENTER_V = 1 << 6,     //  ��ֱ����
};

// ---
//  �ؼ��¼�
// ---
enum class UIEventEnum {
    kKEY,               //  ����
    kMOUSE,             //  ���
    kEDIT_TEXT_FINISH,  //  �������
    kMENU,              //  �˵�
};

//  ת��ΪImGuiID
template <class T>
inline std::string ImID(const T & v)
{
    return "##" + std::to_string(v);
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
