#ifndef SINGALVIEW_H
#define SINGALVIEW_H

#include "def_window.h"

#include <QMainWindow>


namespace PlgDef {
    namespace Window {

        class SingalView : public PlgDef::Window::I_Window
        {
        private:
            QString *gid;
            Core::WsCore * core;
            PlgDef::ContentView::I_ContentView * centralView;
            QString *const plgName;
            QMainWindow *const window;
            QList<PlgDef::ContentView::I_ContentView *> *const list;

        public:
            SingalView();
            ~SingalView();

            // I_PluginBase interface
        public:
            QString *registName();
            PlgDef::PluginType pluginMark();
            PlgDef::PluginType upStreamMark();
            QMenu *getCustomMenu();
            void saveOperation();

            // I_Window interface
        public:
            PlgDef::Window::I_Window *openNewWindow(Core::WsCore *core, QString *gid);
            QString *getGroupID();
            void setTitle(QString *title);
            void placeView(QString *viewTitle, PlgDef::ContentView::I_ContentView *comp);
            void closeContentView(PlgDef::ContentView::I_ContentView *comp);
            void service_RefreshMenuBar(PlgDef::MenuBar::I_MenuBar *bar);
            QList<PlgDef::ContentView::I_ContentView *> * getActivedView();

        };

    }
}
#endif
