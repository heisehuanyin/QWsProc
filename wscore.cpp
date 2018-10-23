#include "def_configport.h"
#include "def_logport.h"
#include "def_window.h"
#include "def_channelpreface.h"
#include "def_textmodel.h"
#include "def_contentview.h"
#include "def_menubar.h"
#include "def_projectmanager.h"

#include "defaultlogport.h"
#include "defaultconfigport.h"
#include "defaulttextmodel.h"
#include "plaintextview.h"
#include "projectview.h"
#include "singalview.h"
#include "defaultmenubar.h"
#include "wsnovelmanager.h"

#include "customdialog.h"

#include <QFileDialog>
#include <QInputDialog>

namespace Log = PlgDef::LogPort;
namespace Cfg = PlgDef::ConfigPort;
namespace Win = PlgDef::Window;
namespace MBar = PlgDef::MenuBar;
namespace CView = PlgDef::ContentView;
namespace CBase = Core::CoreBase;

using namespace Core;

WsCore::WsCore(QApplication* appi):
    DefaultLogpath("./log.wslog"),
    DefaultConfigpath("./cfg.wscfg"),
    manager(new PluginManager(this)),
    app(appi)
{

}

Core::WsCore::~WsCore()
{
    delete this->manager;
}

Core::PluginManager *Core::WsCore::service_getManager()
{
    return manager;
}

void WsCore::service_RegisterPlugin(PlgDef::I_PluginBase *p)
{
    this->connect(p, &PlgDef::I_PluginBase::signal_PushErrorReport,
                  this, &WsCore::slot_Recieve_ProcessError);

    this->manager->factory_RegisterPlugin(p);
}




