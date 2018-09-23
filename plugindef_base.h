#ifndef PLUGINDEF_BASE_H
#define PLUGINDEF_BASE_H

#include <QMenu>

namespace PlgDef{
    enum PluginType{
        UI_MenuBar,
        UI_ToolBar,
        UI_StateBar,
        UI_Window,
        UI_ContentView,

        IO_NoUpStream,
        IO_PluginSymbo,
        IO_TextModel,
        IO_TreeModel,
        IO_TableModel,
        IO_StyleModel,

        Service_ConfigPort,
        Service_LogPort
    };

    class I_PluginBase{
    public:
        virtual ~I_PluginBase();

        virtual int pluginMark() = 0;
        virtual int upStreamMark() = 0;

        virtual QMenu getCustomMenu() = 0;
        virtual void saveOperation() = 0;
    };
}

#endif // PLUGINDEF_H
