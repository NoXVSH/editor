#pragma once

#include "res.h"

class ResSys {
public:
    //  ɾ����Դ
    void OptDeleteRes(uint id);
    void OptDeleteRes(Res * res);
    //  �ƶ���Դ
    void OptModifyRes(uint id, const std::string & url);
    void OptModifyRes(Res * res, const std::string & url);
    //  ������Դ����
    void OptSetResType(uint id, uint type);
    void OptSetResType(Res * res, uint type);

    //  ����
    void Retrieve();
    //  ����ָ��ID��Դ
    Res * GetRes(uint id);
    //  ����ָ��������Դ
    std::vector<Res *> GetResByType(Res::TypeEnum type);
    std::vector<Res *> GetResByType(std::initializer_list<Res::TypeEnum> types);
    //  ����ָ��ƥ����Դ
    std::vector<Res *> GetResByWord(const std::string & word);

private:
    void DeleteRes(Res * res);

private:
    std::map<uint, Res *> _resources;
};