void WsCore::service_RefreshComponents(PlgDef::Window::I_Window *win)
{
    //TODO 刷新菜单栏，刷新工具栏，刷新状态栏

    QString gid = win->getGroupID();
    QList<QMenu*> external;
    auto allviews = win->getAllView();

    //=========================================================================
    QMenu * m_File = new QMenu(tr("文件"));
    QAction * _open = new QAction(tr("打开"), m_File);
    this->connect(_open, &QAction::triggered, this, &WsCore::slot_OpenFileGloble);
    m_File->addAction(_open);
    QMenu * _newPjt = new QMenu(tr("新建项目"), m_File);
    this->connect(_newPjt, &QMenu::triggered, this, &WsCore::slot_NewProject);
    QMenu * _newFile = new QMenu(tr("新建文件"), m_File);
    this->connect(_newFile, &QMenu::triggered, this, &WsCore::slot_NewFile);
    m_File->addMenu(_newPjt);
    m_File->addMenu(_newFile);
    QMenu * _close = new QMenu(tr("不保存关闭"), m_File);
    this->connect(_close, &QMenu::triggered, this, &WsCore::slot_CloseFileGloble);
    m_File->addMenu(_close);
    for(auto itor= allviews.constBegin();
        itor != allviews.constEnd();
        ++itor){
        auto id = this->manager->channel_getChannelId(*itor);
        QAction *one = new QAction(id, _close);
        _close->addAction(one);
    }
    m_File->addSeparator();
    QAction * _save = new QAction(tr("保存全部"), m_File);
    this->connect(_save, &QAction::triggered, this, &WsCore::slot_SaveOperation);
    m_File->addAction(_save);
    m_File->addSeparator();
    QMenu * _print = new QMenu(tr("打印"), m_File);
    this->connect(_print, &QMenu::triggered, this, &WsCore::slot_PrintGloble);
    m_File->addMenu(_print);
    for(auto itor = allviews.constBegin();
        itor != allviews.constEnd();
        ++itor){
        auto id = this->manager->channel_getChannelId(*itor);
        QAction *one = new QAction(id, _print);
        _print->addAction(one);
    }
    m_File->addSeparator();
    QAction * _exit = new QAction(tr("退出"), m_File);
    this->connect(_exit, &QAction::triggered, this->app, &QApplication::closeAllWindows);
    m_File->addAction(_exit);

    //==========================================================================================
    QMenu * m_View = new QMenu(tr("视图"));
    this->connect(m_View, &QMenu::triggered, this, &WsCore::slot_ViewChanges);
    for(auto itor = allviews.constBegin();
        itor != allviews.constEnd();
        ++itor){
        auto id = this->manager->channel_getChannelId(*itor);
        QAction *one = new QAction(id, m_View);
        m_View->addAction(one);
    }

    //============================================================================================
    QMenu * m_Preference = new QMenu(tr("配置"));
    QAction * _setting = new QAction(tr("全局配置"), m_Preference);
    this->connect(_setting, &QAction::triggered, this, &WsCore::slot_MainConfigPanel);
    m_Preference->addAction(_setting);
    QMenu * _pcfg = new QMenu(tr("独立配置"), m_Preference);
    this->connect(_pcfg, &QMenu::triggered, this, &WsCore::slot_IndependentConfig);
    m_Preference->addMenu(_pcfg);
    for(auto itor = allviews.constBegin();
        itor != allviews.constEnd();
        ++itor){
        auto id = this->manager->channel_getChannelId(*itor);
        QAction *one = new QAction(id, _pcfg);
        _pcfg->addAction(one);
    }
    m_Preference->addSeparator();
    QAction * _plgmgr = new QAction(tr("插件管理"), m_Preference);
    this->connect(_plgmgr,&QAction::triggered, this, &WsCore::slot_PluginManager);
    m_Preference->addAction(_plgmgr);

    //==========================================================================================
    QMenu * m_Help = new QMenu(tr("帮助"));
    QAction *_about = new QAction(tr("关于软件"),m_Help);
    this->connect(_about, &QAction::triggered, this, &WsCore::slot_SoftwareAbout);
    m_Help->addAction(_about);
    QAction *_help = new QAction(tr("使用帮助"),m_Help);
    this->connect(_help, &QAction::triggered, this, &WsCore::slot_HelpContent);
    m_Help->addAction(_help);
    QAction *_update = new QAction(tr("检查更新"), m_Help);
    this->connect(_update, &QAction::triggered, this, &WsCore::slot_SoftwareUpdate);
    m_Help->addAction(_update);
    QAction *_report = new QAction(tr("报告bug"), m_Help);
    this->connect(_report, &QAction::triggered, this, &WsCore::slot_ReportBugs);
    m_Help->addAction(_report);

    external.append(m_File);
    auto vlist = win->getActivedView();
    for(auto itor = vlist.constBegin();
        itor != vlist.constEnd();
        ++itor){
        auto x = *itor;
        if(x != nullptr && x->getCustomMenu() != nullptr){
            external.append(x->getCustomMenu());
        }
    }
    external.append(m_View);
    external.append(m_Preference);
    external.append(m_Help);

    //刷新Menubar
    MBar::I_MenuBar* mbar = this->manager->instance_GetMenuBarInstance(gid);
    auto bar = mbar->refreshMenuBarWidget(external);
    win->service_ReplaceMenuBar(bar);

    //刷新Toolsbar


    //刷新StatusBar

}

void WsCore::slot_SaveOperation()
{
    this->manager->operate_SaveOperation();
}

void WsCore::operate_OpenFile(QString filePath, Cfg::I_ConfigPort *pjtcfg, Win::I_Window *win)
{
    QFileInfo info(filePath);
    //文件必须存在
    if(!info.exists()){
        this->slot_Recieve_ProcessError(nullptr, "要打开的文件或者目录不存在：" + filePath, true);
        return;
    }
    filePath = info.canonicalFilePath();

    //不允许重复打开文件
    auto clist = this->manager->channel_GetExistsChannel(filePath);
    if(clist != nullptr){
        this->slot_Recieve_ProcessError(nullptr, "不允许重复打开文件：" + filePath, true);
        return;
    }

    //分辨是否项目管理文件，重置配置文件指向，最高优先级
    auto suffix = info.fileName().replace(info.baseName() + ".", "");
    if(this->manager->service_isProjectDefine(suffix)){
        auto cfgpath = info.canonicalPath() + "/" + info.baseName() + ".pjtcfg";
        pjtcfg = this->manager->instance_GetConfigport(cfgpath, QHash<QString, QString>());
    }


    if(info.isFile()){
        //如果pjtcfg == nullptr 简要写法，用于命令行和clr模式打开文件
        if(!pjtcfg){
            QString pjtPath = this->instance_GetMainConfigPort()->getValue(Cfg::DefaultProjectCfgFilePath_Key);
            QHash<QString, QString> xargs;
            xargs.insert("Encoding", "UTF-8");
            pjtcfg = this->manager->instance_GetConfigport(pjtPath, xargs);
        }

        CBase::PluginListNode *plglist = this->getAppointedPluginList(this->instance_GetMainConfigPort(),
                                                                      pjtcfg,
                                                                      filePath);

        this->manager->operate_BuildChannel(filePath,
                                            this->manager->channel_getChannelId(pjtcfg),
                                            plglist,
                                            nullptr,
                                            win);


        delete plglist;
        if(win)
            this->service_RefreshComponents(win);
    }
    if(info.isDir()){
        this->slot_Recieve_ProcessError(nullptr, "待处理目录打开请求，暂时未处理", true);
    }
}

