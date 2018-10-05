#ifndef DEFAULTLOGPORT_H
#define DEFAULTLOGPORT_H
#include "def_logport.h"
#include <QFile>
#include <QTextStream>

namespace PlgDef {
    namespace LogPort {
        class DefaultLogPort : public I_LogPort
        {
        private:
            QString *const pluginName;
            QFile * logPort;
            QTextStream * logStream;
            QString * error;

        public:
            DefaultLogPort();
            virtual ~DefaultLogPort() override;

            // I_PluginBase interface
        public:
            virtual const QString *registName() override;
            virtual PluginType upStreamMark() override;
            virtual QMenu *getCustomMenu() override;
            virtual void saveOperation() override;

            // I_LogPort interface
        public:
            virtual I_LogPort *createNewPort(QString * const fPath) override;
            virtual void writeLog(I_PluginBase *p, QString * const msg) override;
            virtual void errorLog(I_PluginBase *p, QString * const msg) override;
            virtual void echoLog(I_PluginBase *p, QString * const msg) override;
        };
    }
}

#endif // DEFAULTLOGPORT_H
