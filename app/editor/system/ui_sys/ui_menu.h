#pragma once

#include "base.h"

//  ���ֲ˵�
//  1. ���ڲ˵�
//  2. �����˵�

//  ÿ�����ڿ������Լ��Ĵ��ڲ˵�, �ڴ�����ÿ֡��Ⱦ
//  ͬһʱ��ֻ����һ�������˵�, ������UI֮����Ⱦ

//  �˵���ʽ:
//      @��ѡ, !����, ~�༭
//      Menu0/Menu0[~][@][!]
//      Menu0/Menu1[~][@][!]
//      Menu0/Menu2[~][@][!]
//      Menu1/Menu0[~][@][!]
//      Menu1/Menu1[~][@][!]
//      Menu1/Menu2[~][@][!]
//      Menu2/Menu0~

class UIMenu {
public:
    struct MenuItem {
        std::string           mPath;
        std::string           mName;
        std::string           mBuffer;
        bool                  mEditing;
        bool                  mSelected;
        bool                  mDisabled;
        std::vector<MenuItem> mChildren;

        static std::vector<MenuItem> Parse(const std::string & parent, const std::vector<std::string> & list);
    };

    struct Popup {
        glm::vec2 mMouse;
        UIObject * mObject;
        std::vector<MenuItem> mItems;
    };

public:
    static void BarMenu(UIObject * object, const std::vector<std::string> & list);
    static void PopMenu(UIObject * object, const std::vector<std::string> & list);
    static void RenderPopup();

private:
    static void RenderMenu(UIObject * object, std::vector<MenuItem> & items);
    static int OnResizeBuffer(ImGuiInputTextCallbackData * data);

private:
    static Popup s_popup;
};