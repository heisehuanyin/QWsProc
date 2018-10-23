#ifndef DEF_MENUBAR_H
#define DEF_MENUBAR_H

#include "def_plugin_base.h"

#include <QMenu>
#include <QMenuBar>

namespace Core {
    class WsCore;
}

/**
@brief 插件统一定义命名空间
*/
namespace PlgDef{
    /**
    @brief 菜单栏命名空间*/
    namespace MenuBar{
        /**
        @brief 菜单栏*/
        class I_MenuBar:public I_Configurable{
            Q_OBJECT
        public:
            I_MenuBar();
            virtual ~I_MenuBar() override;

            virtual I_MenuBar* createNewMenubar(Core::WsCore* core, QString groupId) =0;

            /**
             * @brief 返回新的Menubar，单元自行管理生成的Menubar实例，在新的Menubar返回后，自行销毁之前的Menubar
             * @param vlist Menu实例
             * @return 新QMenubar实例
             */
            virtual QMenuBar* refreshMenuBarWidget(QList<QMenu*> vlist)=0;

            // I_PluginBase interface
        public:
            virtual PluginType pluginMark() override final;
        };
    }


}

#endif // DEFMENUBAR_H
