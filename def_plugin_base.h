#ifndef DEFPLUGIN_BASE_H
#define DEFPLUGIN_BASE_H

#include <QMenu>

namespace PlgDef{
    enum PluginType{
        UI_MenuBar,
        UI_ToolBar,
        UI_StateBar,
        UI_Window,
        UI_ContentView,
        UI_FloatPanel,

        IO_NoUpStream,
        IO_ChannelPreface,
        IO_TextModel,
        IO_TreeModel,
        IO_TableModel,
        IO_StyleModel,

        Service_ConfigPort,
        Service_LogPort
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
         * @brief 上游插件的类别
         * @return 插件类别标志
         */
        virtual PlgDef::PluginType upStreamMark() = 0;

        /**
         * @brief 获取插件自定义的菜单
         * @return 插件菜单,如果插件不提供配置菜单，返回nullptr
         */
        virtual QMenu* getCustomMenu() = 0;

        /**
         * @brief 插件自身保存操作
         */
        virtual void saveOperation() = 0;

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

    signals:
        /**
         * @brief 所有插件共有的信号，用于报告插件内部错误
         * @param resp 发生问题的插件
         * @param msg 错误相关信息
         */
        void signal_PushErrorReport(PlgDef::I_PluginBase *const resp, const QString msg, bool ignoreAllows = false);
    };
}

#endif // DEFPLUGIN_BASE_H
