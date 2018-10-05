#ifndef DEFCONFIGPORT_H
#define DEFCONFIGPORT_H

#include "def_plugin_base.h"


namespace PlgDef {
    namespace ConfigPort {
        namespace  {
            QString DefaultWindowType_Key = "DefaultWinType";
            QString DefaultWindowType_Value = "DefaultSingalView";
            QString DefaultWindowWidth_Value = "800";
            QString DefaultWindowHeight_Value = "600";

            QString DefaultWindowWidth_Key(QString groupID){
                QString key = "default.window." + groupID + ".width";
                return key;
            }
            QString DefaultWindowHeight_Key(QString groupID){
                QString key = "default.window." + groupID + ".height";
                return key;
            }
        }

        class I_ConfigPort : public I_PluginBase
        {
            Q_OBJECT

        public:
            I_ConfigPort(){}
            virtual ~I_ConfigPort() override {}

            /**
             * @brief 创建一个新的配置文件访问接口
             * @param fPath 配置文件路径
             * @return 实例
             */
            virtual I_ConfigPort* createNewPort(const QString fPath)=0;

            /**
             * @brief 获取配置条目,如果配置文件中关于此配置条目的配置结果为空，则将默认值写入配置文件
             * @param key 键名
             * @param defaultValue 默认值
             * @return 配置的内容值
             */
            virtual const QString getValue(const QString key, const QString defaultValue)=0;

        public slots:
            /**
             * @brief 设置配置条目
             * @param key 键名：形如“xxxx.yyyy.cccc.zzzz”
             * @param value 内容值
             */
            virtual void setKeyValue(const QString key, const QString value)=0;


            // I_PluginBase interface
        public:
            virtual PluginType pluginMark() override final{
                return PlgDef::Service_ConfigPort;
            }
        };
    }
}

#endif // DEFCONFIGPORT_H
