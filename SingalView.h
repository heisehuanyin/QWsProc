#ifndef SINGALVIEW_H
#define SINGALVIEW_H

#include "def_window.h"

#include <QMainWindow>
#include <QResizeEvent>


namespace PlgDef {
    namespace Window {

        class DefaultSingalView : public PlgDef::Window::I_Window
        {
        private:
            QString gid;
            Core::WsCore * core;
            PlgDef::ContentView::I_ContentView * centralView;
            const QString plgName;
            _CustomWindow *const window;
            QList<PlgDef::ContentView::I_ContentView *> *const list;

        public:
            DefaultSingalView();
            ~DefaultSingalView() override;

            // I_PluginBase interface
        public:
            const QString registName() override;
            PlgDef::PluginType upStreamMark() override;
            QMenu *getCustomMenu() override;
            void saveOperation() override;

            // I_Window interface
        public:
            PlgDef::Window::I_Window *openNewWindow(Core::WsCore *core, const QString gid) override;
            const QString getGroupID() override;
            void setTitle(const QString title) override;
            void placeView(const QString viewTitle, PlgDef::ContentView::I_ContentView *comp) override;
            void closeContentView(PlgDef::ContentView::I_ContentView *comp) override;
            void service_RefreshMenuBar(PlgDef::MenuBar::I_MenuBar *bar) override;
            QList<PlgDef::ContentView::I_ContentView *> * getActivedView() override;

        };

    }
}
#endif
