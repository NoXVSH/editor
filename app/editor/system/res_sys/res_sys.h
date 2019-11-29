#pragma once

#include "res.h"

class ResSys {
public:
    //  ɾ����Դ
    bool OptDeleteRes();
    //  �ƶ���Դ
    bool OptModifyRes();
    //  ������Դ����
    bool OptSetResType(uint id, uint type);
    bool OptSetResType(Res * res, uint type);

    //  ����
    void Retrieve();
    //  ����ָ��ID��Դ
    const Res * GetRes(uint id);
    //  ����ָ��������Դ
    std::vector<const Res *> GetResByType(Res::TypeEnum tag);
    //  ����ָ��ƥ����Դ
    std::vector<const Res *> GetResByWord(const std::string & word);

private:
    std::map<uint, Res *> _resources;
};