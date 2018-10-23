#ifndef PROJECTVIEW_H
#define PROJECTVIEW_H
#include "def_contentview.h"

#include <QTreeView>

namespace PlgDef {
    namespace ProjectManager {
        class I_ProjectManager;
    }

    namespace ContentView {

        class DefaultProjectView:public I_ContentView
        {
            Q_OBJECT
        public:
            DefaultProjectView();
            virtual ~DefaultProjectView() override;

        private:
            QTreeView *view;
            Window::I_Window *win;
            ProjectManager::I_ProjectManager* upStream;
            Core::WsCore *core;


            // I_PluginBase interface
        public:
            virtual const QString registName() override;
            virtual QMenu *getCustomMenu() override;
            virtual void saveOperation() override;

            // I_Combiantion interface
        public:
            virtual PluginType upStreamMark() override;
            virtual QWidget *reloadContentAndReturnSetupWidget() override;

            // I_ContentView interface
        public:
            virtual I_ContentView *createNewContentView(Core::WsCore *core,
                                                        I_PluginBase *upStream,
                                                        QHash<QString, QString> xargs,
                                                        Window::I_Window *win) override;
            virtual QWidget *getWidget() override;
            virtual Window::I_Window *getOwner() override;
        };
    }
}

#endif // PROJECTVIEW_H
