#include "defaultlogport.h"
#include <QTextCodec>
#include <iostream>

using namespace PlgDef::LogPort;

#define ENCODING "Encoding"

DefaultLogPort::DefaultLogPort():
    pluginName("DefaultLogPort")
{
    auto list = QTextCodec::availableCodecs();

    for(auto itor = list.constBegin();
        itor != list.constEnd();
        ++itor){
        this->insertEnumItem(ENCODING, *itor);
    }
}

PlgDef::LogPort::DefaultLogPort::~DefaultLogPort()
{
    delete this->logPort;
    delete this->logStream;
}

const QString DefaultLogPort::registName()
{
    return this->pluginName;
}


QMenu *PlgDef::LogPort::DefaultLogPort::getCustomMenu()
{
    return nullptr;
}

void PlgDef::LogPort::DefaultLogPort::saveOperation()
{
    this->logStream->flush();
}

I_LogPort *PlgDef::LogPort::DefaultLogPort::createNewPort(Core::WsCore *core, const QString fPath,
                                                          QHash<QString, QString> argslist)
{
    DefaultLogPort* rtn = new DefaultLogPort();
    rtn->core = core;
    rtn->logPort = new QFile(fPath);
    if(! rtn->logPort->open(QIODevice::WriteOnly|QIODevice::Text)){

        emit this->signal_PushErrorReport(this, "打开Log文件过程中出现错误");

        return nullptr;
    }
    QTextCodec *codec = QTextCodec::codecForLocale();

    auto valptr = argslist.find(ENCODING);
    if(valptr != argslist.constEnd()){
        auto bytec = valptr.value().toUtf8();
        codec = QTextCodec::codecForName(bytec);
    }
    rtn->logStream = new QTextStream(rtn->logPort);
    rtn->logStream->setCodec(codec);

    return rtn;
}

void PlgDef::LogPort::DefaultLogPort::writeLog(PlgDef::I_PluginBase *p, const QString msg)
{
    QString msgout = "Log::";
    if(p == nullptr)
        msgout += "MainFramework";
    else
        msgout += p->registName();

    *this->logStream <<msgout <<":"<<msg << endl;
}

void PlgDef::LogPort::DefaultLogPort::errorLog(PlgDef::I_PluginBase *p, const QString msg)
{
    QString msgout = "Error::";
    if(p==nullptr)
        msgout += "MainFramework";
    else
        msgout += p->registName();


    std::cout<<msg.toLocal8Bit().toStdString() <<std::endl;
    *this->logStream <<msgout <<":"<<msg << endl;
}

void PlgDef::LogPort::DefaultLogPort::echoLog(PlgDef::I_PluginBase *p, const QString msg)
{
    QString msgout = "Echo::";
    if(p==nullptr)
        msgout += "MainFramework";
    else
        msgout += p->registName();


    std::cout<<msg.toLocal8Bit().toStdString() <<std::endl;
    *this->logStream <<msgout <<":" <<msg << endl;
}
