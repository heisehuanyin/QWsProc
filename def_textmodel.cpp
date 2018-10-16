#include "def_textmodel.h"

PlgDef::TextModel::I_TextModel::I_TextModel(){}

PlgDef::TextModel::I_TextModel::~I_TextModel() {}

PlgDef::PluginType PlgDef::TextModel::I_TextModel::pluginMark() {
    return PlgDef::IO_TextModel;
}
