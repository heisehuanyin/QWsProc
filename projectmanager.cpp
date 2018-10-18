#include "projectmanager.h"

using namespace PlgDef::ProjectManager;


I_ProjectManager::I_ProjectManager()
{

}

I_ProjectManager::~I_ProjectManager(){}

PlgDef::PluginType I_ProjectManager::pluginMark(){
    return PlgDef::Service_ProjectManager;
}
