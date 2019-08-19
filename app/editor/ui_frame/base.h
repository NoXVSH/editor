#pragma once

#include "../include.h"
#include "imgui.h"

class UIState;
class UIClass;
class UIFrame;

enum class DirectEnum {
    kT, kD, kL, kR, LENGTH
};

enum class UITypeEnum {
    kTREE,              //  �������� ���϶�(n) ������(n)
    kWINDOW,            //  �������� ���϶�(y) ������(y)
    kDIALOG,            //  �������� ���϶�(n) ������(n)
    kCONTAINER,         //  ��Ƕ���� ���϶�(n) ������(y)
};

static const std::string WEIGET_CONTAINER      = "DDContainer";
static const std::string WEIGET_BUTTON         = "Button";
