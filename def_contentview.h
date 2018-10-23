#ifndef DEF_CONTENTVIEW_H
#define DEF_CONTENTVIEW_H

#include "def_plugin_base.h"
#include "wscore.h"



namespace PlgDef {
    namespace Window {
        class I_Window;
    }

    /**
    @brief 内容面板命名空间*/
    namespace ContentView {
        /**
        @brief 内容面板*/
        class I_ContentView: public I_Configurable,public I_Combiantion
        {
            Q_OBJECT
        public:
            I_ContentView();
            virtual ~I_ContentView() override;

            /**
             * @brief 创建一个新的内容视图实例
             * @param core 调度核心，提供交互接口
             * @param upStream 上游模块，提供内容
             * @param xargs 参数列表，如果提供的话
             * @return 新实例
             */
            virtual I_ContentView* createNewContentView(Core::WsCore *core, PlgDef::I_PluginBase *upStream,
                                                        QHash<QString,QString> xargs, Window::I_Window* win)=0;

            virtual QWidget* getWidget() = 0;
            virtual Window::I_Window* getOwner()=0;



            // I_PluginBase interface
        public:
            virtual PlgDef::PluginType pluginMark() override final;
        };
    }
}

#endif // DEFCONTENTVIEW_H
