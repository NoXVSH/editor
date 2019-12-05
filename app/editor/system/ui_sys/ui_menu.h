#pragma once

#include "include.h"

//  ���ֲ˵�
//  1. ���ڲ˵�
//  2. �����˵�

//  ÿ�����ڿ������Լ��Ĵ��ڲ˵�, �ڴ�����ÿ֡��Ⱦ
//  ͬһʱ��ֻ����һ�������˵�, ������UI֮����Ⱦ

//  �˵���ʽ:
//      @��ѡ, !����, ~�༭
//      Menu0\\Menu0[~][@][!]
//      Menu0\\Menu1[~][@][!]
//      Menu0\\Menu2[~][@][!]
//      Menu1\\Menu0[~][@][!]
//      Menu1\\Menu1[~][@][!]
//      Menu1\\Menu2[~][@][!]
//      Menu2\\Menu0~

class UIMenu {
public:
    struct MenuItem {
        std::string           mPath;        //  �˵���·��
        std::string           mName;        //  �˵�������
        std::string           mBuffer;      //  �˵����
        bool                  mEditing;     //  �˵����Ƿ�ɱ༭
        bool                  mSelected;    //  �˵����Ƿ���ѡ��
        bool                  mDisabled;    //  �˵����Ƿ��ѽ���
        std::vector<MenuItem> mChildren;    //  �˵�������

        static std::vector<MenuItem> Parse(const std::string & parent, const std::vector<std::string> & list);
    };

    struct Popup {
        glm::vec2 mMouse;
        SharePtr<UIObject> mObject;
        std::vector<MenuItem> mItems;
    };

public:
    static void BarMenu(SharePtr<UIObject> object, const std::vector<std::string> & list);
    static void PopMenu(SharePtr<UIObject> object, const std::vector<std::string> & list);
    static void RenderPopup();

private:
    static void RenderMenu(SharePtr<UIObject> object, std::vector<MenuItem> & items);

private:
    static Popup s_popup;
};