void WsCore::operate_CloseContentView(PlgDef::ContentView::I_ContentView *comp, bool wetherSave)
{
    auto win = comp->getOwner();
    win->removeView(comp);
    auto id = this->manager->channel_getChannelId(comp);

    if(wetherSave){
        auto clist = this->manager->channel_GetExistsChannel(id);
        this->manager->channel_SaveChannel(clist);
    }
    this->manager->channel_CloseChannelWithoutSave(id);
}


PlgDef::LogPort::I_LogPort * WsCore::instance_GetDefaultLogPort()
{
    auto xargs = QHash<QString, QString>();
    xargs.insert("Encoding", "UTF-8");
    auto rtn = this->manager->instance_GetLogport(this->DefaultLogpath, xargs);
    rtn->writeLog(nullptr, QDate::currentDate().toString("'This time:' yyyy.MM.dd.dddd")
                  + "("+ QTime::currentTime().toString("HH:mm:ss")  +")");
    return rtn;
}

PlgDef::ConfigPort::I_ConfigPort * WsCore::instance_GetMainConfigPort()
{
    QHash<QString, QString> xargs;
    xargs.insert("Encoding", "UTF-8");

    return this->manager->instance_GetConfigport(this->DefaultConfigpath, xargs);
}

void WsCore::service_OpenSilentModel()
{
    this->service_EnvironmentCheck();
}

void WsCore::service_OpenGraphicsModel(const QString groupId)
{
    this->service_OpenSilentModel();
    QString gid = groupId;
    if(groupId == QString())
        gid = Cfg::DefaultGraphicsGroupId;
    auto win = this->manager->instance_GetWindowInstance(gid);
    this->instance_GetDefaultLogPort()->writeLog(nullptr, "打开窗口，Groupid=" + gid);
    this->service_RefreshComponents(win);
}

void WsCore::service_EnvironmentCheck()
{
    this->operate_InitDefaultPlugins();
    this->operate_LoadAllPlugins();

    // else ========================
    auto cfg = this->instance_GetMainConfigPort();
    auto defaultPjtPath = cfg->getValue(Cfg::DefaultProjectCfgFilePath_Key);
    if(defaultPjtPath == QString()){
        QFileDialog::Option option = QFileDialog::ShowDirsOnly;
        QString defaultPjtPath = QFileDialog::getExistingDirectory(nullptr, tr("选择默认项目保存路径"),"/home",option);

        if(defaultPjtPath == QString()){
            this->slot_Recieve_ProcessError(nullptr, "主目录输入问题");
            exit(0);
        }else{
            defaultPjtPath.append("/project.pjtcfg");
        }
        cfg->setKeyValue(Cfg::DefaultProjectCfgFilePath_Key, defaultPjtPath);
        this->instance_GetDefaultLogPort()->writeLog(nullptr, "启动时DefaultProjectPath不存在，软件将Path重设为：" + defaultPjtPath);
    }
}

PlgDef::Window::I_Window *WsCore::getActivedWindow()
{
    auto widget = this->app->activeWindow();
    return this->manager->service_CheckWindow(widget);
}


