#ifndef CUSTOMDIALOG_H
#define CUSTOMDIALOG_H

#include "def_configport.h"

#include <QCheckBox>
#include <QDialog>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QListView>
#include <QPushButton>
#include <QTabWidget>
#include <QTableView>

namespace Core {
    namespace CustomBase {
        class FileParsePanel : public QVBoxLayout{
            Q_OBJECT
        public:
            FileParsePanel(Core::WsCore *core, PlgDef::ConfigPort::I_ConfigPort *cfg_port);
            virtual ~FileParsePanel() override;

            /**
             * @brief 配置检索项目，设置配置目标为传入数据
             * @param fullFilePath 文件完全路径
             */
            void config4File(QString fullFilePath);
            void config4Suffix(QString suffix);

        private:
            PlgDef::ConfigPort::I_ConfigPort *const _port;
            Core::WsCore *const core;
            QCheckBox *const isSuffix;
            QLineEdit *const input;
            QPushButton *const click;
            QTableView *const branches;
            QPushButton *const addBranchMod;
            QPushButton *const addViewBranch;
            QPushButton *const removeBranch;
            QListView *const arguments;

        private slots:
            void cfgCheck(int state);

            void setBranchTable();
            void setArgsList();

            void addBranchModule();
            void addViewBranchM();
            void removeBranchNext();
        };

        class FPP_BranchesDataModel: public QAbstractTableModel{


            // QAbstractItemModel interface
        public:
            virtual int rowCount(const QModelIndex &parent) const override;
            virtual int columnCount(const QModelIndex &parent) const override;
            virtual QVariant data(const QModelIndex &index, int role) const override;
            virtual bool setData(const QModelIndex &index, const QVariant &value, int role) override;
            virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
            virtual bool insertRows(int row, int count, const QModelIndex &parent) override;
            virtual bool removeRows(int row, int count, const QModelIndex &parent) override;
            virtual Qt::ItemFlags flags(const QModelIndex &index) const override;
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

    private slots:
        void defaultWindowType(const QString type);
        void defaultMenuBarType(const QString type);
        void defaultToolBarType(const QString type);
        void defaultStatusBarType(const QString type);
        void defaultFloatPanelType(const QString type);

        CustomBase::FileParsePanel * getFileParsePanel(PlgDef::ConfigPort::I_ConfigPort *cfg_port);
    };

}


#endif // CUSTOMDIALOG_H
