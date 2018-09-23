#ifndef WSCORE_H
#define WSCORE_H

#include "plugindef_base.h"

namespace Core {
    class PluginManager{

    };

    class WsCore{
    public:
        WsCore();
        ~WsCore();

        PluginManager service_getManager();
        void service_RegisterPlugin(PlgDef::I_PluginBase p);
    };
}




#endif // WSCORE_H
