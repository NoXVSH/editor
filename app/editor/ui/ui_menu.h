#pragma once

#include "base.h"

//  ���ֲ˵�
//  1. ���ڲ˵�
//  2. �����˵�

//  ÿ�����ڿ������Լ��Ĵ��ڲ˵�, �ڴ�����ÿ֡��Ⱦ
//  ͬһʱ��ֻ����һ�������˵�, ������UI֮����Ⱦ

class UIMenu {
public:
    struct MenuItem {
        std::string                 mPath;
        std::string                 mName;
        bool                        mSelected;
        bool                        mDisabled;
        std::vector<std::string>    mChildren;

        static std::vector<MenuItem> Parse(const std::string & parent, const std::vector<std::string> & list);
    };

public:
    static void BarMenu(UIClass * parent, const std::vector<std::string> & list);
    static void PopMenu(UIClass * parent, const std::vector<std::string> & list);
    static void RenderPopup();

private:
    static void RenderMenu(UIClass * parent, const std::vector<MenuItem> & items);

private:

};