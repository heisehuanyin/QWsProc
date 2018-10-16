#ifndef DEFAULTMENUBAR_H
#define DEFAULTMENUBAR_H

#include "def_menubar.h"

namespace PlgDef {
    namespace MenuBar{

        class DefaultMenuBar:public I_MenuBar
        {
        public:
            DefaultMenuBar();
            virtual ~DefaultMenuBar() override;

            // I_PluginBase interface
        public:
            virtual const QString registName() override;
            virtual QMenu *getCustomMenu() override;
            virtual void saveOperation() override;

            // I_MenuBar interface
        public:
            virtual I_MenuBar *createNewMenubar(Core::WsCore *core, QString groupId) override;
            virtual QMenuBar *refreshMenuBarWidget(QList<QMenu *> vlist) override;

        private:
            QMenuBar *mbar;
            QString groupid;
        };
    }
}

#endif // DEFAULTMENUBAR_H
