#ifndef SINGALVIEW_H
#define SINGALVIEW_H

#include "def_window.h"

#include <QMainWindow>
#include <QResizeEvent>
#include <QStackedLayout>


namespace PlgDef {
    namespace Window {

        class DefaultSingalView : public PlgDef::Window::I_Window
        {
            Q_OBJECT
        public:
            DefaultSingalView();
            virtual ~DefaultSingalView() override;

            // I_PluginBase interface
        public:
            const QString registName() override;
            QMenu *getCustomMenu() override;
            void saveOperation() override;

            // I_Window interface
        public:
            virtual PlgDef::Window::I_Window *openNewWindow(Core::WsCore *core, const QString gid, int width, int height) override;
            virtual const QString getGroupID() override;
            virtual void replaceMenuBar(QMenuBar *bar) override;
            virtual void setTitle(const QString title) override;
            virtual void appendView(const QString viewTitle, PlgDef::ContentView::I_ContentView *comp) override;
            virtual void removeView(ContentView::I_ContentView *cmop) override;
            virtual void FocuseAt(ContentView::I_ContentView *comp) override;
            virtual QList<PlgDef::ContentView::I_ContentView *> getActivedView() override;
            virtual QList<ContentView::I_ContentView *> getAllView() override;
            virtual void setSize(int width, int height) override;
            virtual QWidget *getWidget() override;

        private:
            QString gid;
            Core::WsCore * core;
            const QString plgName;
            _CustomWindow *const window;
            QHash<PlgDef::ContentView::I_ContentView *, QString> *const list;
            QStackedLayout *const stack;
            QWidget*const basePanel;

        private slots:
            void receiveWindowResize(int width, int height){
                emit this->signal_resizeWindow(this->gid, width, height);
            }


        };

    }
}
#endif
