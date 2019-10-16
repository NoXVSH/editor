ok 窗口菜单
ok 弹出菜单
ok 拉伸方向判断


对象
组件


图层
对象:
    组件0
    组件1
    组件2

//  工程类
class Project {
public:
    GLOBject * mRoot;

public:
    const std::string & GetFolderPath() const;
    const std::string & GetFilePath() const;
    bool Load(const std::string & url);
    bool Save(const std::string & url);
    void Free();
}

//  提供二进制序列化/反序列化
//  提供字符串序列化/反序列化
//  序列化接口
class Serializer {
public:
    virtual void Encode(std::ofstream & os) = 0;
    virtual void Decode(std::ifstream & is) = 0;
}

ok  //  event_enum 放进 EventSys
ok  //  event_listen 放进 EventSys
ok  //  parser_tool.h 放进 Serializer
ok  //  UIEventEnum 剥离


需要序列化的对象:
    Project
    GLObject
    Component


拖放参数:
    bool IsCanDrag,

    struct Drag {
        glm::vec2 mBegWorld;
        glm::vec2 mEndWorld;
    }

    struct Drag {
        UIObject * mObject;
        glm::vec2 mBegWorld;
        glm::vec2 mEndWorld;
    }

使用智能指针

为GLObject生成唯一ID
EditorSys只操作GLObject

精灵
瓦片地图
光照
阴影

//  GLCanvas
//      双击 -> 进入编辑
//      Esc -> 退出编辑

//  编辑模式:
//      绘制边框, 显示控制点
//      左键双击边框 -> 新增控制点
//      左键单击控制点 -> 选中控制点
//      左键按住控制点 -> 拖动控制点
//      右键按住控制点 -> 新增控制点
//      Delete        -> 删除控制点

//  拖动模式:
//      绘制边框
//      左键按住边框内 -> 拖动对象
//      Delete        -> 删除对象

资源管理器
资源选择框