#ifndef DEFCONFIGPORT_H
#define DEFCONFIGPORT_H

#include "def_plugin_base.h"
#include "wscore.h"


namespace PlgDef {
    namespace ConfigPort {
        namespace  {
            QString DefaultWindowWidth_Value = "800";
            QString DefaultWindowHeight_Value = "600";
            QString DefaultWindowType_Key = "UI.DefaultWinType";
            QString DefaultWindowType_Value = "DefaultSingalView";

            QString DefaultChannelPreface_Key = "IO.DefaultChannelPreface";
            QString DefaultChannelPreface_Value = "ChannelPreface";
            QString DefaultTextModel_Key = "IO.DefaultTextModelType";
            QString DefaultTextModel_Value = "DefaultTextModel";

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
             * @param core 核心调度模块
             * @param fPath 配置文件路径
             * @param argslist 参数列表
             * @return 实例
             */
            virtual I_ConfigPort* createNewPort(Core::WsCore *core, const QString fPath,
                                                QHash<QString, QString> argslist)=0;

            /**
             * @brief 获取配置条目,如果配置条目存在，则返回配置结果；
             *          如果配置条目不存在且提供写入值则返回写入值，并将写入值写入配置条目，
             *          如果配置条目不存在且未提供写入值，则返回QString()，写入值不写入配置条目；
             * @param key 键名
             * @param defaultValue 默认值
             * @return 配置的内容值
             */
            virtual const QString getValue(const QString key, const QString writeIn = QString())=0;

            /**
             * @brief 通过特定键名，获取一个配置项目列表
             * @param key 键名
             * @return 配置列表
             */
            virtual const QHash<QString, QString> getConfigList(const QString key)=0;

        public slots:
            /**
             * @brief 设置配置条目
             * @param key 键名：形如“xxxx.yyyy.cccc.zzzz”
             * @param value 内容值
             */
            virtual void setKeyValue(const QString key, const QString value)=0;

            /**
             * @brief 设定一个配置项目列表
             * @param key 列表索引键
             * @param list 写入列表
             */
            virtual void setConfigList(const QString key, QHash<QString, QString> list)=0;


            // I_PluginBase interface
        public:
            virtual PluginType pluginMark() override final{
                return PlgDef::Service_ConfigPort;
            }
        };
    }
}

#endif // DEFCONFIGPORT_H