CBase::PluginListNode* WsCore::buildPluginList(QString keyExport,
                                               QString procdef,
                                               Cfg::I_ConfigPort* pjtCfg){

    CBase::PluginListNode * rtn = nullptr;
    CBase::PluginListNode *parent = nullptr;

    QStringList list = procdef.split("=>");
    for(auto it = list.constBegin();
        it != list.constEnd();
        ++it){

        CBase::PluginListNode *nnode = new CBase::PluginListNode(*it, parent);
        if(it == list.constBegin()){
            rtn = nnode;
        }

        if(parent){
            parent->setNextNode(nnode);
        }
        parent = nnode;


        auto args = pjtCfg->getConfigList(keyExport + "." + *it);
        nnode->mergeArgsList(args);

    }

    return rtn;
}

void WsCore::slot_OpenFileGloble()
{
    auto activeW = this->getActivedWindow();
    auto list = QFileDialog::getOpenFileNames(activeW->getWidget(),
                                              tr("选择打开文件"),
                                              "/home");
    for(int i = 0; i < list.length(); i++){
        auto path = list.at(i);
        this->operate_OpenFile(path, nullptr, activeW);
    }
}

void WsCore::slot_NewProject(QAction *act)
{

}
void WsCore::slot_NewFile(QAction* act)
{
    std::cout<<"newfile"<<std::endl;
}
void WsCore::slot_PrintGloble(QAction* act)
{
    std::cout<<"print"<<std::endl;
}
void WsCore::slot_CloseFileGloble(QAction* act)
{
    auto id = act->text();
    auto c = this->manager->channel_GetExistsChannel(id);
    for(auto itor = c->constBegin();
        itor != c->constEnd();
        ++itor){
        if((*itor)->pluginMark() == PlgDef::UI_ContentView){
            this->operate_CloseContentView(dynamic_cast<CView::I_ContentView*>(*itor), false);
            break;
        }
    }
    this->service_RefreshComponents(this->getActivedWindow());
}
void WsCore::slot_ViewChanges(QAction *act)
{
    auto aw = this->getActivedWindow();
    auto id = act->text();
    auto clist = this->manager->channel_GetExistsChannel(id);
    for(auto itor=clist->constBegin();
        itor != clist->constEnd();
        ++itor){
        if((*itor)->pluginMark() == PlgDef::UI_ContentView){
            auto view = dynamic_cast<CView::I_ContentView*>(*itor);
            view->getOwner()->bringViewToFront(view);
            this->service_RefreshComponents(aw);
            break;
        }
    }
}
void WsCore::slot_MainConfigPanel()
{
    auto x = this->getActivedWindow();
    this->customPane4ConfigPort(x, nullptr);
}

void WsCore::slot_IndependentConfig(QAction *act)
{
    auto aw = this->getActivedWindow();
    auto id = act->text();
    auto pjtm = this->getProjectConfigPort(this->manager->
                                           channel_GetExistsChannel(id)->
                                           at(0));
    this->customPane4ConfigPort(aw, pjtm, id);
}
void WsCore::slot_PluginManager()
{

}
void WsCore::slot_SoftwareAbout()
{

}
void WsCore::slot_HelpContent()
{

}
void WsCore::slot_ReportBugs()
{

}
void WsCore::slot_SoftwareUpdate()
{

}
void WsCore::customPane4ConfigPort(PlgDef::Window::I_Window* parent,
                                   PlgDef::ConfigPort::I_ConfigPort *customTarget,
                                   QString filepathAtProject)
{
    Win::I_Window *win = parent;

    CustomDialog preference(this, this->instance_GetMainConfigPort(), win->getWidget());
    if(customTarget){
        QString path = this->manager->channel_getChannelId(customTarget);
        preference.setFileParserCustomPane(customTarget, path, filepathAtProject);
    }
    preference.exec();
}
QString WsCore::service_Proc4Suffix(QString fullFilePath)
{
    QFileInfo info(fullFilePath);
    QString suffix = info.fileName().replace(info.baseName() + ".", "");
    suffix.replace('.', '_');
    return suffix;
}
QString WsCore::service_ProcFilePath(QString fullFilePath, QString pjtfullFilePath)
{
    QFileInfo pjt(pjtfullFilePath);
    QString pdir = pjt.canonicalPath();

    QFileInfo file(fullFilePath);
    fullFilePath = file.canonicalFilePath();

    if(fullFilePath.indexOf(pdir) == -1){
        fullFilePath.replace(':',"___");
    }else{
        fullFilePath.replace(pdir+"/", "");
    }

    return fullFilePath.replace('.','_')
            .replace('\\', "__")
            .replace('/',"__")
            .replace(" ","-")
            .replace("(","-")
            .replace(")","-");
}

