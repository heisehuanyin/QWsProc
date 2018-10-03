#ifndef DEFMENUBAR_H
#define DEFMENUBAR_H

#include "def_plugin_base.h"

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

#endif // DEFMENUBAR_H
