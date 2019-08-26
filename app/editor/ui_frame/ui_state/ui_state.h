#pragma once

#include "../base.h"

class UIState {
public:
    UIState();

    virtual ~UIState()
    { }

    std::map<std::string, std::any> mData;
};

class UIStateWindow : public UIState {
public:
    UIStateWindow();
};

class UIStateLayout : public UIState {
public:
    struct LayoutInfo {
        struct LinkEdge {
            UIClass  * mLayout;
            DirectEnum mDirect;
            LinkEdge(DirectEnum direct = DirectEnum::kU,
                     UIClass * object   = nullptr)
                : mDirect(direct), mLayout(object)
            { }
        };
        std::vector<UIClass*> mLinks[(size_t)DirectEnum::LENGTH]; //  ������Layout
        std::vector<LinkEdge> mEdges[(size_t)DirectEnum::LENGTH]; //  ������Edge
    } mLayoutInfo;

    UIStateLayout();
};