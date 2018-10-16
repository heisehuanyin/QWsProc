#ifndef PLUGINLISTNODE_H
#define PLUGINLISTNODE_H

#include <QHash>
#include <QString>

namespace Core {
    class PluginListNode
    {
    public:
        PluginListNode(const QString plgRName, PluginListNode *previous);
        ~PluginListNode();

        PluginListNode* getPreviousNode();
        QString getPluginName();

        void setNextNode(PluginListNode *node);

        PluginListNode* getNextNode();






        QHash<QString,QString>* getArgsList();

        /**
         * @brief 插入参数到本节点参数列表
         * @param key
         * @param value
         */
        void insertArgs(QString key, QString value);
        /**
         * @brief 合并外来参数列表到节点内部列表，如果两个列表的参数重合，以本节点参数列表为准
         * @param args 外来参数列表
         */
        void mergeArgsList(QHash<QString,QString> args);

    private:
        const QString plugName;
        PluginListNode *previous;
        PluginListNode *nextnode;
        QHash<QString, QString>*const xargs;
    };
}

#endif // PLUGINLISTNODE_H