QString WsCore::service_getPluginlistdefAtCfgport(QString keyStr, PlgDef::ConfigPort::I_ConfigPort *cfgPort, CBase::PluginListNode **nodelist)
{
    QString procdef = cfgPort->getValue(keyStr);
    if(procdef == QString()){
        return procdef;
    }

    //构建主链条
    *nodelist = buildPluginList(keyStr,
                                procdef,        //主链条定义
                                cfgPort         //项目配置文件，用于搜索模块参数定义
                                );

    return procdef;
}

void WsCore::service_setPluginlistdefAtCfgport(CBase::PluginListNode *nodelist, QString keyStr, PlgDef::ConfigPort::I_ConfigPort *cfgPort)
{
    auto args = nodelist->getArgsList();
    cfgPort->setConfigList(keyStr+"."+nodelist->getPluginName(), *args);

    QString temp("");
    if(nodelist->getPreviousNode() != nullptr)
        temp = cfgPort->getValue(keyStr);

    temp += nodelist->getPluginName();
    if(nodelist->getNextNode() != nullptr){
        temp += "=>";
    }
    cfgPort->setKeyValue(keyStr, temp);

    if(nodelist->getNextNode() != nullptr)
        this->service_setPluginlistdefAtCfgport(nodelist->getNextNode(), keyStr, cfgPort);
}

CBase::PluginListNode *Core::WsCore::getAppointedPluginList(PlgDef::ConfigPort::I_ConfigPort *frameConfig,
                                                            PlgDef::ConfigPort::I_ConfigPort *projectConfig,
                                                            QString filePath)
{
    // Replace invalid string
    QString suffix = this->service_Proc4Suffix(filePath);
    QString prjPath = this->service_getManager()->channel_getChannelId(projectConfig);
    filePath = this->service_ProcFilePath(filePath, prjPath);

    // Encode the key string
    QStringList list_key;
    list_key << Cfg::EncodeFromMsg_4_KeyExport(filePath)
             << Cfg::EncodeFromMsg_4_KeyExport(suffix)
             << Cfg::EncodeFromMsg_4_KeyExport("____default")
             << Cfg::EncodeFromMsg_4_KeyExport(suffix)
             << Cfg::EncodeFromMsg_4_KeyExport("____default");

    CBase::PluginListNode * nodelist;
    for(int i=0; i<list_key.length(); ++i){
        QString procdef;
        auto key = list_key.at(i);
        if(i < 3){
            procdef = this->service_getPluginlistdefAtCfgport(key, projectConfig, &nodelist);
        }else{
            procdef = this->service_getPluginlistdefAtCfgport(key, frameConfig, &nodelist);
            if(i == 4 && procdef == QString()){
                nodelist = this->buildPluginList(key, Cfg::DefaultAnyFileParseList, frameConfig);
                this->service_setPluginlistdefAtCfgport(nodelist, key, frameConfig);
                procdef = Cfg::DefaultAnyFileParseList;
            }
            if(procdef != QString()){
                this->service_setPluginlistdefAtCfgport(nodelist, key, projectConfig);
            }
        }

        if(procdef != QString())
            return nodelist;
    }
    return nullptr;
}


void WsCore::test_InnerTest()
{
    QHash<QString, QString> p;
    p.insert("Encoding", "UTF-8");
    auto a = this->manager->instance_GetChannelPreface(Cfg::DefaultChannelPreface_Value,"./log.wslog","pjtPath");
    auto x = this->manager->instance_GetTextModel(Cfg::DefaultTextModel_Value, a, p);
    for(int i=0;i<x->getRowsCount();++i){
        std::cout<< x->getLineContent(i).toStdString() << std::endl;
    }
}

