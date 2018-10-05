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
                this->file->flush();
                this->file->close();
                delete this->doc;
                delete this->file;
            }

            // I_PluginBase interface
        public:
            virtual const QString registName() override;
            virtual PluginType upStreamMark() override;
            virtual QMenu *getCustomMenu() override;
            virtual void saveOperation() override;

            // I_ConfigPort interface
        public:
            virtual I_ConfigPort *createNewPort(const QString fPath) override;
            virtual void setKeyValue(const QString key, const QString value) override;
            virtual const QString getValue(const QString key, const QString defaultValue) override;

        private:
            QDomDocument * doc;
            const QString pluginName;
            QFile * file;

            QDomElement searchElementAsDescription(QDomElement * elm, const QString tagName);
        };
    }
}

#endif // DEFAULTCONFIGPORT_H
