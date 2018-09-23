#ifndef PLUGINDEF_H
#define PLUGINDEF_H

#include "plugindef_base.h"
#include "wscore.h"

#include <QMenu>

/**
@brief 插件统一定义命名空间
*/
namespace PlgDef{
    /**
    @brief 内容面板命名空间*/
    namespace ContentView {
        /**
        @brief 内容面板*/
        class I_ContentView:I_PluginBase{

        };
    }
    /**
    @brief 菜单栏命名空间*/
    namespace MenuBar{
        /**
        @brief 菜单栏*/
        class I_MenuBar:I_PluginBase{};
    }

    /**
    @brief 窗口命名空间*/
    namespace Window {
        /**
        @brief 窗口*/
        class I_Window:I_PluginBase{
        public:
            virtual ~I_Window();

            virtual I_Window* openNewWindow(Core::WsCore *core) = 0;
            virtual QString* getGroupID() = 0;
            virtual void setTitle(QString *title) = 0;
            virtual void placeView(QString *viewTitle, ContentView::I_ContentView *comp) = 0;
            virtual void closeContentView(ContentView::I_ContentView *comp) = 0;
            virtual void service_ResetMenuBar(MenuBar::I_MenuBar *bar) = 0;
            virtual void getActivedView(QList<ContentView::I_ContentView> *views) = 0;
        };
    }


}

#endif // PLUGINDEF_H
