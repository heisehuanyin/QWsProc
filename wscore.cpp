#include "def_configport.h"
#include "def_logport.h"
#include "def_window.h"
#include "SingalView.h"
#include "defaultlogport.h"

namespace Log = PlgDef::LogPort;
namespace Cfg = PlgDef::ConfigPort;
namespace Win = PlgDef::Window;

Core::WsCore::WsCore():
    DefaultLogpath(new QString("./log.wslog")),
    DefaultConfigpath(new QString("./cfg.wscfg")),
    manager(new PluginManager(this))
{

}

Core::WsCore::~WsCore()
{
    delete this->manager;
    delete this->DefaultConfigpath;
    delete this->DefaultLogpath;
}

Core::PluginManager* Core::WsCore::service_getManager()
{
    return manager;
}

void Core::WsCore::service_RegisterPlugin(PlgDef::I_PluginBase *p)
{
    this->manager->factory_RegisterPlugin(p);
}

void Core::WsCore::service_RefreshMenuBar(PlgDef::Window::I_Window *win)
{
    //TODO
}

void Core::WsCore::service_SaveOperation()
{
    this->manager->operate_SaveOperation();
}

void Core::WsCore::service_OpenFile(PlgDef::I_FileSymbo *fileIndicate, PlgDef::I_PluginBase *win)
{
    //TODO
}

PlgDef::LogPort::I_LogPort *Core::WsCore::instance_GetDefaultLogPort()
{
    return this->service_getManager()->instance_GetLogport(this->DefaultLogpath);
}

PlgDef::ConfigPort::I_ConfigPort *Core::WsCore::instance_GetMainConfigPort()
{
    return this->service_getManager()->instance_GetConfigport(this->DefaultConfigpath);
}

void Core::WsCore::service_OpenSilentModel()
{
    this->operate_InitDefaultPlugins();
    this->operate_LoadAllPlugins();
}

void Core::WsCore::service_OpenGraphicsModel(QString *groupId)
{
    this->service_OpenSilentModel();
    this->service_getManager()->instance_GetWindowInstance(groupId);
}

void Core::WsCore::operate_LoadAllPlugins()
{
    //TODO
}

void Core::WsCore::operate_InitDefaultPlugins()
{
    this->service_RegisterPlugin(new Log::DefaultLogPort());
    this->service_RegisterPlugin(new Win::SingalView());
}



Core::PluginManager::PluginManager(Core::WsCore *core):
    factories(new QHash<QString, PlgDef::I_PluginBase *>()),
    configunits(new QList<PlgDef::I_PluginBase *>()),
    instances(new QHash<QString, QList<PlgDef::I_PluginBase *> *>()),
    core(core),
    logportName(new QString),
    configportName(new QString),
    list(new QList<QPair<QString, PlgDef::PluginType>>())
{

}

Core::PluginManager::~PluginManager()
{
    delete this->factories;
    delete this->configunits;
    delete this->instances;
}

void Core::PluginManager::factory_RegisterPlugin(PlgDef::I_PluginBase *p)
{
    if(p->pluginMark() == PlgDef::Service_ConfigPort){
        this->configportName = p->registName();
    }

    if(p->pluginMark() == PlgDef::Service_LogPort){
        this->logportName = p->registName();
    }

    this->factories->insert(*p->registName(), p);
}

void Core::PluginManager::operate_SaveOperation()
{
    for(auto channelPair = this->instances->constBegin();
        channelPair != this->instances->constEnd();
        ++channelPair){
        auto channel = channelPair.value();
        this->channel_SaveChannel(channel);
    }
}


void Core::PluginManager::channel_SaveChannel(QList<PlgDef::I_PluginBase *> * channel)
{
    for(auto plugin = channel->constBegin();
        plugin != channel->constEnd();
        ++plugin){
        (*plugin)->saveOperation();
    }
}

void Core::PluginManager::channel_CloseChannelWithoutSave(QString *const key)
{
    if(this->instances->contains(*key)){
        auto channel = this->instances->take(*key);
        for(auto plugin = channel->begin();
            plugin != channel->end();
            ++plugin){
            delete *plugin;
        }
    }
}

