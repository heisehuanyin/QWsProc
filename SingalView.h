#ifndef SINGALVIEW_H
#define SINGALVIEW_H

#include "windowdef.h"

#include <QMainWindow>


namespace PlgDef {
    namespace Window {

        class SingalView : public PlgDef::Window::I_Window
        {
        private:
            QString *plgName;
            QString *gid;
            Core::WsCore* core;
            QMainWindow* window;
            PlgDef::ContentView::I_ContentView* centralView;

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
            void getActivedView(QList<PlgDef::ContentView::I_ContentView*> *views);

        };

    }
}
#endif
