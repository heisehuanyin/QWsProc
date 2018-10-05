#ifndef DEFLOGPORT_H
#define DEFLOGPORT_H
#include "def_plugin_base.h"

#include <exception>

namespace PlgDef {

    namespace LogPort {
        namespace  {
            QString DefaultWindowType_Key = "DefaultWinType";
            QString DefaultWindowType_Value = "DefaultSingalView";
        }

        class I_LogPort : public I_PluginBase
        {
            Q_OBJECT

        public:
            I_LogPort(){}
            virtual ~I_LogPort() override {}

            /**
             * @brief 创建一个Log输出实例
             * @param fPath Log文件路径
             * @return 实例
             */
            virtual I_LogPort* createNewPort(const QString fPath)=0;


        public slots:
            /**
             * @brief 输出常规Log信息到文件
             * @param p 信息源插件
             * @param msg 信息
             */
            virtual void writeLog(PlgDef::I_PluginBase* p, const QString msg)=0;

            /**
             * @brief 输出错误Log信息到文件，到std::out
             * @param p 信息源插件
             * @param msg 信息
             */
            virtual void errorLog(PlgDef::I_PluginBase* p, const QString msg)=0;

            /**
             * @brief 输出提示性信息到std::out,到文件
             * @param p 信息源插件
             * @param msg 信息
             */
            virtual void echoLog(PlgDef::I_PluginBase* p, const QString msg)=0;

            // I_PluginBase interface
        public:
            virtual PluginType pluginMark() override final{
                return PlgDef::Service_LogPort;
            }
        };
    }


}

#endif // DEFLOGPORT_H
