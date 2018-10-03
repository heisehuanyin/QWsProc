#ifndef DEFCONFIGPORT_H
#define DEFCONFIGPORT_H

#include "def_plugin_base.h"


namespace PlgDef {
    namespace ConfigPort {
        class I_ConfigPort : public I_PluginBase{
        public:
            I_ConfigPort();
            virtual ~I_ConfigPort(){}

            /**
             * @brief 创建一个新的配置文件访问接口
             * @param fPath 配置文件路径
             * @return 实例
             */
            virtual I_ConfigPort* createNewPort(QString *const fPath)=0;

            /**
             * @brief 设置配置条目
             * @param key 键名
             * @param value 内容值
             */
            virtual void setKeyValue(QString *const key, QString *const value)=0;

            /**
             * @brief 获取配置条目
             * @param key 键名
             * @param defaultValue 默认值
             * @return 配置的内容值
             */
            virtual QString * getValue(QString *const key, QString *const defaultValue)=0;
        };
    }
}

#endif // DEFCONFIGPORT_H
