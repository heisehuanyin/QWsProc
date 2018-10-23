#ifndef WSNOVELMANAGER_H
#define WSNOVELMANAGER_H

#include "def_projectmanager.h"

#include <QDomDocument>

namespace PlgDef {
    namespace ProjectManager {
        namespace WsNovelBase {
            class NodeWithDomElement:public QObject, public QStandardItem
            {
                Q_OBJECT
            public:
                explicit NodeWithDomElement(QDomDocument *doc, QDomElement elm);

                virtual ~NodeWithDomElement();

                QString getNodeName();
                QDomDocument* getDocument();
                QDomElement getElement();

            private:
                QDomDocument *doc;
                QDomElement elm;
            };
        }

        class WsNovelProjectManager:public I_ProjectManager
        {
            Q_OBJECT
        public:
            WsNovelProjectManager();
            virtual ~WsNovelProjectManager() override;

        private:
            QMenu *const customMenu;
            QDomDocument * doc;
            PlgDef::ChannelPreface::I_ChannelPreface* upStream;
            Core::WsCore* core;
            QStandardItemModel*const model;

            void parseDocForDetail(QDomElement elm, QStandardItem* item);
        private slots:
            virtual void slot_NodeIsRemoved(const QModelIndex &parent, int first, int last);
            virtual void slot_NodeIsInserted(const QModelIndex &parent, int first, int last);
            virtual void slot_NodeIsMoved(const QModelIndex &parent, int start, int end, const QModelIndex &destination, int row);
            virtual void slot_NodeIsModified(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles = QVector<int> ());

            // I_PluginBase interface
        public:
            virtual const QString registName() override;
            virtual QMenu *getCustomMenu() override;
            virtual void saveOperation() override;

            // I_Combiantion interface
        public:
            virtual PluginType upStreamMark() override;
            virtual QWidget *reloadContentAndReturnSetupWidget() override;

            // I_ProjectManager interface
        public:
            virtual QString suffix() override;
            virtual I_ProjectManager *openExistProject(Core::WsCore *core, I_PluginBase *upStream, QHash<QString, QString> xargs) override;
            virtual QString getEmptyDocument() override;
            virtual QList<QString> getAvaliableGroups() override;
            virtual QString translate_Insert(bool transOnly, QString fileName, QString groupName) override;
            virtual QAbstractItemModel *getTreeModel() override;
            virtual QMenu *getContextMenu(QModelIndex &index) override;

        public slots:
            virtual void slot_FileIsOpening(QString filePath) override;
            virtual void slot_FileIsClosed(QString filePath) override;
            virtual void slot_doubleClicked(const QModelIndex &index) override;
            virtual void slot_clicked(const QModelIndex &index) override;

        };

    }
}

#endif // DEFAULTPROJECTMANAGER_H
