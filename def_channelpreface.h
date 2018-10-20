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
            QString filePath;
            QString pjtPath;
            Core::WsCore * core;

            // I_PluginBase interface
        public:
            I_ChannelPreface();
            virtual ~I_ChannelPreface() override;

            virtual I_ChannelPreface* createNewChannelPreface(Core::WsCore * core,
                                                              const QString filePath,
                                                              const QString pjtPath);

            virtual QString getProjectCfgfilePath();

            virtual QString getFilePath();

            virtual const QString registName() override;
            virtual PluginType pluginMark() override final;
            virtual QMenu *getCustomMenu() override;
            virtual void saveOperation() override
            {}
        };
    }
}

#endif // CHANNELPREFACE_H
