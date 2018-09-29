#ifndef WSCORE_H
#define WSCORE_H

#include "plugindef_base.h"

namespace Core {
    class PluginManager{

    };

    class WsCore{
    public:
        WsCore(){}
        ~WsCore(){}

        /**
         * @brief 获取插件管理器
         * @return 插件管理器实例
         */
        PluginManager service_getManager();

        /**
         * @brief 注册插件工厂便捷接口
         * @param p 插件实例
         */
        void service_RegisterPlugin(PlgDef::I_PluginBase *p);

        /**
         * @brief 刷新指定窗口实例上的menubar
         * @param window 指定窗口实例
         */
        void service_RefreshMenuBar(PlgDef::I_PluginBase *window);

        /**
         * @brief 默认全平台保存操作
         */
        void service_SaveOperation();
    };
}




#endif // WSCORE_H
