#pragma once

#include "../../include.h"
#include "gl_object.h"
#include "component.h"
#include "res.h"
#include "raw.h"

class Project {
public:
    Project();
    ~Project();

    //  ��Ŀ
    bool IsOpen() const { return mObject != nullptr; }
    const std::string & GetURL() const { return mURL; }
    const std::string & GetDIR() const { return tools::GetFileFolder(mURL); }
    
    void New(const std::string & url);
    void Load(const std::string & url);
    void Save(const std::string & url);

    //  ����
    bool IsExistObject(uint id) { return mObjects.count(id) != 0; }
    const SharePtr<GLObject> & GetObject()        { return mObject; }
    const SharePtr<GLObject> & GetObject(uint id) { return mObjects.at(id); }

    SharePtr<GLObject> NewObject();
    void DeleteObject(const uint & id);
    void DeleteObject(const SharePtr<GLObject> & object);

    //  ��Դ
    bool DeleteRes(Res * res);
    bool RenameRes(Res * res, const std::string & url);
    bool SetResType(Res * res, uint type);

    void Retrieve();    //  ����
    Res * GetRes(uint id);
    Res * GetResFromPGID(uint PGID);
    std::vector<Res *> GetResByType(const std::vector<Res::TypeEnum> & types);

private:
    std::string GetResURL(const std::string & url)
    { 
        return url + ".res";
    }

    Res * NewRes();

private:
    std::map<uint, Res *>               _resources;     //  ��Դ
    std::map<uint, SharePtr<GLObject>>  mObjects;       //  ����

    SharePtr<GLObject> mObject;
    std::string        mURL;       //  ��Ŀ·��
    uint               _gid;       //  ȫ��ID
};