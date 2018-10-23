#ifndef PLAINTEXTVIEW_H
#define PLAINTEXTVIEW_H

#include "def_contentview.h"

#include <QTextEdit>

namespace PlgDef {
    namespace ContentView {
        class PlainTextView:public I_ContentView
        {
            Q_OBJECT
        public:
            PlainTextView();
            virtual ~PlainTextView() override;



            // I_PluginBase interface
        public:
            virtual const QString registName() override {
                return "PlainTextView";
            }
            virtual QMenu *getCustomMenu() override;
            virtual void saveOperation() override;

            // I_ContentView interface
        public:
            virtual I_ContentView *createNewContentView(Core::WsCore *core, I_PluginBase *upStream,
                                                        QHash<QString, QString> xargs, PlgDef::Window::I_Window* win) override;
            virtual QWidget *getWidget() override;
            virtual Window::I_Window *getOwner() override;

        private:
            Window::I_Window* owner;
            Core::WsCore *core;
            PlgDef::TextModel::I_TextModel *textmodel;
            QTextEdit *const editArea;


            // I_CombinaUnit interface
        public:
            virtual PluginType upStreamMark() override;
            virtual QWidget *reloadContentAndReturnSetupWidget() override;

        };
    }
}

#endif // PLAINTEXTVIEW_H
