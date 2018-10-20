#include "def_menubar.h"

using namespace PlgDef::MenuBar;
I_MenuBar::I_MenuBar(){}

I_MenuBar::~I_MenuBar() {}

PlgDef::PluginType I_MenuBar::pluginMark(){
    return PlgDef::UI_MenuBar;
}
