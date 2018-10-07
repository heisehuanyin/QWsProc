#ifndef CHANNELPREFACE_H
#define CHANNELPREFACE_H

#include "def_plugin_base.h"
#include "wscore.h"

namespace PlgDef {
    namespace ChannelPreface {
        class I_ChannelPreface:public I_PluginBase
        {
            Q_OBJECT
        private:
            const QString plgName;
            QString filePath;
            QString pjtPath;
            Core::WsCore * core;

            // I_PluginBase interface
        public:
            I_ChannelPreface():
                plgName("ChannelPreface")
            {}
            virtual ~I_ChannelPreface() override
            {}

            virtual I_ChannelPreface* createNewChannelPreface(Core::WsCore * core, const QString filePath, const QString pjtPath){
                auto rtn = new I_ChannelPreface();

                rtn->filePath = filePath;
                rtn->core = core;
                rtn->pjtPath = pjtPath;

                return rtn;
            }

            virtual QString getProjectPath(){
                return this->pjtPath;
            }

            virtual QString getFilePath(){
                return this->filePath;
            }

            virtual const QString registName() override{
                return this->plgName;
            }
            virtual PluginType pluginMark() override final{
                return PlgDef::IO_ChannelPreface;
            }
            virtual PluginType upStreamMark() override final{
                return PlgDef::IO_NoUpStream;
            }
            virtual QMenu *getCustomMenu() override{
                return nullptr;
            }
            virtual void saveOperation() override
            {}
        };
    }
}

#endif // CHANNELPREFACE_H
