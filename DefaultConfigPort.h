#ifndef DEFAULTCONFIGPORT_H
#define DEFAULTCONFIGPORT_H

#include "def_configport.h"

#include <QtXml>

namespace PlgDef {
    namespace ConfigPort {
        class DefaultConfigPort : public I_ConfigPort
        {
        public:
            DefaultConfigPort();
            virtual ~DefaultConfigPort() override{
                delete this->error;
                this->file->flush();
                this->file->close();
                delete this->doc;
                delete this->file;
                delete this->pluginName;
            }

            // I_PluginBase interface
        public:
            virtual const QString *registName() override;
            virtual PluginType upStreamMark() override;
            virtual QMenu *getCustomMenu() override;
            virtual void saveOperation() override;

            // I_ConfigPort interface
        public:
            virtual I_ConfigPort *createNewPort(QString * const fPath) override;
            virtual void setKeyValue(QString * const key, QString * const value) override;
            virtual const QString *getValue(QString * const key, QString * const defaultValue) override;

        private:
            QDomDocument * doc;
            const QString *const pluginName;
            QFile * file;
            QString * error;
            const QString * value_temp;

            QDomElement searchElementAsDescription(QDomElement * elm, QString *tagName);
        };
    }
}

#endif // DEFAULTCONFIGPORT_H
