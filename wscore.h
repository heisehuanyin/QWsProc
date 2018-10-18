#ifndef WSCORE_H
#define WSCORE_H

#include "def_plugin_base.h"
#include "pluginlistnode.h"
#include <QMessageBox>
#include <QApplication>

namespace PlgDef {
    namespace ConfigPort {
        class I_ConfigPort;
    }
    namespace LogPort {
        class I_LogPort;
    }
    namespace Window {
        class I_Window;
    }
    namespace ChannelPreface {
        class I_ChannelPreface;
    }
    namespace TextModel {
        class I_TextModel;
    }
    namespace ContentView {
        class I_ContentView;
    }
    namespace MenuBar {
        class I_MenuBar;
    }
}



namespace Core {
    class WsCore;

    class PluginManager final:public QObject
    {
        Q_OBJECT

    public:
        PluginManager(WsCore* core);
        ~PluginManager();

        /**
         * @brief 注册插件实例生成器
         * @param p 工厂类
         */
        void factory_RegisterPlugin(PlgDef::I_PluginBase *p);

        /**
         * @brief 全局保存操作
         */
        void operate_SaveOperation();

        /**
         * @brief 构建一个打开的文件编辑通道
         * @param filepath 文件路径
         * @param pjtcfgpath 项目配置文件路径
         * @param plglist 插件定义链
         * @param upStream 上游插件实例
         * @param win 刷新窗口
         * @return 插件链条最后一个插件实例
         */
        PlgDef::I_PluginBase* operate_BuildChannel(const QString filepath,
                                  const QString pjtcfgpath,
                                  CoreBase::PluginListNode * plglist,
                                  PlgDef::I_PluginBase* upStream,
                                  PlgDef::Window::I_Window* win);

        /**
         * @brief 校对本文件后缀名是否用于定义项目
         * @param suffix 后缀
         * @return 结果
         */
        bool service_isProjectDefine(QString suffix);

        /**
         * @brief 核查，确认与提供widget相同的Window实例
         * @param widget 关键widget
         * @return window实例
         */
        PlgDef::Window::I_Window* service_CheckWindow(QWidget* widget);

        /**
         * @brief 保存特定通道所有插件内容
         * @param c 通道实例
         */
        void channel_SaveChannel(QList<PlgDef::I_PluginBase *> * c);

        /**
         * @brief 移除特定通道
         * @param cId 通道id
         */
        void channel_CloseChannelWithoutSave(const QString cId);

        /**
         * @brief 通过插件实例，查询指定插件通道id
         * @param pExample 插件实例
         * @return 通道id，一般是指向URL
         */
        const QString channel_getChannelId(PlgDef::I_PluginBase *pExample);

        /**
         * @brief 通过通道id获取整条通道
         * @param cId 通道id
         * @return 插件实例通道
         */
        QList<PlgDef::I_PluginBase *> * channel_GetExistsChannel(const QString cId);


        //Special Components
        /**
         * @brief 获取一个Configport实例，每次程序启动，只保留最后装载的一种Configport插件
         * @param fPath 配置文件路径
         * @return Configport实例
         */
        PlgDef::ConfigPort::I_ConfigPort *instance_GetConfigport(const QString fPath, QHash<QString,QString> argslist);

        /**
         * @brief 获取一个Logport实例，每次程序启动，只保留最后装载的一种Logport插件
         * @param fPath Log文件路径
         * @return Logport插件实例
         */
        PlgDef::LogPort::I_LogPort *instance_GetLogport(const QString fPath, QHash<QString, QString> argslist);

        /**
         * @brief 获得一个ChannelPreface实例
         * @param filePath 向其中存储文件路径，也是通道id，也是插件实例分组id
         * @param pjtPath 向其中存储文件属于的项目路径
         * @return 实例
         */
        PlgDef::ChannelPreface::I_ChannelPreface *instance_GetChannelPreface(const QString factory_id,
                                                                             const QString filePath, const QString pjtPath);

        /**
         * @brief 获得一个TextModel实例
         * @param factory_id 插件注册名
         * @param upStream 上游插件
         * @param xargs 参数列表
         * @return 实例
         */
        PlgDef::TextModel::I_TextModel *instance_GetTextModel(const QString factory_id,
                                                              PlgDef::I_PluginBase *upStream, QHash<QString, QString> xargs);

