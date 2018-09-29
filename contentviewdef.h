#ifndef CONTENTVIEWDEF_H
#define CONTENTVIEWDEF_H

#include "plugindef_base.h"



namespace PlgDef {

    /**
    @brief 内容面板命名空间*/
    namespace ContentView {
        /**
        @brief 内容面板*/
        class I_ContentView: public I_PluginBase{
        public:

            virtual QWidget* getWidget() = 0;

        };
    }
}

#endif // CONTENTVIEWDEF_H
