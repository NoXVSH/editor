ok ���ڲ˵�
ok �����˵�
ok ���췽���ж�


����
���


ͼ��
����:
    ���0
    ���1
    ���2

//  ������
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

//  �ṩ���������л�/�����л�
//  �ṩ�ַ������л�/�����л�
//  ���л��ӿ�
class Serializer {
public:
    virtual void Encode(std::ofstream & os) = 0;
    virtual void Decode(std::ifstream & is) = 0;
}

ok  //  event_enum �Ž� EventSys
ok  //  event_listen �Ž� EventSys
ok  //  parser_tool.h �Ž� Serializer
ok  //  UIEventEnum ����


// ��Ҫ���л��Ķ���:
//     Project
//     GLObject
//     Component


�ϷŲ���:
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

// ʹ������ָ��

// ΪGLObject����ΨһID
// EditorSysֻ����GLObject

//  ͼ��/ͼƬ�ϲ�
//  ��Դ�������WeakPtr

 GLCanvas
    �л�ģʽ: �س�

�༭ģʽ:
    ���Ʊ߿�, ��ʾ���Ƶ�
    ���˫���߿� -> �������Ƶ�
    ����������Ƶ� -> ѡ�п��Ƶ�
    �����ס���Ƶ� -> �϶����Ƶ�
    �Ҽ���ס���Ƶ� -> �������Ƶ�
    Delete        -> ɾ�����Ƶ�

 �϶�ģʽ:
     ���Ʊ߿�
     �����ס�߿��� -> �϶�����
     Delete        -> ɾ������

// ����
// ��Ƭ��ͼ
// ����
// ����������
// �������ü�
// �ֲ���Ⱦ
// ��������
// ����ɼ�
// �����Ϸ�
// ����

��Դ������:
    ���,
    Ŀ¼,
    Ҷ��,
UISys.Update()
UISys.OpenWindow()
UISys.FreeWindow()
UISys.IsWindowExist()
_windows

��Դ����:
    Text
    Image
    Object
    Tilemap
    Variable
    BehaviorTree

�����ƻ�
��������
��Ϊ��
����
����
����
