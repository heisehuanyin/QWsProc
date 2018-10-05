#include "def_configport.h"
#include "def_logport.h"
#include "def_window.h"
#include "singalview.h"
#include "defaultlogport.h"
#include "defaultconfigport.h"

namespace Log = PlgDef::LogPort;
namespace Cfg = PlgDef::ConfigPort;
namespace Win = PlgDef::Window;

Core::WsCore::WsCore():
    DefaultLogpath("./log.wslog"),
    DefaultConfigpath("./cfg.wscfg"),
    manager(new PluginManager(this))
{

}

Core::WsCore::~WsCore()
{
    delete this->manager;
}

Core::PluginManager* Core::WsCore::service_getManager()
{
    return manager;
}

void Core::WsCore::service_RegisterPlugin(PlgDef::I_PluginBase *p)
{
    this->connect(p, &PlgDef::I_PluginBase::signal_Recieve_ProcessError,
                  this, &WsCore::slot_Recieve_ProcessError);

    this->manager->factory_RegisterPlugin(p);
}

void Core::WsCore::service_RefreshComponents(PlgDef::Window::I_Window *win)
{
    //TODO
}

void Core::WsCore::service_SaveOperation()
{
    this->manager->operate_SaveOperation();
}

void Core::WsCore::service_OpenFile(const QString filePath, PlgDef::I_PluginBase *win)
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

void Core::WsCore::service_OpenGraphicsModel(const QString groupId)
{
    this->service_OpenSilentModel();
    this->service_getManager()->instance_GetWindowInstance(groupId);
}

void Core::WsCore::test_InnerTest()
{
    auto x = this->instance_GetDefaultLogPort();
    QString msg("测试弹出错误对话框");
    emit x->signal_Recieve_ProcessError(x, msg);
}

void Core::WsCore::slot_Recieve_ProcessError(PlgDef::I_PluginBase * const resp, QString msg)
{
    auto log = this->instance_GetDefaultLogPort();
    log->errorLog(resp,msg);

    QString title("");
    if(resp)
        title += resp->registName();
    else
        title += "MainFrame";

    QMessageBox::StandardButton reply;
    reply = QMessageBox::critical(nullptr, title, msg,QMessageBox::Ignore | QMessageBox::Abort);

    if(reply == QMessageBox::Abort){
        this->service_SaveOperation();
        exit(0);
    }
}

void Core::WsCore::operate_LoadAllPlugins()
{
    //TODO
}

void Core::WsCore::operate_InitDefaultPlugins()
{
    this->service_RegisterPlugin(new Log::DefaultLogPort());
    this->service_RegisterPlugin(new Cfg::DefaultConfigPort());
    this->service_RegisterPlugin(new Win::DefaultSingalView());
}



Core::PluginManager::PluginManager(Core::WsCore *core):
    factories(new QHash<QString, PlgDef::I_PluginBase *>()),
    configunits(new QList<PlgDef::I_PluginBase *>()),
    instances(new QHash<QString, QList<PlgDef::I_PluginBase *> *>()),
    core(core),
    logportName(""),
    configportName(""),
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
    auto repeat = this->factories->find(p->registName());
    if(repeat != this->factories->constEnd()){
        QString error = "重复注册插件：";
        error += p->registName();
        emit p->signal_Recieve_ProcessError(p, error);

        return;
    }

    if(p->pluginMark() == PlgDef::Service_ConfigPort){
        this->configportName = p->registName();
    }

    if(p->pluginMark() == PlgDef::Service_LogPort){
        this->logportName = p->registName();
    }

    this->factories->insert(p->registName(), p);
}

void Core::PluginManager::operate_SaveOperation()
{
    for(auto channelPair = this->instances->constBegin();
        channelPair != this->instances->constEnd();
        ++channelPair){
        auto channel = channelPair.value();
        this->channel_SaveChannel(channel);
    }
    this->channel_SaveChannel(this->configunits);
}


void Core::PluginManager::channel_SaveChannel(QList<PlgDef::I_PluginBase *> * channel)
{
    for(auto plugin = channel->constBegin();
        plugin != channel->constEnd();
        ++plugin){
        (*plugin)->saveOperation();
    }
}

