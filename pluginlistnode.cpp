#include "pluginlistnode.h"

using namespace Core::CoreBase;

PluginListNode::PluginListNode(const QString plgRName, PluginListNode *previous):
    plugName(plgRName),
    previous(previous),
    nextnode(nullptr),
    xargs(new QHash<QString, QString>())
{}

PluginListNode::~PluginListNode(){
    delete this->nextnode;
    delete xargs;
}

PluginListNode *PluginListNode::getPreviousNode(){
    return this->previous;
}

QString PluginListNode::getPluginName(){
    return this->plugName;
}

void PluginListNode::setNextNode(PluginListNode *node){
    this->nextnode = node;
}

PluginListNode *PluginListNode::getNextNode()
{
    return this->nextnode;
}


QHash<QString, QString> *PluginListNode::getArgsList(){
    return this->xargs;
}

void PluginListNode::insertArgs(QString key, QString value){
    this->xargs->insert(key, value);
}

void PluginListNode::mergeArgsList(QHash<QString, QString> args){
    for(auto xargsi = args.constBegin();
        xargsi != args.constEnd();
        ++xargsi){

        auto findR = this->xargs->find(xargsi.key());
        if(findR == this->xargs->constEnd()){
            this->xargs->insert(xargsi.key(), xargsi.value());
        }
    }
}
