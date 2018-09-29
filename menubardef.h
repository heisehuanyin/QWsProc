#ifndef MENUBARDEF_H
#define MENUBARDEF_H

#include "plugindef_base.h"
#include "wscore.h"

#include <QMenu>

/**
@brief 插件统一定义命名空间
*/
namespace PlgDef{
    /**
    @brief 菜单栏命名空间*/
    namespace MenuBar{
        /**
        @brief 菜单栏*/
        class I_MenuBar:public I_PluginBase{};
    }


}

#endif // PLUGINDEF_H
