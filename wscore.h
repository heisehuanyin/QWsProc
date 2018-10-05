#ifndef WSCORE_H
#define WSCORE_H

#include "def_plugin_base.h"
#include <QMessageBox>

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
        void operate_BuildChannel(QString *const cStr, const QString cId);

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
        const QString * channel_getChannelId(PlgDef::I_PluginBase *pExample);

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
        PlgDef::ConfigPort::I_ConfigPort *instance_GetConfigport(const QString fPath);

        /**
         * @brief 获取一个Logport实例，每次程序启动，只保留最后装载的一种Logport插件
         * @param fPath Log文件路径
         * @return Logport插件实例
         */
        PlgDef::LogPort::I_LogPort *instance_GetLogport(const QString fPath);

        //UI Components
        /**
         * @brief 获取配置项设定的窗口实例一个
         * @param gId GroupId
         * @return 实例
         */
        PlgDef::I_PluginBase * instance_GetWindowInstance(const QString gId);
        PlgDef::I_PluginBase * instance_GetMenuBarInstance(const QString gId);

        //Query Methods
        /**
         * @brief 通过指定typeMark，查询所有同种插件列表
         * @param typeMark 插件类别码
         * @return 信息组合
         */
        QList<QPair<QString, PlgDef::PluginType>> * service_QueryFactoryList(PlgDef::PluginType typeMark);

        /**
         * @brief 通过指定插件注册名，查询所有同种插件列表
         * @param pRegistName 插件注册名
         * @return 信息组合
         */
        QList<QPair<QString, PlgDef::PluginType>> * service_QueryFactoryList(const QString pRegistName);

    private:
        QHash<QString, PlgDef::I_PluginBase *> *const factories;
        QList<PlgDef::I_PluginBase *> *const configunits;
        QHash<QString, QList<PlgDef::I_PluginBase *> *> *const instances;
        WsCore *const core;
        QString logportName;
        QString configportName;
        QList<QPair<QString, PlgDef::PluginType>> *const list;

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
        WsCore();
        ~WsCore();

        /**
         * @brief 获取插件管理器
         * @return 插件管理器实例
         */
        PluginManager* service_getManager();

        /**
         * @brief 注册插件工厂便捷接口
         * @param p 插件实例
         */
        void service_RegisterPlugin(PlgDef::I_PluginBase *p);

        /**
         * @brief 刷新指定窗口实例上的menubar
         * @param win 指定窗口实例
         */
        void service_RefreshMenuBar(PlgDef::Window::I_Window *win);

        /**
         * @brief 默认全平台保存操作
         */
        void service_SaveOperation();

        /**
         * @brief 打开特定文件，如果通道中插件有ContentView类型，会自动加载到本窗口上
         * @param fileIndicate 文件指代符
         * @param win 发出请求的窗口
         */
        void service_OpenFile(const QString filePath, PlgDef::I_PluginBase *win);

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
         * @brief 打开图形模式，所有操作可以通过图形界面进行操作
         * @param groupId 插件实例注册id
         */
        void service_OpenGraphicsModel(const QString groupId);

        void test_InnerTest();

    public slots:
        void slot_Recieve_ProcessError(PlgDef::I_PluginBase *const res, const QString msg);

    private:
        void operate_LoadAllPlugins();
        void operate_InitDefaultPlugins();

        const QString DefaultLogpath;
        const QString DefaultConfigpath;
        PluginManager *const manager;
    };
}




#endif // WSCORE_H
