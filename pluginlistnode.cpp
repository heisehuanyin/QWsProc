#include "pluginlistnode.h"

Core::PluginListNode::PluginListNode(const QString plgRName, Core::PluginListNode *previous):
    plugName(plgRName),
    previous(previous),
    nextnode(nullptr),
    xargs(new QHash<QString, QString>())
{}

Core::PluginListNode::~PluginListNode(){
    delete this->nextnode;
    delete xargs;
}

Core::PluginListNode *Core::PluginListNode::getPreviousNode(){
    return this->previous;
}

QString Core::PluginListNode::getPluginName(){
    return this->plugName;
}

void Core::PluginListNode::setNextNode(Core::PluginListNode *node){
    this->nextnode = node;
}

Core::PluginListNode *Core::PluginListNode::getNextNode()
{
    return this->nextnode;
}


QHash<QString, QString> *Core::PluginListNode::getArgsList(){
    return this->xargs;
}

void Core::PluginListNode::insertArgs(QString key, QString value){
    this->xargs->insert(key, value);
}

void Core::PluginListNode::mergeArgsList(QHash<QString, QString> args){
    for(auto xargsi = args.constBegin();
        xargsi != args.constEnd();
        ++xargsi){

        auto findR = this->xargs->find(xargsi.key());
        if(findR == this->xargs->constEnd()){
            this->xargs->insert(xargsi.key(), xargsi.value());
        }
    }
}
