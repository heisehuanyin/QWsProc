#include <QApplication>
#include <iostream>

#include "wscore.h"

using Core::WsCore;

void __printPluginList(QHash<PlgDef::PluginType,QList<QPair<QString, PlgDef::PluginType>>> lists);

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    WsCore *app = new WsCore(&a);

    if(argc == 2 && QString("-print") == argv[1]){
        app->service_OpenSilentModel();
        auto m = app->service_getManager();
        std::cout<<"PlgDef::Feature_NoCombination === "<< PlgDef::Feature_NoCombination <<std::endl;
        std::cout<<"PlgDef::Feature_Combination   === "<< PlgDef::Feature_Combination   <<std::endl;

        auto lists = m->service_QueryFactoryList();
        __printPluginList(lists);

        app->slot_SaveOperation();
        return 0;
    }

    if(argc == 2 && QString("-test") == argv[1]){
        app->service_OpenSilentModel();
        app->test_InnerTest();
        app->slot_SaveOperation();

        return 0;
    }


    app->service_OpenGraphicsModel("Default");

    if(argc > 2 && QString(argv[1]).indexOf('s') != -1){
        app->operate_OpenFile(argv[2], nullptr,
                app->service_getManager()->instance_GetWindowInstance("Default"));
    }

    int rtnum = a.exec();
    app->slot_SaveOperation();
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







