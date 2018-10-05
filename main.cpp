#include <QApplication>
#include <iostream>

#include "wscore.h"

using Core::WsCore;

void __printPluginList(PlgDef::PluginType ,QList<QPair<QString, PlgDef::PluginType>> *const);

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    WsCore *app = new WsCore();

    if(argc == 2 && QString("-print") == argv[1]){
        app->service_OpenSilentModel();
        auto m = app->service_getManager();
        std::cout<<"PlgDef::IO_NoUpStream => "<< PlgDef::IO_NoUpStream <<std::endl;

        auto x = m->service_QueryFactoryList(PlgDef::IO_PluginSymbo);
        __printPluginList(PlgDef::IO_PluginSymbo,x);
        x = m->service_QueryFactoryList(PlgDef::IO_StyleModel);
        __printPluginList(PlgDef::IO_StyleModel,x);
        x = m->service_QueryFactoryList(PlgDef::IO_TableModel);
        __printPluginList(PlgDef::IO_TableModel,x);
        x = m->service_QueryFactoryList(PlgDef::IO_TextModel);
        __printPluginList(PlgDef::IO_TextModel,x);
        x = m->service_QueryFactoryList(PlgDef::IO_TreeModel);
        __printPluginList(PlgDef::IO_TreeModel,x);
        x = m->service_QueryFactoryList(PlgDef::Service_ConfigPort);
        __printPluginList(PlgDef::Service_ConfigPort,x);
        x = m->service_QueryFactoryList(PlgDef::Service_LogPort);
        __printPluginList(PlgDef::Service_LogPort,x);
        x = m->service_QueryFactoryList(PlgDef::UI_ContentView);
        __printPluginList(PlgDef::UI_ContentView,x);
        x = m->service_QueryFactoryList(PlgDef::UI_MenuBar);
        __printPluginList(PlgDef::UI_MenuBar,x);
        x = m->service_QueryFactoryList(PlgDef::UI_StateBar);
        __printPluginList(PlgDef::UI_StateBar,x);
        x = m->service_QueryFactoryList(PlgDef::UI_ToolBar);
        __printPluginList(PlgDef::UI_ToolBar,x);
        x = m->service_QueryFactoryList(PlgDef::UI_Window);
        __printPluginList(PlgDef::UI_Window,x);

        return 0;
    }
    if(argc == 2 && QString("-test") == argv[1]){
        app->service_OpenSilentModel();
        app->test_InnerTest();
        app->service_SaveOperation();

        return 0;
    }
    QString win_id("Default");
    app->service_OpenGraphicsModel(&win_id);



    int rtnum = a.exec();
    app->service_SaveOperation();
    return rtnum;
}

void __printPluginList(PlgDef::PluginType type ,QList<QPair<QString, PlgDef::PluginType>> *const list){
    std::cout << "TypeMark:"<<type << std::endl;
    for(auto itor = list->constBegin();
        itor != list->constEnd();
        ++itor){
        auto pair = *itor;
        std::cout<<"    "<<pair.first.toStdString() <<"    Upstream->" <<pair.second<<std::endl;
    }
}
