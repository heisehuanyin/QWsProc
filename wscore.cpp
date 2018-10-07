#include "def_configport.h"
#include "def_logport.h"
#include "def_window.h"
#include "singalview.h"
#include "defaultlogport.h"
#include "defaultconfigport.h"
#include "def_channelpreface.h"
#include "defaulttextmodel.h"
#include "def_textmodel.h"

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
    this->connect(p, &PlgDef::I_PluginBase::signal_PushErrorReport,
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

void Core::WsCore::service_OpenFile(const QString filePath, const QString pjtPath, PlgDef::I_PluginBase *win)
{
    //TODO
}

PlgDef::LogPort::I_LogPort *Core::WsCore::instance_GetDefaultLogPort()
{
    auto xargs = QHash<QString, QString>();
    xargs.insert("Encoding", "UTF-8");
    auto rtn = this->service_getManager()->instance_GetLogport(this->DefaultLogpath, xargs);
    rtn->writeLog(nullptr, QDate::currentDate().toString("'This time:' yyyy.MM.dd.dddd")
                  + "("+ QTime::currentTime().toString("HH:mm:ss")  +")");
    return rtn;
}

PlgDef::ConfigPort::I_ConfigPort *Core::WsCore::instance_GetMainConfigPort()
{
    QHash<QString, QString> xargs;
    xargs.insert("Encoding", "UTF-8");

    return this->service_getManager()->instance_GetConfigport(this->DefaultConfigpath, xargs);
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
    QHash<QString, QString> p;
    p.insert("Encoding", "UTF-8");
    auto a = this->service_getManager()->instance_GetChannelPreface(Cfg::DefaultChannelPreface_Value,"./log.wslog","pjtPath");
    auto x = this->service_getManager()->instance_GetTextModel(Cfg::DefaultTextModel_Value, a, p);
    for(int i=0;i<x->getRowsCount();++i){
        std::cout<< x->getLineContent(i).toStdString() << std::endl;
    }
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
    this->service_RegisterPlugin(new Log::DefaultLogPort);
    this->service_RegisterPlugin(new Cfg::DefaultConfigPort);

    this->service_RegisterPlugin(new PlgDef::ChannelPreface::I_ChannelPreface);
    this->service_RegisterPlugin(new PlgDef::TextModel::DefaultTextModel);

    this->service_RegisterPlugin(new Win::DefaultSingalView);
}



Core::PluginManager::PluginManager(Core::WsCore *core):
    factories(new QHash<QString, PlgDef::I_PluginBase *>()),
    configunits(new QList<PlgDef::I_PluginBase *>()),
    instances(new QHash<QString, QList<PlgDef::I_PluginBase *> *>()),
    core(core),
    logportName(""),
    configportName("")
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
        emit p->signal_PushErrorReport(p, error);

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

const QString Core::PluginManager::channel_getChannelId(PlgDef::I_PluginBase *pExample)
{
    for(auto i=this->instances->constBegin();
        i != this->instances->constEnd();
        ++i){
        auto _v = i.value();
        if(_v->contains(pExample)){
            return i.key();
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

Cfg::I_ConfigPort *Core::PluginManager::instance_GetConfigport(const QString fPath, QHash<QString, QString> argslist)
{
    auto cList = this->channel_GetExistsChannel(fPath);
    if(cList != nullptr)
        return dynamic_cast<Cfg::I_ConfigPort *>(*cList->constBegin());

    auto factory = this->factory_GetExistsFactory(this->configportName);
    auto config = dynamic_cast<Cfg::I_ConfigPort *>(factory)->createNewPort(this->core, fPath, argslist);
    this->instance_RegisterPluginInstance(fPath, config);
    this->configunits->append(config);

    return config;
}

Log::I_LogPort *Core::PluginManager::instance_GetLogport(const QString fPath, QHash<QString, QString> argslist)
{
    auto cList = this->channel_GetExistsChannel(fPath);
    if(cList != nullptr)
        return dynamic_cast<Log::I_LogPort *>(*cList->constBegin());

    auto factory = this->factory_GetExistsFactory(this->logportName);

    auto log = dynamic_cast<Log::I_LogPort *>(factory)->createNewPort(this->core, fPath , argslist);
    this->instance_RegisterPluginInstance(fPath, log);
    this->configunits->append(log);

    return log;
}

PlgDef::ChannelPreface::I_ChannelPreface *Core::PluginManager::instance_GetChannelPreface(const QString factory_id,
                                                                                          const QString filePath, const QString pjtPath)
{
    auto cList = this->channel_GetExistsChannel(filePath);
    if(cList != nullptr){
        for(auto i=cList->constBegin();
            i != cList->constEnd();
            ++i){
            if((*i)->registName() == factory_id)
                return dynamic_cast<PlgDef::ChannelPreface::I_ChannelPreface *>(*i);
        }
    }

    auto factory = this->factory_GetExistsFactory(factory_id);
    if(factory == nullptr){
        this->core->slot_Recieve_ProcessError(nullptr, "参数错误，索求插件未注册。id:" + factory_id);
        exit(0);
    }

    auto chnlPreface = dynamic_cast<PlgDef::ChannelPreface::I_ChannelPreface *>(factory)->createNewChannelPreface(this->core, filePath, pjtPath);
    this->instance_RegisterPluginInstance(filePath, chnlPreface);

    return chnlPreface;
}

PlgDef::TextModel::I_TextModel *Core::PluginManager::instance_GetTextModel(const QString factory_id,
                                                                           PlgDef::I_PluginBase *upStream, QHash<QString, QString> xargs)
{
    auto cflag = this->channel_getChannelId(upStream);
    auto cList = this->channel_GetExistsChannel(cflag);
    if(cList != nullptr){
        for(auto i=cList->constBegin();
            i != cList->constEnd();
            ++i){
            if((*i)->registName() == factory_id){
                return dynamic_cast<PlgDef::TextModel::I_TextModel *>(*cList->constBegin());
            }
        }
    }

    auto factory = this->factory_GetExistsFactory(factory_id);
    if(factory == nullptr){
        this->core->slot_Recieve_ProcessError(nullptr, "参数错误，索求插件未注册。id:" + factory_id);
        exit(0);
    }

    auto textm = dynamic_cast<PlgDef::TextModel::I_TextModel*>(factory)->openNewTextModel(this->core, upStream, xargs);
    this->instance_RegisterPluginInstance(cflag, textm);

    return textm;
}

PlgDef::I_PluginBase *Core::PluginManager::instance_GetWindowInstance(const QString gId)
{
    auto cList = this->channel_GetExistsChannel(gId);
    if(cList != nullptr)
        return dynamic_cast<Win::I_Window *>(*cList->constBegin());

    auto factory = this->factory_GetExistsFactoryWithCfg(Cfg::DefaultWindowType_Key, Cfg::DefaultWindowType_Value);

    auto cfgi = this->core->instance_GetMainConfigPort();
    QString width = cfgi->getValue(Cfg::DefaultWindowWidth_Key(gId), Cfg::DefaultWindowWidth_Value);
    QString height = cfgi->getValue(Cfg::DefaultWindowHeight_Key(gId), Cfg::DefaultWindowHeight_Value);

    auto win = dynamic_cast<Win::I_Window *>(factory)->openNewWindow(this->core, gId, width.toInt(), height.toInt());
    this->instance_RegisterPluginInstance(gId, win);

    this->connect(win, &Win::I_Window::signal_resizeWindow,
                  this, &PluginManager::slot_saveWindowSize);

    return win;
}

QHash<PlgDef::PluginType, QList<QPair<QString, PlgDef::PluginType>>> Core::PluginManager::service_QueryFactoryList()
{
    auto rtn = QHash<PlgDef::PluginType, QList<QPair<QString, PlgDef::PluginType>>>();
    for(auto itor = this->factories->constBegin();
        itor != this->factories->constEnd();
        ++itor){
        auto plg_ptr = itor.value();
        auto name = itor.key();

        auto listitor = rtn.find(plg_ptr->pluginMark());
        if(listitor == rtn.constEnd()){
            QList<QPair<QString, PlgDef::PluginType>> alist;
            rtn.insert(plg_ptr->pluginMark(), alist);
            listitor = rtn.find(plg_ptr->pluginMark());
        }
        auto &list = listitor.value();
        list.append(QPair<QString, PlgDef::PluginType>(name, plg_ptr->upStreamMark()));
    }

    return rtn;
}

QList<QPair<QString, PlgDef::PluginType> > Core::PluginManager::service_QueryFactoryList(PlgDef::PluginType typeMark)
{
    auto hash = this->service_QueryFactoryList();
    auto itor = hash.find(typeMark);
    if(itor != hash.constEnd())
        return itor.value();
    return QList<QPair<QString, PlgDef::PluginType>>();
}

QList<QPair<QString, PlgDef::PluginType> > Core::PluginManager::service_QueryFactoryList(const QString pRegistName)
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
    QObject::connect(p, &PlgDef::I_PluginBase::signal_PushErrorReport,
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













