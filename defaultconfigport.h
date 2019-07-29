#ifndef DEFAULTCONFIGPORT_H
#define DEFAULTCONFIGPORT_H

#include "def_configport.h"

#include <QtXml>

namespace PlgDef {
    namespace ConfigPort {
        class DefaultConfigPort : public I_ConfigPort
        {
            Q_OBJECT
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
            virtual const QString registName() override{
                return "DefaultConfigPort";
            }
            virtual QMenu *getCustomMenu() override;
            virtual void saveOperation() override;

            // I_ConfigPort interface
        public:
            virtual I_ConfigPort *createNewPort(Core::WsCore *core, const QString fPath,
                                                QHash<QString, QString> argslist) override;
            virtual void setKeyValue(const QString key, const QString value) override;
            virtual const QString getValue(const QString key, const QString defaultValue) override;

        private:
            QDomDocument * doc;
            QFile * file;

            QDomElement searchElementAsDescription(QDomElement * elm, const QString tagName);

            // I_ConfigPort interface
        public:
            virtual const QHash<QString, QString> getConfigList(const QString key) override;

        public slots:
            virtual void setConfigList(const QString key, QHash<QString, QString> list) override;
        };
    }
}

#endif // DEFAULTCONFIGPORT_H
