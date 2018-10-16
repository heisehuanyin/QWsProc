#include "def_logport.h"

PlgDef::LogPort::I_LogPort::I_LogPort(){}

PlgDef::LogPort::I_LogPort::~I_LogPort() {}

PlgDef::PluginType PlgDef::LogPort::I_LogPort::pluginMark(){
    return PlgDef::Service_LogPort;
}