        /**
         * @brief 获取一个内容视图实例
         * @param factory_id
         * @param upStream
         * @param xargs
         * @return
         */
        PlgDef::ContentView::I_ContentView* instance_GetContentView(PlgDef::Window::I_Window * win, const QString factory_id,
                                                                    PlgDef::I_PluginBase *upStream, QHash<QString, QString> xargs);
        //UI Components
        /**
         * @brief 获取配置项设定的窗口实例一个
         * @param gId GroupId
         * @return 实例
         */
        PlgDef::Window::I_Window * instance_GetWindowInstance(const QString gId);

        /**
         * @brief 获取配置项设定的菜单栏实例一个
         * @param gId GroupId
         * @return 实例
         */
        PlgDef::MenuBar::I_MenuBar * instance_GetMenuBarInstance(const QString gId);

        //Query Methods
        /**
         * @brief 查询所有载入的插件
         * @return
         */
        QHash<PlgDef::PluginType, QList<QPair<QString, PlgDef::PluginType>>> service_QueryFactoryList();

        /**
         * @brief 通过指定typeMark，查询所有同种插件列表
         * @param typeMark 插件类别码
         * @return 信息组合
         */
        QList<QPair<QString, PlgDef::PluginType> > service_QueryFactoryList(PlgDef::PluginType typeMark);

        /**
         * @brief 通过指定插件注册名，查询所有同种插件列表
         * @param pRegistName 插件注册名
         * @return 信息组合
         */
        QList<QPair<QString, PlgDef::PluginType>> service_QueryFactoryList(const QString pRegistName);

        QList<QString> service_QueryChannelList();

    private:
        QHash<QString, PlgDef::I_PluginBase *> *const factories;
        QList<PlgDef::I_PluginBase *> *const key_ins;
        QHash<QString, QList<PlgDef::I_PluginBase *> *> *const instances;
        WsCore *const core;
        QString logportName;
        QString configportName;
        QList<QString> pjtformats;
        QList<PlgDef::Window::I_Window*> winlist;

        /**
         * @brief 获取已经存在的插件工厂
         * @param key 插件注册名
         * @return 插件实例生成器
         */
        PlgDef::I_PluginBase * factory_GetExistsFactory(const QString key);

        /**
         * @brief 输入指定配置条目和默认配置值，获取插件实例生成器。若配置文件中条目为空，写入默认值
         * @param key 配置文件中配置条目
         * @param defaultVal 默认配置值
         * @return 插件实例
         */
        PlgDef::I_PluginBase * factory_GetExistsFactoryWithCfg(const QString key, const QString defaultVal);

        /**
         * @brief 注册插件实例到Manager
         * @param key 注册键名
         * @param p 插件实例
         */
        void instance_RegisterPluginInstance(const QString key, PlgDef::I_PluginBase *const p);

    private slots:
        void slot_saveWindowSize(const QString groupID, int width, int height);

    };




    class WsCore final:public QObject
    {
        Q_OBJECT

    public:
        WsCore(QApplication *appi);
        ~WsCore();

        PluginManager *service_getManager();
        /**
         * @brief 注册插件工厂便捷接口
         * @param p 插件实例
         */
        void service_RegisterPlugin(PlgDef::I_PluginBase *p);

        /**
         * @brief 更新指定窗口实例上的各种组件
         * @param win 指定窗口实例
         */
        void service_RefreshComponents(PlgDef::Window::I_Window *win);


        /**
         * @brief 打开指定文件，如果通道中插件有ContentView类型且win!=nullptr，ContentView会自动加载到本窗口上
         * 1.如果指定文件是项目定义文件，其默认项目配置文件处于同路径，同文件名，解析过程中先加载配置文件，然后应用于项目文件解析
         * 2.如果指定文件不是项目定义文件，其打开过程需要提供项目配置文件，以便于提取解析参数，若不指定项目配置文件，则采用默认项目配置文件
         * @param filePath 目标文件所在路径
         * @param prjPath 项目配置文件所在路径
         * @param win 发出请求的窗口
         */
        void operate_OpenFile(const QString filePath, PlgDef::ConfigPort::I_ConfigPort* prjcfg = nullptr, PlgDef::Window::I_Window *win = nullptr);

        /**
         * @brief 关闭指定ContentView
         * @param comp 指定组件
         * @param wetherSave 是否保持选项
         */
        void operate_CloseContentView(PlgDef::ContentView::I_ContentView *comp, bool wetherSave=true);

        /**
         * @brief 获取默认的LogPort插件实例
         * @return 实例
         */
        PlgDef::LogPort::I_LogPort* instance_GetDefaultLogPort();

        /**
         * @brief 获取默认的ConfigPort插件实例
         * @return 实例
         */
        PlgDef::ConfigPort::I_ConfigPort* instance_GetMainConfigPort();

