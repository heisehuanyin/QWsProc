#include "defaultconfigport.h"
#include <iostream>

using namespace PlgDef::ConfigPort;

#define LISTITEM_TAGNAME "____args"
#define LISTITEM_KEY     "key"
#define LISTITEM_VALUE   "value"
#define SIMPLE_APPOINT   "specify"
#define ENCODING_KEY     "Encoding"


DefaultConfigPort::DefaultConfigPort()
{
    auto list = QTextCodec::availableCodecs();

    for(auto itor = list.constBegin();
        itor != list.constEnd();
        ++itor){
        this->insertEnumItem(ENCODING_KEY, *itor);
    }
}

QMenu *DefaultConfigPort::getCustomMenu()
{
    return nullptr;
}

void DefaultConfigPort::saveOperation()
{
    const int indentSize = 4;

    if(!this->file->open(QIODevice::WriteOnly | QIODevice::Text)){
        emit this->signal_PushErrorReport(this,"保存配置文件过程出错");

        return;
    }

    QTextStream out(this->file);
    this->doc->save(out,indentSize);
    out.flush();
    this->file->close();
}

I_ConfigPort *DefaultConfigPort::createNewPort(Core::WsCore *core, const QString fPath,
                                               QHash<QString, QString> argslist)
{
    DefaultConfigPort *rtn = new DefaultConfigPort();

    rtn->file = new QFile(fPath);
    if(! rtn->file->exists()){
        if(! rtn->file->open(QIODevice::WriteOnly | QIODevice::Text)){
            QString error = "新建配置文件过程出错：";
            error += fPath;
            emit this->signal_PushErrorReport(this,error);

            return nullptr;
        }

        QTextCodec *codec = QTextCodec::codecForLocale();
        auto encodeItor = argslist.find(ENCODING_KEY);
        if(encodeItor != argslist.constEnd()){
            codec = QTextCodec::codecForName(encodeItor.value().toUtf8());
        }

        QTextStream out(rtn->file);
        out.setCodec(codec);
        out<<"<?xml version=\"1.0\" ?>"
             "<recordList></recordList>";
        out.flush();
        rtn->file->close();
    }

    if(!rtn->file->open(QIODevice::ReadOnly | QIODevice::Text)){
        QString error = "打不开配置文件：";
        error += fPath;
        emit this->signal_PushErrorReport(this,error);

        return nullptr;
    }

    rtn->doc = new QDomDocument();
    if (!rtn->doc->setContent(rtn->file)) {
        rtn->file->close();
        QString error = "解析XML配置文件出错:";
        error += fPath;
        emit this->signal_PushErrorReport(this, error);

        return nullptr;
    }
    rtn->file->close();

    return rtn;
}

QDomElement DefaultConfigPort::searchElementAsDescription(QDomElement * elm, const QString tagName){
    auto nodeList = elm->elementsByTagName(tagName);
    if(nodeList.size() == 0){
        auto newElm = this->doc->createElement(tagName);
        elm->appendChild(newElm);

        return newElm;
    }

    auto target = nodeList.item(0);
    return target.toElement();
}

const QHash<QString, QString> DefaultConfigPort::getConfigList(const QString key)
{
    auto rtn = QHash<QString, QString>();
    const QStringList list = key.split(".");
    QDomElement root = this->doc->documentElement();

    for(auto item = list.constBegin();
        item != list.constEnd();
        ++item){
        QString str(*item);
        root =  searchElementAsDescription(&root, str);
    }

    for(auto oneElm = root.firstChildElement(LISTITEM_TAGNAME);
        !oneElm.isNull();
        oneElm = root.nextSiblingElement(LISTITEM_TAGNAME)){
        QString key = oneElm.attribute(LISTITEM_KEY);
        QString val = oneElm.attribute(LISTITEM_VALUE);
        rtn.insert(key, val);
    }

    return rtn;
}

void DefaultConfigPort::setConfigList(const QString key, QHash<QString, QString> list)
{
    auto rtn = QHash<QString, QString>();
    const QStringList tagList = key.split(".");
    QDomElement root = this->doc->documentElement();

    for(auto item = tagList.constBegin();
        item != tagList.constEnd();
        ++item){
        QString str(*item);
        root =  searchElementAsDescription(&root, str);
    }

    for(auto oneElm = root.firstChildElement(LISTITEM_TAGNAME);
        !oneElm.isNull();
        oneElm = root.nextSiblingElement(LISTITEM_TAGNAME)){

        QString key = oneElm.attribute(LISTITEM_KEY);

        auto itor = list.find(key);
        if(itor != list.constEnd()){
            oneElm.setAttribute(LISTITEM_VALUE, itor.value());
            list.remove(key);
        }
    }

    for(auto itor = list.constBegin();
        itor != list.constEnd();
        ++itor){
        auto item = this->doc->createElement(LISTITEM_TAGNAME);
        item.setAttribute(LISTITEM_KEY, itor.key());
        item.setAttribute(LISTITEM_VALUE, itor.value());
        root.appendChild(item);
    }
}

void DefaultConfigPort::setKeyValue(const QString key, const QString value)
{
    const QStringList list = key.split(".");
    QDomElement root = this->doc->documentElement();

    for(auto item = list.constBegin();
        item != list.constEnd();
        ++item){
        QString str(*item);
        root =  searchElementAsDescription(&root, str);
    }

    root.setAttribute(SIMPLE_APPOINT, value);
}

const QString DefaultConfigPort::getValue(const QString key, const QString defaultValue)
{
    const QStringList list = key.split(".");
    QDomElement root = this->doc->documentElement();

    for(auto item = list.constBegin();
        item != list.constEnd();
        ++item){
        QString str(*item);
        root =  searchElementAsDescription(&root, str);
    }
    auto xval = root.attribute(SIMPLE_APPOINT);
    if(xval == QString()){
        if(defaultValue != QString()){
            root.setAttribute(SIMPLE_APPOINT, defaultValue);
            xval = defaultValue;
        }
    }

    return xval;
}









