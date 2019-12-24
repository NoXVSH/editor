#include "project.h"
#include "raw_sys.h"

Project::Project(): mGID(0)
{ }

Project::~Project()
{
    //  ɾ����Դ, ��Ϊ��Դ����ҪSharePtr, �ʴ�������ɾ��
    for (auto & pair : _resources)
    {
        delete pair.second;
    }
}

void Project::New(const std::string & url)
{
    mGID = 0x0;
    mURL = url;
    mObject = NewObject();
}

void Project::Load(const std::string & url)
{
    ASSERT_LOG(mObject == nullptr, url.c_str());
    ASSERT_LOG(_resources.empty(), url.c_str());
    std::ifstream is;

    //  ������Դ
    is.open(GetResURL(url), std::ios::binary);
    ASSERT_LOG(is, url.c_str());
    tools::Deserialize(is,mGID);

    //  ����Res����
    uint num = 0; tools::Deserialize(is, num);
    for (auto i = 0; i != num; ++i)
    {
        auto res    = new Res(this);
        res->DecodeBinary(is, this);
        auto pair = std::make_pair(res->GetID(), res);
        _resources.insert(pair);
    }
    is.close();

    //  ���ض���
    is.open(url, std::ios::binary);
    ASSERT_LOG(is,    url.c_str());
    mObject.reset(new  GLObject());
    mObject->DecodeBinary(is,this);
    is.close();
    mURL = url;

    //  ����Object Map
    std::deque<SharePtr<GLObject>> list{ mObject };
    while (!list.empty())
    {
        auto & front = list.front();
        std::copy(
            front->GetObjects().begin(),
            front->GetObjects().end(),
            std::back_inserter(list));
        mObjects.insert(std::make_pair(front->GetID(), front));
        list.pop_front();
    }
    Retrieve();
}

void Project::Save(const std::string & url)
{
    ASSERT_LOG(mObject != nullptr, url.c_str());
    const auto & saveURL = url.empty()?mURL:url;

    std::ofstream os;
    //  д����Դ
    os.open(GetResURL(saveURL), std::ios::binary);
    tools::Serialize(os, mGID);
    uint num = _resources.size();
    tools::Serialize(os,  num);
    for (auto & pair : _resources)
    {
        pair.second->EncodeBinary(os, this);
    }
    os.close();

    //  д��Obj����
    os.open(saveURL, std::ios::binary);
    mObject->EncodeBinary(os, this);
    os.close();
}

SharePtr<GLObject> Project::NewObject()
{
    auto object = std::create_ptr<GLObject>(++mGID);
    mObjects.insert(std::make_pair(mGID, object));
    return object;
}

void Project::DeleteObject(const uint & id)
{
    mObjects.erase(id);
}

void Project::DeleteObject(const SharePtr<GLObject> & object)
{
    DeleteObject(object->GetID());
}

bool Project::DeleteRes(Res * res)
{ 
    if (res->Type() == Res::kNull ||
        res->Type() == Res::kJson ||
        res->Type() == Res::kImg ||
        res->Type() == Res::kMap ||
        res->Type() == Res::kFont)
    {
        std::filesystem::remove(res->Path());
        _resources.erase(res->GetID());
        res->WakeRefs();
        delete  res;
        return true;
    }
    return false;
}

bool Project::RenameRes(Res * res, const std::string & url)
{
    if (res->Type() == Res::kNull ||
        res->Type() == Res::kJson ||
        res->Type() == Res::kImg ||
        res->Type() == Res::kMap ||
        res->Type() == Res::kFont)
    {
        try
        {
            std::filesystem::rename(res->Path(), url);
            res->Meta(url);
            return true;
        }
        catch (const std::exception &)
        {
            //  Ų���ļ�λ�÷����쳣, ����ʧ��, ���Ա��β���
        }
    }
    return false;
}

bool Project::SetResType(Res * res, uint type)
{
    auto ret = res->GetRefCount() == 0 
            && type != Res::kObj 
            && type != Res::kVar 
            && type != Res::kBlueprint 
            && (res->Type() == Res::kNull ||
                res->Type() == Res::kJson ||
                res->Type() == Res::kImg ||
                res->Type() == Res::kMap ||
                res->Type() == Res::kFont);
    if (ret) { res->Type((Res::TypeEnum)type); }
    return ret;
}

void Project::Retrieve()
{
    std::set<std::string>   set0;     //  �ļ�
    std::set<uint>          set1;     //  id

    for (auto it = _resources.begin(); it != _resources.end();)
    {
        auto tmp = it;
        switch (it->second->Type())
        {
        case Res::kNull:
        case Res::kJson:
        case Res::kImg:
        case Res::kMap:
        case Res::kFont:
            if (tools::IsFileExists(it->second->Meta<std::string>()))
            {
                set0.insert(it->second->Meta<std::string>()); ++it;
            }
            break;
        case Res::kObj:
            if (IsExistObject(it->second->Meta<uint>()))
            {
                set1.insert(it->second->Meta<uint>()); ++it;
            }
            break;
        }
        if (tmp == it)
        {
            delete it->second; it = _resources.erase(it); 
        }
    }

    //  ���������ļ�
    tools::ListPath("res", [&] (const std::string & path)
        {
            if (0 == set0.count(path))
            {
                auto res = NewRes();
                res->Meta(path);
                auto & type = Global::Ref().mRawSys->QueryType(path);
                if      (type == typeid(RawTexture))
                {
                    res->Type(Res::kImg);
                }
                else if (type == typeid(RawMap))
                {
                    res->Type(Res::kMap);
                }
                else if (type == typeid(RawFont))
                {
                    res->Type(Res::kFont);
                }
                else if (type == typeid(mmc::Json))
                {
                    res->Type((Res::kJson));
                }
                else
                {
                    res->Type(Res::kNull);
                }
            }
        });

    //  ����
    std::deque<SharePtr<GLObject>> list{
        mObject->GetObjects().begin(),
        mObject->GetObjects().end()
    };
    while (!list.empty())
    {
        auto & front = list.front();
        std::copy(
            front->GetObjects().begin(),
            front->GetObjects().end(),
            std::back_inserter(list));
        auto objectid =front->GetID();
        if (0 == set1.count(objectid))
        {
            auto res = NewRes();
            res->Meta(objectid);
            res->Type(Res::kObj);
        }
        list.pop_front();
    }
}

Res * Project::GetRes(uint id)
{
    return _resources.at(id);
}

Res * Project::GetResFromPGID(uint PGID)
{
    auto it = std::find_if(_resources.begin(), _resources.end(),
        [PGID] (const std::pair<uint, Res *> & pair)
        {
            return (pair.second->Type() == Res::TypeEnum::kObj 
                 || pair.second->Type() == Res::TypeEnum::kVar)
                 && pair.second->Meta<uint>() == PGID;
        });
    return it != _resources.end() ? it->second : nullptr;
}

std::vector<Res*> Project::GetResByType(const std::vector<Res::TypeEnum> & types)
{
    std::vector<Res *> result;
    for (auto & pair : _resources)
    {
        if (types.empty())
        {
            result.emplace_back(pair.second);
        }
        else
        {
            auto fn =[&pair](const Res::TypeEnum type){return pair.second->Type()==type;};
            if (auto it = std::find_if(types.begin(), types.end(), fn); it != types.end())
            {
                result.emplace_back(pair.second);
            }
        }
    }
    return std::move(result);
}

Res * Project::NewRes()
{
    auto res = new Res(this, ++mGID);
    _resources.emplace(mGID, res);
    return res;
}
