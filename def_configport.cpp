#include "def_configport.h"

QString PlgDef::ConfigPort::DefaultWindowWidth_Key(QString groupID){
    QString key = "default.window." + groupID + ".width";
    return key;
}

QString PlgDef::ConfigPort::DefaultWindowHeight_Key(QString groupID){
    QString key = "default.window." + groupID + ".height";
    return key;
}

QString PlgDef::ConfigPort::EncodeFromMsg_4_KeyExport(QString msg){
    return "file_parse." + msg +".proclist";
}

PlgDef::ConfigPort::I_ConfigPort::I_ConfigPort(){}

PlgDef::ConfigPort::I_ConfigPort::~I_ConfigPort() {}

PlgDef::PluginType PlgDef::ConfigPort::I_ConfigPort::pluginMark(){
    return PlgDef::Service_ConfigPort;
}
