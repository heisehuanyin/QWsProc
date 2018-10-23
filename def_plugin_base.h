#ifndef DEF_PLUGIN_BASE_H
#define DEF_PLUGIN_BASE_H

#include <QMenu>

namespace PlgDef{
    enum PluginType{
        Feature_NoCombination = 0x00000000,
        Feature_Combination   = 0x00000001,
        Feature_Configurable  = 0x00000010,

        UI_MenuBar    = 0x10010000 | Feature_Configurable,
        UI_ToolBar    = 0x10020000 | Feature_Configurable,
        UI_StateBar   = 0x10030000 | Feature_Configurable,
        UI_Window     = 0x10040000,
        UI_ContentView= 0x10050000 | Feature_Configurable | Feature_Combination,
        UI_FloatPanel = 0x10060000,

        IO_ChannelPreface       = 0x20010000,
        IO_TextModel            = 0x20020000 | Feature_Combination | Feature_Configurable,
        IO_TreeModel            = 0x20030000 | Feature_Combination | Feature_Configurable,
        IO_TableModel           = 0x20040000 | Feature_Combination | Feature_Configurable,
        IO_StyleModel           = 0x20050000 | Feature_Combination | Feature_Configurable,
        Service_ProjectManager  = 0x20060000 | Feature_Combination | Feature_Configurable,

        Service_ConfigPort = 0x30010000 | Feature_Configurable,
        Service_LogPort    = 0x30020000 | Feature_Configurable,

    };

    /**
    @brief 所有插件的共同基类
    */
    class I_PluginBase : public QObject
    {
        Q_OBJECT

    public:
        I_PluginBase();
        virtual ~I_PluginBase();

        /**
         * @brief 插件注册名称，不可重复
         * @return 插件注册名称
         */
        virtual const QString registName() = 0;

        /**
         * @brief 获取插件类别
         * @return 插件类别标志
         */
        virtual PlgDef::PluginType pluginMark() = 0;

        /**
         * @brief 获取插件自定义的菜单
         * @return 插件菜单,如果插件不提供配置菜单，返回nullptr
         */
        virtual QMenu* getCustomMenu() = 0;

        /**
         * @brief 插件自身保存操作
         */
        virtual void saveOperation() = 0;

    signals:
        /**
         * @brief 所有插件共有的信号，用于报告插件内部错误
         * @param resp 发生问题的插件
         * @param msg 错误相关信息
         */
        void signal_PushErrorReport(PlgDef::I_PluginBase *const resp, const QString msg, bool ignoreAllows = false);
    };

    class I_Combiantion{
    public:
        I_Combiantion();
        virtual ~I_Combiantion();

        virtual PlgDef::PluginType upStreamMark() = 0;

        /**
         * @brief 重新载入上游插件内容，返回设置面板，用于设置插件状态。典型应用新建过程
         * 如果返回nullptr,则代表着放弃配置过程
         * 通过面板设置的状态都是临时状态，持久化状态需要通过获取ConfigPort来存储。
         * ConfigPort可以存储初始化参数，也可以存储插件自定义参数。
         * @return 配置面板
         */
        virtual QWidget* reloadContentAndReturnSetupWidget() = 0;
    };

    class I_Configurable:public I_PluginBase
    {
    public:
        I_Configurable();
        virtual ~I_Configurable();

        /**
         * @brief 获取插件参数列表<argsName, <key, val>>,key=["type","1","2","3",...],二级列表中type的值直接关联ArgsType类型
         * @return 返回的插件参数列表
         */
        QHash<QString, QHash<QString, QString>> getArgsList();

        enum ArgsType{
            ENUM_ITEM,
            INPUT,
        };

    protected:
        /**
         * @brief 定义一个参数，同时定义参数类型，参数类型定义见I_PluginBase::ArgsType
         * @param argsName 参数名
         * @param typeItem 参数类型
         */
        void defineArgs(const QString argsName, ArgsType typeItem);

        /**
         * @brief 向指定枚举选项参数中添加枚举项目，若该参数已经定义且本非EnumItem类型，则报错，否则添加入枚举项目
         * @param argsName 参数名
         * @param enumValue 可选枚举项目值
         */
        void insertEnumItem(const QString argsName, const QString enumValue);

    private:
        QHash<QString, QHash<QString, QString>> argsList;
    };
}

#endif // DEFPLUGIN_BASE_H
