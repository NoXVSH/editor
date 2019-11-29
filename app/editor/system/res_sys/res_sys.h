#pragma once

#include "res.h"

class ResSys {
public:
    //  ɾ����Դ
    bool OptDeleteRes(uint res);
    bool OptDeleteRes(Res * res);
    //  �ƶ���Դ
    bool OptModifyRes(uint res, const std::string & url);
    bool OptModifyRes(Res * res, const std::string & url);
    //  ������Դ����
    bool OptSetResType(uint res, uint type);
    bool OptSetResType(Res * res, uint type);

    //  ����
    void Retrieve();
    //  ����ָ��ID��Դ
    Res * GetRes(uint id);
    //  ����ָ��������Դ
    std::vector<Res *> GetResByType(Res::TypeEnum type);
    //  ����ָ��ƥ����Դ
    std::vector<Res *> GetResByWord(const std::string & word);

private:
    std::map<uint, Res *> _resources;
};