void Core::PluginManager::channel_CloseChannelWithoutSave(const QString key)
{
    if(this->instances->contains(key)){
        auto channel = this->instances->take(key);
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

QList<PlgDef::I_PluginBase *> *Core::PluginManager::channel_GetExistsChannel(const QString cId)
{
    if(this->instances->contains(cId)){
        return this->instances->find(cId).value();
    }
    return nullptr;
}

Cfg::I_ConfigPort *Core::PluginManager::instance_GetConfigport(const QString fPath)
{
    auto cList = this->channel_GetExistsChannel(fPath);
    if(cList != nullptr)
        return dynamic_cast<Cfg::I_ConfigPort *>(*cList->constBegin());

    auto factory = this->factory_GetExistsFactory(this->configportName);
    auto config = dynamic_cast<Cfg::I_ConfigPort *>(factory)->createNewPort(fPath);
    this->instance_RegisterPluginInstance(fPath, config);
    this->configunits->append(config);

    return config;
}

Log::I_LogPort *Core::PluginManager::instance_GetLogport(const QString fPath)
{
    auto cList = this->channel_GetExistsChannel(fPath);
    if(cList != nullptr)
        return dynamic_cast<Log::I_LogPort *>(*cList->constBegin());

    auto factory = this->factory_GetExistsFactory(this->logportName);
    auto log = dynamic_cast<Log::I_LogPort *>(factory)->createNewPort(fPath);
    this->instance_RegisterPluginInstance(fPath, log);
    this->configunits->append(log);

    return log;
}

PlgDef::I_PluginBase *Core::PluginManager::instance_GetWindowInstance(const QString gId)
{
    auto cList = this->channel_GetExistsChannel(gId);
    if(cList != nullptr)
        return dynamic_cast<Win::I_Window *>(*cList->constBegin());

    auto factory = this->factory_GetExistsFactoryWithCfg(Cfg::DefaultWindowType_Key, Cfg::DefaultWindowType_Value);
    auto win = dynamic_cast<Win::I_Window *>(factory)->openNewWindow(this->core, gId);
    this->instance_RegisterPluginInstance(gId, win);

    auto cfgi = this->core->instance_GetMainConfigPort();
    QString width = cfgi->getValue(Cfg::DefaultWindowWidth_Key(gId), Cfg::DefaultWindowWidth_Value);
    QString height = cfgi->getValue(Cfg::DefaultWindowHeight_Key(gId), Cfg::DefaultWindowHeight_Value);
    win->setSize(width.toInt(), height.toInt());

    this->connect(win, &Win::I_Window::signal_resizeWindow,
                  this, &PluginManager::slot_saveWindowSize);

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
                                (itor.value()->registName(), itor.value()->upStreamMark()));
        }
    }

    return this->list;
}

QList<QPair<QString, PlgDef::PluginType> > *Core::PluginManager::service_QueryFactoryList(const QString pRegistName)
{
    auto typeMark = this->factory_GetExistsFactory(pRegistName)->pluginMark();
    return this->service_QueryFactoryList(typeMark);
}

PlgDef::I_PluginBase *Core::PluginManager::factory_GetExistsFactory(const QString key)
{
    if(this->factories->contains(key)){
        auto x = this->factories->find(key);
        return x.value();
    }
    return nullptr;
}

PlgDef::I_PluginBase *Core::PluginManager::factory_GetExistsFactoryWithCfg
            (const QString key, const QString defaultVal)
{
    const QString fId = this->core->instance_GetMainConfigPort()
            ->getValue(key, defaultVal);

    auto factory = this->factory_GetExistsFactory(fId);
    if(factory == nullptr){
        factory = this->factory_GetExistsFactory(defaultVal);
        QString msg = "配置文件出错，插件factory_id不存在，注册值:";
        msg += fId;

        this->core->instance_GetDefaultLogPort()->errorLog(nullptr, msg);
        this->core->slot_Recieve_ProcessError(nullptr, msg);
    }
    return factory;
}

void Core::PluginManager::instance_RegisterPluginInstance(const QString key, PlgDef::I_PluginBase * const p)
{
    QObject::connect(p, &PlgDef::I_PluginBase::signal_Recieve_ProcessError,
                     this->core, &WsCore::slot_Recieve_ProcessError);

    auto cList = this->instances->find(key);
    if(cList == this->instances->end()){
        auto c = new QList<PlgDef::I_PluginBase *>();
        this->instances->insert(key, c);
        cList = this->instances->find(key);
    }
    cList.value()->insert(0, p);
}

void Core::PluginManager::slot_saveWindowSize(const QString groupID, int width, int height)
{
    auto cfg = this->core->instance_GetMainConfigPort();
    QString wStr = QString("%1").arg(width, 5, 10, QChar('0'));
    QString hStr = QString("%1").arg(height,5, 10, QChar('0'));

    cfg->setKeyValue(Cfg::DefaultWindowHeight_Key(groupID), hStr);
    cfg->setKeyValue(Cfg::DefaultWindowWidth_Key(groupID), wStr);
}













