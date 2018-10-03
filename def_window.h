#ifndef DEFWINDOW_H
#define DEFWINDOW_H

#include "def_contentview.h"
#include "def_menubar.h"
#include "WsCore.h"

#include <QString>


namespace PlgDef {

    /**
    @brief 窗口命名空间*/
    namespace Window {
        /**
        @brief 窗口*/
        class I_Window: public I_PluginBase{
        public:
            I_Window(){}
            virtual ~I_Window(){}
            /**
             * @brief 创建一个新的Window实例,与原本的实例并无任何联系
             * @param core 核心调度模块
             * @param gid GroupId
             * @return 新实例
             */
            virtual I_Window* openNewWindow(Core::WsCore *core, QString* gid) = 0;

            /**
             * @brief 获取本实例绑定的GroupId
             * @return GroupId
             */
            virtual QString* getGroupID() = 0;

            /**
             * @brief 设置窗口标题
             * @param title 标题
             */
            virtual void setTitle(QString *title) = 0;

            /**
             * @brief 将ContentView放置在本窗体上
             * @param viewTitle 视图标题
             * @param comp 视图组件
             */
            virtual void placeView(QString *viewTitle, ContentView::I_ContentView *comp) = 0;

            /**
             * @brief 关闭指定ContentView
             * @param comp 指定组件
             */
            virtual void closeContentView(ContentView::I_ContentView *comp) = 0;

            /**
             * @brief 利用传入的MenuBar刷新本窗口的菜单栏及其组件
             * @param bar 菜单栏组件
             */
            virtual void service_RefreshMenuBar(MenuBar::I_MenuBar *bar) = 0;

            /**
             * @brief 获取本窗口中的活动视图
             * @param views 存取视图容器
             */
            virtual QList<ContentView::I_ContentView *> * getActivedView() = 0;
        };
    }

}


#endif // DEFWINDOW_H
