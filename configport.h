#ifndef CONFIGPORT_H
#define CONFIGPORT_H

#include "plugindef_base.h"


namespace PlgDef {
    namespace ConfigPort {
        class I_ConfigPort : public I_PluginBase{
        public:
            /**
             * @brief 创建一个新的配置文件访问接口
             * @param fPath 配置文件路径
             * @return 实例
             */
            I_ConfigPort* createNewPort(QString* fPath);

            /**
             * @brief 设置配置条目
             * @param key 键名
             * @param value 内容值
             */
            void setKeyValue(QString *key, QString *value);

            /**
             * @brief 获取配置条目
             * @param key 键名
             * @param defaultValue 默认值
             * @return 配置的内容值
             */
            QString* getValue(QString *key, QString *defaultValue);
        };
    }
}

#endif // CONFIGPORT_H
