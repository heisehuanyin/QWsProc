#include "DefaultLogPort.h"
#include <iostream>

using namespace PlgDef::LogPort;

DefaultLogPort::DefaultLogPort():
    pluginName(new QString("DefaultLogPort"))
{

}

PlgDef::LogPort::DefaultLogPort::~DefaultLogPort()
{
    delete this->pluginName;
    delete this->logPort;
    delete this->logStream;
    delete this->error;
}

const QString *DefaultLogPort::registName()
{
    return this->pluginName;
}


PlgDef::PluginType PlgDef::LogPort::DefaultLogPort::upStreamMark()
{
    return PlgDef::IO_NoUpStream;
}

QMenu *PlgDef::LogPort::DefaultLogPort::getCustomMenu()
{
    return nullptr;
}

void PlgDef::LogPort::DefaultLogPort::saveOperation()
{
    this->logStream->flush();
}

I_LogPort *PlgDef::LogPort::DefaultLogPort::createNewPort(QString * const fPath)
{
    DefaultLogPort* rtn = new DefaultLogPort();
    rtn->logPort = new QFile(*fPath);
    if(! rtn->logPort->open(QIODevice::WriteOnly|QIODevice::Text)){
        *this->error = "打开Log文件过程中出现错误";

        emit this->signal_Recieve_ProcessError(this, this->error);

        return nullptr;
    }
    rtn->logStream = new QTextStream(rtn->logPort);

    return rtn;
}

void PlgDef::LogPort::DefaultLogPort::writeLog(PlgDef::I_PluginBase *p, QString * const msg)
{
    QString msgout = "Log::";
    if(p==nullptr)
        msgout += "MainFramework";
    else
        msgout += p->registName();

    *this->logStream <<msgout <<":"<<*msg <<"\n";
}

void PlgDef::LogPort::DefaultLogPort::errorLog(PlgDef::I_PluginBase *p, QString * const msg)
{
    QString msgout = "Error::";
    if(p==nullptr)
        msgout += "MainFramework";
    else
        msgout += p->registName();


    *this->logStream <<msgout <<":"<<*msg;
}

void PlgDef::LogPort::DefaultLogPort::echoLog(PlgDef::I_PluginBase *p, QString * const msg)
{
    QString msgout = "Echo::";
    if(p==nullptr)
        msgout += "MainFramework";
    else
        msgout += *p->registName();


    *this->logStream <<msgout <<":" <<*msg <<"\n";
    std::cout<<msg <<std::endl;
}