const QString * Core::PluginManager::channel_getChannelId(PlgDef::I_PluginBase *pExample)
{
    for(auto i=this->instances->begin();
        i != this->instances->end();
        ++i){
        auto _v = i.value();
        if(_v->contains(pExample)){
            return &(i.key());
        }
    }

    return nullptr;
}

QList<PlgDef::I_PluginBase *> *Core::PluginManager::channel_GetExistsChannel(QString * const cId)
{
    if(this->instances->contains(*cId)){
        return this->instances->find(*cId).value();
    }
    return nullptr;
}

Cfg::I_ConfigPort *Core::PluginManager::instance_GetConfigport(QString * const fPath)
{
    auto cList = this->channel_GetExistsChannel(fPath);
    if(cList != nullptr)
        return (Cfg::I_ConfigPort *) *cList->constBegin();

    auto factory = this->factory_GetExistsFactory(this->configportName);
    auto config = ((Cfg::I_ConfigPort *)factory)->createNewPort(fPath);
    this->instance_RegisterPluginInstance(fPath, config);
    this->configunits->append(config);

    return config;
}

Log::I_LogPort *Core::PluginManager::instance_GetLogport(QString * const fPath)
{
    auto cList = this->channel_GetExistsChannel(fPath);
    if(cList != nullptr)
        return (Log::I_LogPort *) *cList->constBegin();

    auto factory = this->factory_GetExistsFactory(this->logportName);
    auto log = ((Log::I_LogPort *)factory)->createNewPort(fPath);
    this->instance_RegisterPluginInstance(fPath, log);
    this->configunits->append(log);

    return log;
}

PlgDef::I_PluginBase *Core::PluginManager::instance_GetWindowInstance(QString * const gId)
{
    auto cList = this->channel_GetExistsChannel(gId);
    if(cList != nullptr)
        return (Win::I_Window *) *cList->constBegin();

    auto factory = this->factory_GetExistsFactoryWithCfg(&Log::DefaultWindowType_Key, &Log::DefaultWindowType_Value);
    auto win = ((Win::I_Window *)factory)->openNewWindow(this->core, gId);
    this->instance_RegisterPluginInstance(gId, win);
    this->configunits->append(win);

    return win;
}

QList<QPair<QString, PlgDef::PluginType> > *Core::PluginManager::service_QueryFactoryList(PlgDef::PluginType typeMark)
{
    this->list->clear();

    for(auto itor = this->factories->begin();
        itor != this->factories->end();
        itor++){
        if((itor.value())->pluginMark() == typeMark){
            this->list->append(QPair<const QString, PlgDef::PluginType>
                                (*itor.value()->registName(), itor.value()->upStreamMark()));
        }
    }

    return this->list;
}

QList<QPair<QString, PlgDef::PluginType> > *Core::PluginManager::service_QueryFactoryList(QString * const pRegistName)
{
    auto typeMark = this->factory_GetExistsFactory(pRegistName)->pluginMark();
    return this->service_QueryFactoryList(typeMark);
}

PlgDef::I_PluginBase *Core::PluginManager::factory_GetExistsFactory(QString * const key)
{
    if(this->factories->contains(*key)){
        auto x = this->factories->find(*key);
        return x.value();
    }
    return nullptr;
}

PlgDef::I_PluginBase *Core::PluginManager::factory_GetExistsFactoryWithCfg
            (QString * const key, QString * const defaultVal)
{
    QString *const fId = this->core->instance_GetMainConfigPort()
            ->getValue(key, defaultVal);

    auto factory = this->factory_GetExistsFactory(fId);
    if(factory == nullptr){
        factory = this->factory_GetExistsFactory(defaultVal);
        QString msg = "配置文件出错，插件factory_id不存在，注册值:";
        msg += fId;

        this->core->instance_GetDefaultLogPort()->errorLog(nullptr, &msg);
    }
    return factory;
}

void Core::PluginManager::instance_RegisterPluginInstance(QString * const key, PlgDef::I_PluginBase * const p)
{
    auto cList = this->instances->find(*key);
    if(cList == this->instances->end()){
        auto c = new QList<PlgDef::I_PluginBase *>();
        this->instances->insert(*key, c);
        cList = this->instances->find(*key);
    }
    cList.value()->insert(0, p);
}
