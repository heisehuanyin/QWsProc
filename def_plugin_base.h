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

        IO_NoUpStream,
        IO_PluginSymbo,
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
    class I_PluginBase{
    public:
        I_PluginBase(){}
        virtual ~I_PluginBase(){}

        /**
         * @brief 插件注册名称，不可重复
         * @return 插件注册名称
         */
        virtual QString* registName() = 0;

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
         * @return 插件菜单
         */
        virtual QMenu* getCustomMenu() = 0;

        /**
         * @brief 插件自身保存操作
         */
        virtual void saveOperation() = 0;
    };

    class I_PluginEvent {
    public:
        virtual ~I_PluginEvent();

        virtual I_PluginBase* getSource()=0;
        virtual QString* getMsg()=0;
    };

    class I_FileSymbo{
    public:
        virtual ~I_FileSymbo();

        virtual QString* getFilePath() = 0;
        virtual QString* getSimpleFileName() = 0;
    };

    class I_PluginException:std::exception{
    private:
        QString *const reason;


    public:
        I_PluginException(QString &reason):
            reason(&reason)
        {

        }
        virtual ~I_PluginException();



        // exception interface
    public:
        virtual const char *what(){

        }
    };
}

#endif // DEFPLUGIN_BASE_H