PlgDef::ConfigPort::I_ConfigPort *Core::WsCore::getProjectConfigPort(PlgDef::I_PluginBase *p){
    QString cid = this->manager->channel_getChannelId(p);

    auto list = this->manager->channel_GetExistsChannel(cid);
    if(list != nullptr){
        auto x = -- (list->constEnd());
        auto p = *x;
        if(p->pluginMark() != PlgDef::IO_ChannelPreface)
            return nullptr;

        QString pjtPath = dynamic_cast<PlgDef::ChannelPreface::I_ChannelPreface *>(p)->getProjectCfgfilePath();
        auto rtn = this->manager->instance_GetConfigport(pjtPath, QHash<QString, QString>());
        return rtn;
    }

    return nullptr;
}

void WsCore::slot_Recieve_ProcessError(PlgDef::I_PluginBase * const resp, QString msg, bool ignoreAllows)
{
    auto log = this->instance_GetDefaultLogPort();
    log->errorLog(resp,msg);

    QString title("");
    if(resp)
        title += resp->registName();
    else
        title += "MainFrame";

    QMessageBox::StandardButton reply;
    if(ignoreAllows){
        reply = QMessageBox::critical(nullptr, title, msg, QMessageBox::Ignore | QMessageBox::Abort, QMessageBox::Abort);
    }else{
        reply = QMessageBox::critical(nullptr, title, msg, QMessageBox::Abort);
    }


    if(reply == QMessageBox::Abort || !ignoreAllows){
        this->slot_SaveOperation();
        exit(0);
    }
}

void WsCore::slot_Recieve_ProcessNomarlMsg(PlgDef::I_PluginBase * const res, QString msg)
{
    auto log = this->instance_GetDefaultLogPort();
    log->writeLog(res, msg);

    QString title("");
    if(res)
        title += res->registName();
    else
        title += "MainFrame";

    QMessageBox::information(nullptr, title, msg);
}

void WsCore::operate_LoadAllPlugins()
{
    //TODO
}

void WsCore::operate_InitDefaultPlugins()
{
    this->service_RegisterPlugin(new Log::DefaultLogPort);
    this->service_RegisterPlugin(new Cfg::DefaultConfigPort);

    this->service_RegisterPlugin(new PlgDef::ChannelPreface::I_ChannelPreface);
    this->service_RegisterPlugin(new PlgDef::TextModel::DefaultTextModel);
    this->service_RegisterPlugin(new PlgDef::ProjectManager::WsNovelProjectManager);

    this->service_RegisterPlugin(new CView::PlainTextView);
    this->service_RegisterPlugin(new PlgDef::ContentView::DefaultProjectView);

    this->service_RegisterPlugin(new Win::DefaultSingalView);
    this->service_RegisterPlugin(new MBar::DefaultMenuBar);
}



Core::PluginManager::PluginManager(Core::WsCore *core):
    factories(new QHash<QString, PlgDef::I_PluginBase *>()),
    key_ins(new QList<PlgDef::I_PluginBase *>()),
    instances(new QHash<QString, QList<PlgDef::I_PluginBase *> *>()),
    core(core),
    logportName(""),
    configportName("")
{

}

Core::PluginManager::~PluginManager()
{
    delete this->factories;
    delete this->key_ins;
    delete this->instances;
}

void PluginManager::factory_RegisterPlugin(PlgDef::I_PluginBase *p)
{
    auto repeat = this->factories->find(p->registName());
    if(repeat != this->factories->constEnd()){
        QString error = "重复注册插件：";
        error += p->registName();
        error += " {Type:";
        error += QString("%1, ").arg(p->pluginMark());
        if((p->pluginMark() | PlgDef::Feature_Combination) == p->pluginMark()){
            error += "UpStream Type:";
            error += QString("%1").arg(dynamic_cast<PlgDef::I_Combiantion*>(p)->upStreamMark());
        }
        error += "}";

        emit p->signal_PushErrorReport(p, error);

        return;
    }

    if(p->pluginMark() == PlgDef::Service_ConfigPort){
        this->configportName = p->registName();
    }

    if(p->pluginMark() == PlgDef::Service_LogPort){
        this->logportName = p->registName();
    }
    if(p->pluginMark() == PlgDef::Service_ProjectManager){
        auto pmr = dynamic_cast<PlgDef::ProjectManager::I_ProjectManager*>(p);
        this->pjtformats.append(pmr->suffix());
    }

    this->factories->insert(p->registName(), p);
}

