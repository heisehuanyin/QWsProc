#include <QApplication>
#include <iostream>

#include "wscore.h"

using Core::WsCore;

void __printPluginList(QHash<PlgDef::PluginType,QList<QPair<QString, PlgDef::PluginType>>> lists);

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    WsCore *app = new WsCore();

    if(argc == 2 && QString("-print") == argv[1]){
        app->service_OpenSilentModel();
        auto m = app->service_getManager();
        std::cout<<"PlgDef::IO_NoUpStream === "<< PlgDef::IO_NoUpStream <<std::endl;

        auto lists = m->service_QueryFactoryList();
        __printPluginList(lists);

        return 0;
    }

    if(argc == 2 && QString("-test") == argv[1]){
        app->service_OpenSilentModel();
        app->test_InnerTest();
        app->service_SaveOperation();

        return 0;
    }


    app->service_OpenGraphicsModel("Default");

    int rtnum = a.exec();
    app->service_SaveOperation();
    return rtnum;
}

void __printPluginList(QHash<PlgDef::PluginType,QList<QPair<QString, PlgDef::PluginType>>> lists){
    for(auto itor=lists.constBegin();
        itor != lists.constEnd();
        ++itor){
        auto mark = itor.key();
        auto list = itor.value();

        std::cout << "TypeMark:"<<mark << std::endl;
        for(auto itor = list.constBegin();
            itor != list.constEnd();
            ++itor){
            auto pair = *itor;
            std::cout<<"    "<<pair.first.toStdString() <<"    Upstream->" <<pair.second<<std::endl;
        }


    }
}







