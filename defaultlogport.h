#ifndef DEFAULTLOGPORT_H
#define DEFAULTLOGPORT_H

#include "def_logport.h"

#include <QFile>
#include <QTextStream>

namespace PlgDef {
    namespace LogPort {
        class DefaultLogPort : public I_LogPort
        {
            Q_OBJECT
        private:
            const QString pluginName;
            QFile * logPort;
            QTextStream * logStream;
            Core::WsCore *core;

        public:
            DefaultLogPort();
            virtual ~DefaultLogPort() override;

            // I_PluginBase interface
        public:
            virtual const QString registName() override;
            virtual QMenu *getCustomMenu() override;
            virtual void saveOperation() override;

            // I_LogPort interface
        public:
            virtual I_LogPort *createNewPort(Core::WsCore *core, const QString fPath,
                                             QHash<QString, QString> argslist) override;
            virtual void writeLog(I_PluginBase *p, const QString msg) override;
            virtual void errorLog(I_PluginBase *p, const QString msg) override;
            virtual void echoLog(I_PluginBase *p, const QString msg) override;
        };
    }
}

#endif // DEFAULTLOGPORT_H
