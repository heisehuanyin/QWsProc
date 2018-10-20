#ifndef CUSTOMDIALOG_H
#define CUSTOMDIALOG_H

#include "def_configport.h"

#include <QCheckBox>
#include <QComboBox>
#include <QDialog>
#include <QHBoxLayout>
#include <QItemDelegate>
#include <QLineEdit>
#include <QListView>
#include <QPushButton>
#include <QTabWidget>
#include <QTableView>

namespace Core {
    namespace CoreBase {
        class FPP_TableDataModel;
        class FPP_ListDataModel;

        class FileParsePanel : public QVBoxLayout{
            Q_OBJECT
        public:
            FileParsePanel(Core::WsCore *core, PlgDef::ConfigPort::I_ConfigPort *cfg_port);
            virtual ~FileParsePanel() override;

            /**
             * @brief 配置检索项目，设置配置目标为传入数据
             * @param fullFilePath 文件完全路径
             */
            void config4Keywords(QString keywords);

        private:
            PlgDef::ConfigPort::I_ConfigPort *const _port;
            Core::WsCore *const core;
            QComboBox *const queryType;
            QLineEdit *const input;
            QPushButton *const click;
            QTableView *const modules;
            QLabel *const msg;
            QListView *const argslist;
            FPP_TableDataModel * tableModel;
            FPP_ListDataModel *listModel;


        private slots:
            void queryTypeChanged(int index);
            void click_Query();

            void currentChanged(const QModelIndex &current, const QModelIndex &previous);
        };

        class FPP_TableDataModel: public QAbstractTableModel{
            Q_OBJECT

        public:
            FPP_TableDataModel(Core::WsCore *core,
                               QString keyString,
                               CoreBase::PluginListNode* nodelist,
                               PlgDef::ConfigPort::I_ConfigPort* port);
            virtual ~FPP_TableDataModel() override;

            virtual PluginListNode* getInnerNode(int row) const;

            // QAbstractItemModel interface
        public:
            virtual int rowCount(const QModelIndex &parent) const override;
            virtual int columnCount(const QModelIndex &parent) const override;
            virtual QVariant data(const QModelIndex &index, int role) const override;
            virtual bool setData(const QModelIndex &index, const QVariant &value, int role) override;
            virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
            virtual Qt::ItemFlags flags(const QModelIndex &index) const override;

        private:
            QList<PluginListNode*> list;
            Core::WsCore *const core;
            QString keyString;
            PlgDef::ConfigPort::I_ConfigPort *const _port;

            // QAbstractItemModel interface
        public:
            virtual bool insertRows(int row, int count, const QModelIndex &parent) override;
            virtual bool removeRows(int row, int count, const QModelIndex &parent) override;
        };

        class FPP_TableDelegate:public QItemDelegate
        {
            Q_OBJECT
        public:
            FPP_TableDelegate(Core::WsCore *core);
            virtual ~FPP_TableDelegate() override;


            // QAbstractItemDelegate interface
        public:
            virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
            virtual void setEditorData(QWidget *editor, const QModelIndex &index) const override;
            virtual void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
            virtual void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

        private:
            Core::WsCore *const core;
        };

        class FPP_ListDataModel: public QAbstractListModel
        {
            Q_OBJECT
        public:
            FPP_ListDataModel(PluginListNode* target, PlgDef::I_Configurable *factory);
            virtual ~FPP_ListDataModel() override;

            // QAbstractItemModel interface
        public:
            virtual int rowCount(const QModelIndex &parent) const override;
            virtual QVariant data(const QModelIndex &index, int role) const override;
            virtual bool setData(const QModelIndex &index, const QVariant &value, int role) override;
            virtual Qt::ItemFlags flags(const QModelIndex &index) const override;

        private:
            PluginListNode *const target;
            PlgDef::I_Configurable *factory;
        };

        class FPP_ListDelegate:public QItemDelegate{
            Q_OBJECT
        public:
            FPP_ListDelegate();
        };
    }


    class CustomDialog : public QDialog
    {
        Q_OBJECT

    public:
        explicit CustomDialog(Core::WsCore *core, PlgDef::ConfigPort::I_ConfigPort* mainCfg, QWidget *parent = nullptr);
        ~CustomDialog();

        /**
         * @brief 添加项目端口配置界面，如果指定项目文件完全路径，则直接设定好检索条件
         * @param customTarget 目标配置端口
         * @param title 标题
         * @param filePathAtPjt 文件完全路径
         */
        void setFileParserCustomPane(PlgDef::ConfigPort::I_ConfigPort * customTarget, const QString title, QString filePathAtPjt=QString());


    private:
        PlgDef::ConfigPort::I_ConfigPort * const mainCfg;
        QHBoxLayout*const layout;
        QTabWidget*const tab;
        QWidget*const uicfg;
        QWidget*const g_cfg;
        Core::WsCore *const core;
        QLineEdit *const defaultpath;
        QPushButton *const explor;

    private slots:
        void defaultWindowType(const QString type);
        void defaultMenuBarType(const QString type);
        void defaultToolBarType(const QString type);
        void defaultStatusBarType(const QString type);
        void defaultFloatPanelType(const QString type);

        CoreBase::FileParsePanel * getFileParsePanel(PlgDef::ConfigPort::I_ConfigPort *cfg_port);
    };

}


#endif // CUSTOMDIALOG_H
