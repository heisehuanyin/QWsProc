#include "DefaultConfigPort.h"
#include <iostream>

using namespace PlgDef::ConfigPort;

DefaultConfigPort::DefaultConfigPort():
    pluginName(new QString("DefaultConfigPort")),
    value_temp(nullptr)
{

}

const QString *DefaultConfigPort::registName()
{
    return this->pluginName;
}

PlgDef::PluginType DefaultConfigPort::upStreamMark()
{
    return PlgDef::IO_NoUpStream;
}

QMenu *DefaultConfigPort::getCustomMenu()
{
    return nullptr;
}

void DefaultConfigPort::saveOperation()
{
    const int indentSize = 4;

    if(!this->file->open(QIODevice::WriteOnly | QIODevice::Text)){
        *this->error = "保存配置文件过程出错";
        emit this->signal_Recieve_ProcessError(this,this->error);

        return;
    }

    QTextStream out(this->file);
    this->doc->save(out,indentSize);
    out.flush();
    this->file->close();
}

I_ConfigPort *DefaultConfigPort::createNewPort(QString * const fPath)
{
    DefaultConfigPort *rtn = new DefaultConfigPort();

    rtn->file = new QFile(*fPath);
    if(! rtn->file->exists()){
        if(! rtn->file->open(QIODevice::WriteOnly | QIODevice::Text)){
            *this->error = "新建配置文件过程出错：";
            *this->error += *fPath;
            emit this->signal_Recieve_ProcessError(this,this->error);

            return nullptr;
        }

        QTextStream out(rtn->file);
        out<<"<?xml version=\"1.0\" encoding=\"utf-8\" ?>"
             "<recordList></recordList>";
        out.flush();
        rtn->file->close();
    }

    if(!rtn->file->open(QIODevice::ReadOnly | QIODevice::Text)){
        *this->error = "打不开配置文件：";
        *this->error += *fPath;
        emit this->signal_Recieve_ProcessError(this,this->error);

        return nullptr;
    }

    rtn->doc = new QDomDocument();
    if (!rtn->doc->setContent(rtn->file)) {
        rtn->file->close();
        *this->error = "解析XML配置文件出错:";
        *this->error += *fPath;
        emit this->signal_Recieve_ProcessError(this, this->error);

        return nullptr;
    }
    rtn->file->close();

    return rtn;
}

QDomElement DefaultConfigPort::searchElementAsDescription(QDomElement * elm, QString *tagName){
    auto nodeList = elm->elementsByTagName(*tagName);
    if(nodeList.size() == 0){
        auto newElm = this->doc->createElement(*tagName);
        elm->appendChild(newElm);

        return newElm;
    }

    auto target = nodeList.item(0);
    return target.toElement();
}

void DefaultConfigPort::setKeyValue(QString * const key, QString * const value)
{
    const QStringList list = (*key).split(".");
    QDomElement root = this->doc->documentElement();

    for(auto item = list.constBegin();
        item != list.constEnd();
        ++item){
        QString str(*item);
        root =  searchElementAsDescription(&root, &str);
    }

    root.setAttribute("value", *value);
}

const QString *DefaultConfigPort::getValue(QString * const key, QString * const defaultValue)
{
    const QStringList list = (*key).split(".");
    QDomElement root = this->doc->documentElement();

    for(auto item = list.constBegin();
        item != list.constEnd();
        ++item){
        QString str(*item);
        root =  searchElementAsDescription(&root, &str);
    }
    auto xval = root.attribute("value", *defaultValue);
    if(xval == *defaultValue){
        root.setAttribute("value", *defaultValue);
    }

    if(this->value_temp)
        delete this->value_temp;
    this->value_temp = new QString(xval);

    return this->value_temp;
}












































