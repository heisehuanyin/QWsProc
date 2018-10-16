#ifndef DEFAULTTEXTMODEL_H
#define DEFAULTTEXTMODEL_H

#include "def_textmodel.h"

#include <QFile>

namespace PlgDef {
    namespace TextModel {
        class DefaultTextModel:public I_TextModel
        {
            Q_OBJECT
        public:
            DefaultTextModel();
            virtual ~DefaultTextModel() override {
                delete this->file;
            }

            // I_PluginBase interface
        public:
            virtual const QString registName() override {
                return "DefaultTextModel";
            }
            virtual PluginType upStreamMark() override{
                return PlgDef::IO_ChannelPreface;
            }
            virtual QMenu *getCustomMenu() override;
            virtual void saveOperation() override;

            // I_TextModel interface
        public:
            virtual I_TextModel *openNewTextModel(Core::WsCore *core, I_PluginBase *upStream, QHash<QString, QString> xargs) override;
            virtual int getRowsCount() override;
            virtual QString getLineContent(int row) override;
            virtual void updateLineContent(int row, const QString content) override;
            virtual void insertLineAt(int row, const QString content) override;
            virtual void removeLines(int begin, int offset = -1) override;

        private:
            QList<QString> contents;
            Core::WsCore * core;
            QTextCodec *codec;
            QFile *file;

            // I_TextModel interface
        public:
            virtual void reLoadAllText(QString text) override;
        };
    }
}

#endif // DEFAULTTEXTMODEL_H
