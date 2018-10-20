#include "def_plugin_base.h"
using namespace PlgDef;

I_Configurable::I_Configurable():
    argsList(QHash<QString, QHash<QString, QString>>())
{}

I_Configurable::~I_Configurable()
{

}

I_PluginBase::I_PluginBase()
{

}

PlgDef::I_PluginBase::~I_PluginBase(){}

QHash<QString, QHash<QString, QString> > I_Configurable::getArgsList(){
    return this->argsList;
}

void I_Configurable::defineArgs(const QString argsName, I_Configurable::ArgsType typeItem){
    QString typeNum = QString("%1").arg(typeItem);
    auto tempHash = QHash<QString,QString>();
    tempHash.insert("type",typeNum);
    this->argsList.insert(argsName,tempHash);
}

void I_Configurable::insertEnumItem(const QString argsName, const QString enumValue){
    auto temp = this->argsList.find(argsName);
    if(temp == this->argsList.constEnd()){
        this->defineArgs(argsName,ArgsType::ENUM_ITEM);
        temp = this->argsList.find(argsName);
    }
    auto &t_def = temp.value();
    auto t_val = t_def.find("type");
    if(t_val.value() != QString("%1").arg(ArgsType::ENUM_ITEM)){
        QString msg = "重复定义参数：";
        msg += argsName;
        msg += ",插件试图修改此参数为EnumItem类型，故报错。";

        emit this->signal_PushErrorReport(this, msg);
    }
    auto index = QString("%1").arg(t_def.size());
    t_def.insert(index, enumValue);
}

PlgDef::I_Combiantion::I_Combiantion(){}

PlgDef::I_Combiantion::~I_Combiantion(){}
