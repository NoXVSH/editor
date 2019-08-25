#pragma once

#include "../include.h"
#include "imgui.h"

class UIState;
class UIClass;
class UIFrame;

enum class DirectEnum {
    kT, kD, kL, kR, LENGTH
};

//  �ؼ��б�
enum class UITypeEnum {
    kTREE,                  //  ����
    kIMAGE,                 //  ͼƬ
    kBUTTON,                //  ��ť
    kLAYOUT,                //  ����
    kWINDOW,                //  ����
    kEDITBOX,               //  �༭��
    kTEXTBOX,               //  �ı���
    kCOMBOBOX,              //  ������
    kUICONVAS,              //  UI����
    kGLCONVAS,              //  GL����
};

//  �ؼ��б�
static const std::string WEIGET_TREE        = "Tree";
static const std::string WEIGET_IMAGE       = "Image";
static const std::string WEIGET_BUTTON      = "Button";
static const std::string WEIGET_LAYOUT      = "LAYOUT";
static const std::string WEIGET_WINDOW      = "Window";
static const std::string WEIGET_EDITBOX     = "EditBox";
static const std::string WEIGET_TEXTBOX     = "TextBox";
static const std::string WEIGET_ComboBox    = "ComboBox";
static const std::string WEIGET_UICANVAS    = "UICanvas";
static const std::string WEIGET_GLCANVAS    = "GLCanvas";

//  �Զ������� Get/Set
using UserData = std::map<std::string, std::any>;

template <class T>
const T & __GetUserData(const UserData & data, const std::string & key)
{
    return std::any_cast<const T &>(data.at(key));
}

#define __REG_GET_USER_DATA_FUNC(type, key)                         \
    inline const type & __GetUserData##key(const UserData & data)   \
    {                                                               \
        return __GetUserData<type>(data, #key);                     \
    }

__REG_GET_USER_DATA_FUNC(bool, __WindowIsNav)                   //  �е����� 
__REG_GET_USER_DATA_FUNC(bool, __WindowIsSize)                  //  ��������
__REG_GET_USER_DATA_FUNC(bool, __WindowIsMove)                  //  �����ƶ�
__REG_GET_USER_DATA_FUNC(bool, __WindowIsTitleBar)              //  �б�����
__REG_GET_USER_DATA_FUNC(bool, __WindowIsCollapse)              //  ��������
__REG_GET_USER_DATA_FUNC(bool, __WindowIsScrollBar)             //  �й�����
__REG_GET_USER_DATA_FUNC(bool, __WindowIsFullScreen)            //  ����ȫ��

__REG_GET_USER_DATA_FUNC(bool, __LayoutIsShowBorder)            //  ��ʾ�߿�
__REG_GET_USER_DATA_FUNC(float, __LayoutBorderNumber)           //  �߿���

#define __GET_USER_DATA(data, key)  __GetUserData##key(data)

//  �ַ���ת��������
inline float S2F(const std::string & s)
{
    return std::stof(s);
}

inline bool S2B(const std::string & s)
{
    return s == "ok";
}

inline glm::vec2 S2V2(const std::string & s)
{
    auto arr = string_tool::Split(s, " ");
    ASSERT_LOG(arr.size() == 2, "Arr Length: {0}!", arr.size());
    return glm::vec2(S2F(arr.at(0)), S2F(arr.at(1)));
}

inline glm::vec3 S2V3(const std::string & s)
{
    auto arr = string_tool::Split(s, " ");
    ASSERT_LOG(arr.size() == 3, "Arr Length: {0}!", arr.size());
    return glm::vec3(S2F(arr.at(0)), S2F(arr.at(1)), S2F(arr.at(2)));
}

inline glm::vec3 S2V3(const std::string & s)
{
    auto arr = string_tool::Split(s, " ");
    ASSERT_LOG(arr.size() == 4, "Arr Length: {0}!", arr.size());
    return glm::vec4(S2F(arr.at(0)), S2F(arr.at(1)), S2F(arr.at(2)), S2F(arr.at(3)));
}