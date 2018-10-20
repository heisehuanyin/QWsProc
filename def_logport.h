#ifndef DEFLOGPORT_H
#define DEFLOGPORT_H
#include "def_plugin_base.h"
#include "wscore.h"

#include <exception>

namespace PlgDef {

    namespace LogPort {

        class I_LogPort : public I_Configurable
        {
            Q_OBJECT

        public:
            I_LogPort();
            virtual ~I_LogPort() override;

            /**
             * @brief 创建一个新的配置文件访问接口
             * @param core 核心调度模块
             * @param fPath 配置文件路径
             * @param argslist 参数列表
             * @return 实例
             */
            virtual I_LogPort* createNewPort(Core::WsCore *core, const QString fPath,
                                             QHash<QString, QString> argslist)=0;


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
            virtual PluginType pluginMark() override final;
        };
    }


}

#endif // DEFLOGPORT_H