        /**
         * @brief 打开静默模式，可以通过命令行和宏文件操纵软件
         */
        void service_OpenSilentModel();

        /**
         * @brief 打开图形模式，所有操作可以通过图形界面进行操作，groupid默认值为QString()
         * @param groupId 插件实例注册id,默认值为QString()
         */
        void service_OpenGraphicsModel(const QString groupId = QString());


        void test_InnerTest();

        /**
         * @brief 获取本文件所属项目的配置端口
         * @param p 调用插件
         * @return 端口
         */
        PlgDef::ConfigPort::I_ConfigPort* getProjectConfigPort(PlgDef::I_PluginBase *p);

        /**
         * @brief 弹出软件配置面板，可以处理项目配置
         * @param groupId 窗口的groupid码
         * @param customTarget 目标配置端口
         */
        void customPane4ConfigPort(PlgDef::Window::I_Window* parent, PlgDef::ConfigPort::I_ConfigPort * customTarget = nullptr);

        QString service_Proc4Suffix(QString fullFilePath);
        QString service_ProcFilePath(QString fullFilePath, QString pjtFullFilePath);

        /**
         * @brief 获取插件定义链，如果该配置端口中无定义，返回QString()
         * @param keyStr 索引定义关键字符串,首先经过去除非法字符处理，又经过编码处理后的“文件路径”、“后缀名”、“全局默认键值”
         * @param cfgPort 配置端口
         * @param nodelist 返回的插件定义链
         * @return 插件定义字符串----主编辑插件链
         */
        QString service_getPluginlistdefAtCfgport(QString keyStr, PlgDef::ConfigPort::I_ConfigPort* cfgPort, CoreBase::PluginListNode **nodelist);

        /**
         * @brief 向指定配置端口写入插件定义链条
         * @param nodelist 处理链条定义
         * @param keyStr 索引关键键名
         * @param cfgPort 指定配置端口
         */
        void service_setPluginlistdefAtCfgport(CoreBase::PluginListNode *nodelist, QString keyStr, PlgDef::ConfigPort::I_ConfigPort* cfgPort);

    public slots:
        /**
         * @brief 接收软件产生的错误信息，并用弹框的方式提醒使用者，并记录
         * @param res 报告消息的插件指针
         * @param msg 具体消息内容
         * @param ignoreAllows 是否允许使用者忽略
         */
        void slot_Recieve_ProcessError(PlgDef::I_PluginBase *const res, const QString msg, bool ignoreAllows = false);

        /**
         * @brief 默认全平台保存操作
         */
        void slot_SaveOperation();



    private:
        const QString DefaultLogpath;
        const QString DefaultConfigpath;
        PluginManager *const manager;
        QApplication *const app;


        /**
         * @brief 载入所有插件
         */
        void operate_LoadAllPlugins();
        /**
         * @brief 初始化系统默认插件
         */
        void operate_InitDefaultPlugins();
        /**
         * @brief 核查环境信息
         */
        void service_EnvironmentCheck();

        /**
         * @brief 获取当前激活的窗口，屏幕最上方的那个
         * @return 窗口插件实例
         */
        PlgDef::Window::I_Window* getActivedWindow();

        enum ProcLocation{
            fileName,
            suffix,
            pjtCfg,
            g_suffix,
            g_Cfg,
        };
        /**
         * @brief 获得指定的文件的解析插件定义列表，包括插件组合方式和参数列表
         * @param frameConfig 软件默认主框架配置接口
         * @param projectConfig 文件项目配置接口
         * @param filePath 指定文件
         * @return 插件定义列表示例，记得删除该实例
         */
        CoreBase::PluginListNode* getAppointedPluginList(PlgDef::ConfigPort::I_ConfigPort *frameConfig,
                                               PlgDef::ConfigPort::I_ConfigPort *projectConfig,
                                               QString filePath);

        Core::CoreBase::PluginListNode* buildPluginList(QString keyExport,
                                              QString procdef,
                                              PlgDef::ConfigPort::I_ConfigPort* pjtCfg);
    private slots:
        //File
        void slot_OpenFileGloble();
        void slot_NewFileGloble(QAction* act);
        void slot_PrintGloble(QAction* act);
        void slot_CloseFileGloble(QAction* act);
        //Views
        void slot_ViewChanges(QAction* act);
        //Preference
        void slot_MainConfigPanel();
        void slot_PluginManager();
        void slot_ProjectConfig();
        void slot_DocumentConfig();
        //Help
        void slot_SoftwareAbout();
        void slot_HelpContent();
        void slot_ReportBugs();
        void slot_SoftwareUpdate();
    };
}




#endif // WSCORE_H
