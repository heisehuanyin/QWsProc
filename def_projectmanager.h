#ifndef DEF_PROJECTMANAGER_H
#define DEF_PROJECTMANAGER_H

#include "def_plugin_base.h"
#include "wscore.h"

#include <QAbstractItemModel>
#include <QStandardItemModel>

namespace PlgDef {
    namespace ProjectManager{

        class I_ProjectManager: public I_Configurable,public I_Combiantion
        {
            Q_OBJECT
        public:
            I_ProjectManager();
            virtual ~I_ProjectManager() override;

            /**
             * @brief 返回关联后缀名
             * @return 后缀名
             */
            virtual QString suffix() = 0;

            /**
             * @brief 获取一个空的文档内容，代表一个格式合法的空项目管理文件，
             * 在新建过程中此内容会被写入磁盘文件
             * @return 空文档内容
             */
            virtual QString getEmptyDocument() = 0;

            /**
             * @brief 打开已经存在的项目
             * @param core
             * @param upStream
             * @param xargs
             * @return
             */
            virtual I_ProjectManager* openExistProject(Core::WsCore *core, PlgDef::I_PluginBase* upStream, QHash<QString, QString> xargs) = 0;

            /**
             * @brief 返回本项目中所有可用的Group名称列表，不论内部是如何构造，都将翻译成为list<string>
             * @return 列表
             */
            virtual QList<QString> getAvaliableGroups() = 0;

            /**
             * @brief 传入需要插入的文件名称和指向group，string自动翻译成为内部数据结构，解析后返回实际的详细文件路径
             * 如果translateOnly==true,表示只执行路径转换操作，反之，同时在Manager内部添加项目条目
             * @param translateOnly 本次调用是否只用于路径转换
             * @param fileName 文件名称
             * @param groupName GroupName名称
             * @return 目标的实际路径,groupName非法则返回QString()
             */
            virtual QString translate_Insert(bool translateOnly, QString fileName, QString groupName) = 0;

            /**
             * @brief 获取TreeModel提供给QTreeView
             * @return model
             */
            virtual QAbstractItemModel* getTreeModel()=0;

            /**
             * @brief 根据QModelIndex获取上下文菜单（右键菜单）
             * @param index 索引
             * @return 菜单，用于弹出
             */
            virtual QMenu* getContextMenu(QModelIndex &index) = 0;

        public slots:
            /**
             * @brief 项目中关联文件如果打开和关闭，框架会向其所属项目管理单元报告文档状态，由其自行处理是否记录
             * @param filePath 文件路径
             */
            virtual void slot_FileIsOpening(QString filePath) = 0;
            virtual void slot_FileIsClosed(QString filePath) = 0;

            virtual void slot_doubleClicked(const QModelIndex &index) = 0;
            virtual void slot_clicked(const QModelIndex &index) = 0;

            // I_PluginBase interface
        public:
            virtual PluginType pluginMark() override final;
        };
    }
}

#endif // PROJECTMANAGER_H
