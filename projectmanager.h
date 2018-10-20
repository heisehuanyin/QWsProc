#ifndef PROJECTMANAGER_H
#define PROJECTMANAGER_H

#include "def_plugin_base.h"

namespace PlgDef {
    namespace ProjectManager{
        class I_ProjectManager: public I_Configurable,public I_Combiantion
        {
        public:
            I_ProjectManager();
            virtual ~I_ProjectManager() override;

            virtual QString suffix() = 0;


            // I_PluginBase interface
        public:
            virtual PluginType pluginMark() override;
        };
    }
}

#endif // PROJECTMANAGER_H