void PluginManager::operate_SaveOperation()
{
    for(auto channelPair = this->instances->constBegin();
        channelPair != this->instances->constEnd();
        ++channelPair){
        auto channel = channelPair.value();
        this->channel_SaveChannel(channel);
    }
    this->channel_SaveChannel(this->key_ins);
}

PlgDef::I_PluginBase* PluginManager::operate_BuildChannel(const QString filepath,
                                                          const QString projectpath,
                                                          CBase::PluginListNode * plglist,
                                                          PlgDef::I_PluginBase* upStream,
                                                          PlgDef::Window::I_Window* win)
{
    auto plgname = plglist->getPluginName();
    auto factory = this->factory_GetExistsFactory(plgname);
    PlgDef::I_PluginBase* thisInstance = nullptr;


    switch (factory->pluginMark()) {
    case PlgDef::IO_ChannelPreface:{
            thisInstance = this->instance_GetChannelPreface(plgname, filepath, projectpath);
        };break;
    case PlgDef::IO_TextModel:{
            thisInstance = this->instance_GetTextModel(plgname, upStream, *plglist->getArgsList());
        };break;
    case PlgDef::UI_ContentView:{
            thisInstance = this->instance_GetContentView(win, plgname, upStream, *plglist->getArgsList());
        };break;
    case PlgDef::Service_ProjectManager:{
            thisInstance = this->instance_GetProjectManager(plgname, upStream, *plglist->getArgsList());
        };break;
    }


    if(plglist->getNextNode() != nullptr){
        return this->operate_BuildChannel(filepath, projectpath,plglist->getNextNode(), thisInstance, win);
    }

    return thisInstance;
}

bool PluginManager::service_isProjectDefine(QString suffix)
{
    return this->pjtformats.contains(suffix);
}

PlgDef::Window::I_Window *PluginManager::service_CheckWindow(QWidget *widget)
{
    for(auto itor= this->winlist.constBegin();
        itor != this->winlist.constEnd();
        ++itor){
        auto x = *itor;
        if(x->getWidget() == widget){
            return x;
        }
    }
    return nullptr;
}


void PluginManager::channel_SaveChannel(QList<PlgDef::I_PluginBase *> * channel)
{
    for(auto plugin = channel->constBegin();
        plugin != channel->constEnd();
        ++plugin){
        (*plugin)->saveOperation();
    }
}

void PluginManager::channel_CloseChannelWithoutSave(const QString key)
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

const QString PluginManager::channel_getChannelId(PlgDef::I_PluginBase *pExample)
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
        auto x = this->instances->find(cId);
        if(x != this->instances->constEnd())
            return x.value();
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
    this->key_ins->append(config);

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
    this->key_ins->append(log);

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
                return dynamic_cast<PlgDef::TextModel::I_TextModel *>(*i);
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

PlgDef::ProjectManager::I_ProjectManager *PluginManager::instance_GetProjectManager(const QString factory_id,
                                                                                    PlgDef::I_PluginBase *upStream,
                                                                                    QHash<QString, QString> xargs)
{
    auto cflag = this->channel_getChannelId(upStream);
    auto cList = this->channel_GetExistsChannel(cflag);
    if(cList != nullptr){
        for(auto i=cList->constBegin();
            i != cList->constEnd();
            ++i){
            if((*i)->registName() == factory_id){
                return dynamic_cast<PlgDef::ProjectManager::I_ProjectManager *>(*i);
            }
        }
    }

    auto factory = this->factory_GetExistsFactory(factory_id);
    if(factory == nullptr){
        this->core->slot_Recieve_ProcessError(nullptr, "参数错误，索求插件未注册。id:" + factory_id);
        exit(0);
    }

    auto pjtmgr = dynamic_cast<PlgDef::ProjectManager::I_ProjectManager *>(factory)
            ->openExistProject(this->core, upStream, xargs);
    this->instance_RegisterPluginInstance(cflag, pjtmgr);

    return pjtmgr;
}

