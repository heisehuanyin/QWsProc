#include "def_channelpreface.h"

PlgDef::ChannelPreface::I_ChannelPreface::I_ChannelPreface():
    plgName("ChannelPreface")
{}

PlgDef::ChannelPreface::I_ChannelPreface::~I_ChannelPreface()
{}

PlgDef::ChannelPreface::I_ChannelPreface *PlgDef::ChannelPreface::I_ChannelPreface::createNewChannelPreface(Core::WsCore *core,
                                                                                                            const QString filePath,
                                                                                                            const QString pjtPath){
    auto rtn = new I_ChannelPreface();

    rtn->filePath = filePath;
    rtn->core = core;
    rtn->pjtPath = pjtPath;

    return rtn;
}

QString PlgDef::ChannelPreface::I_ChannelPreface::getProjectCfgfilePath(){
    return this->pjtPath;
}

QString PlgDef::ChannelPreface::I_ChannelPreface::getFilePath(){
    return this->filePath;
}

const QString PlgDef::ChannelPreface::I_ChannelPreface::registName(){
    return this->plgName;
}

PlgDef::PluginType PlgDef::ChannelPreface::I_ChannelPreface::pluginMark(){
    return PlgDef::IO_ChannelPreface;
}

PlgDef::PluginType PlgDef::ChannelPreface::I_ChannelPreface::upStreamMark(){
    return PlgDef::IO_NoUpStream;
}

QMenu *PlgDef::ChannelPreface::I_ChannelPreface::getCustomMenu(){
    return nullptr;
}