CView::I_ContentView *Core::PluginManager::instance_GetContentView(Win::I_Window * win,
                                                                   const QString factory_id,
                                                                   PlgDef::I_PluginBase *upStream,
                                                                   QHash<QString, QString> xargs)
{
    auto cflag = this->channel_getChannelId(upStream);
    auto cList = this->channel_GetExistsChannel(cflag);
    if(cList != nullptr){
        for(auto i=cList->constBegin();
            i != cList->constEnd();
            ++i){
            if((*i)->registName() == factory_id){
                return dynamic_cast<PlgDef::ContentView::I_ContentView *>(*cList->constBegin());
            }
        }
    }

    auto factory = this->factory_GetExistsFactory(factory_id);
    if(factory == nullptr){
        this->core->slot_Recieve_ProcessError(nullptr, "参数错误，索求插件未注册。id:" + factory_id);
        exit(0);
    }

    auto view = dynamic_cast<PlgDef::ContentView::I_ContentView*>(factory)
            ->createNewContentView(this->core, upStream, xargs, win);
    this->instance_RegisterPluginInstance(cflag,view);

    if(win != nullptr)
        win->placeView(cflag, view);

    return view;
}

Win::I_Window *Core::PluginManager::instance_GetWindowInstance(const QString gId)
{
    auto cList = this->channel_GetExistsChannel(gId);
    if(cList != nullptr)
        for(auto i=cList->constBegin();
            i != cList->constEnd();
            ++i){
            if((*i)->pluginMark() == PlgDef::UI_Window)
                return dynamic_cast<Win::I_Window*>(*i);
        }

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

PlgDef::MenuBar::I_MenuBar *PluginManager::instance_GetMenuBarInstance(const QString gId)
{
    auto cList = this->channel_GetExistsChannel(gId);
    if(cList != nullptr)
        for(auto i=cList->constBegin();
            i != cList->constEnd();
            ++i){
            if((*i)->pluginMark() == PlgDef::UI_MenuBar)
                return dynamic_cast<MBar::I_MenuBar*>(*i);
        }

    auto factory = this->factory_GetExistsFactoryWithCfg(Cfg::DefaultMenuBarType_Key, Cfg::DefaultMenuBarType_Value);

    auto mbar = dynamic_cast<MBar::I_MenuBar*>(factory)->createNewMenubar(this->core, gId);
    this->instance_RegisterPluginInstance(gId, mbar);

    return mbar;
}

QHash<PlgDef::PluginType, QList<QPair<QString, PlgDef::PluginType>>> PluginManager::service_QueryFactoryList()
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
        auto x = PlgDef::Feature_NoCombination;
        if(plg_ptr->pluginMark() == (plg_ptr->pluginMark() | PlgDef::Feature_Combination))
            x = dynamic_cast<PlgDef::I_Combiantion*>(plg_ptr)->upStreamMark();

        list.append(QPair<QString, PlgDef::PluginType>(name, x));
    }

    return rtn;
}

QList<QPair<QString, PlgDef::PluginType> > PluginManager::service_QueryFactoryList(PlgDef::PluginType typeMark)
{
    auto hash = this->service_QueryFactoryList();
    auto itor = hash.find(typeMark);
    if(itor != hash.constEnd())
        return itor.value();
    return QList<QPair<QString, PlgDef::PluginType>>();
}

QList<QPair<QString, PlgDef::PluginType> > PluginManager::service_QueryFactoryList(const QString pRegistName)
{
    auto typeMark = this->factory_GetExistsFactory(pRegistName)->pluginMark();
    return this->service_QueryFactoryList(typeMark);
}

QList<QString> PluginManager::service_QueryChannelList()
{
    return this->instances->keys();
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

void PluginManager::instance_RegisterPluginInstance(const QString key, PlgDef::I_PluginBase * const p)
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
    if(p->pluginMark() == PlgDef::UI_Window){
        this->winlist.append(dynamic_cast<Win::I_Window*>(p));
    }
}

void PluginManager::slot_saveWindowSize(const QString groupID, int width, int height)
{
    auto cfg = this->core->instance_GetMainConfigPort();
    QString wStr = QString("%1").arg(width, 5, 10, QChar('0'));
    QString hStr = QString("%1").arg(height,5, 10, QChar('0'));

    cfg->setKeyValue(Cfg::DefaultWindowHeight_Key(groupID), hStr);
    cfg->setKeyValue(Cfg::DefaultWindowWidth_Key(groupID), wStr);
}













