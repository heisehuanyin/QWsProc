#include <QApplication>
#include <iostream>

#include "wscore.h"
#include "args_parse.h"

using Core::WsCore;

void __printPluginList(QHash<PlgDef::PluginType,QList<QPair<QString, PlgDef::PluginType>>> lists);

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // arguments groups
    args_extra::gnu parse("QWsProc", "综合性插件化通用文件处理系统");
    parse.resetDescription("软件致力于设计一个通用的插件化的文件处理系统，利用软件提供的接口设计的各个插件能够自由的组合到一起进行文件处理。"
                           "通过不同的通用插件之间的相互配合应对各种不同的文件格式的编辑工作。保留拓展本系统到更加广阔的用途的能力。");
    parse.setLongOptions({
                             {"help","","显示详细帮助命令信息"},
                             {"list","","罗列系统中已经注册的插件以及插件的相关信息"},
                             {"test","","方便开发者使用的内部测试函数，测试代码写在里面"},
                             {"graphic","path_to_file","打开图形界面，直接打开指定文件"},
                             {"slience","path_to_script","不打开图形界面，使用静默模式运行指定脚本文件"}
                         });

    std::vector<std::string> args;
    for (auto i=0;i<argc;++i) {
        args.push_back(argv[i]);
    }

    try {
        parse.argsParse(args);
    } catch (args_extra::ex_base *e) {
        std::cout<< e->type() << ":" << e->description() << "." << std::endl;
        return 0;
    }


    // arguments parse and exec
    WsCore *app = new WsCore(&a);
    app->openCoreService();

    std::string value, key = parse.peekOption({"help"}, value);
    if(key != ""){
        std::cout << parse.helpString() << std::endl;
    }

    key = parse.peekOption({"list"}, value);
    if(key!=""){
        auto m = app->service_getManager();
        std::cout<<"PlgDef::Feature_NoCombination === "<< PlgDef::Feature_NoCombination <<std::endl;
        std::cout<<"PlgDef::Feature_Combination   === "<< PlgDef::Feature_Combination   <<std::endl;

        auto lists = m->service_QueryFactoryList();
        __printPluginList(lists);
    }

    key = parse.peekOption({"test"}, value);
    if(key!=""){
        app->test_InnerTest();
    }

    int retcode = 0;
    key = parse.peekOption({"graphic", "slience"}, value);
    if(key == "graphic"){
        app->service_OpenGraphicsModel("Default");
        if(value != "")
            app->operate_OpenFile(QString(value.c_str()), nullptr, app->service_getManager()->instance_GetWindowInstance("Default"));

        retcode = a.exec();
    }

    app->slot_SaveOperation();
    return retcode;
}

void __printPluginList(QHash<PlgDef::PluginType,QList<QPair<QString, PlgDef::PluginType>>> lists){
    int name_max = 0;
    for (auto it=lists.constBegin(); it != lists.constEnd(); ++it) {
        auto list = it.value();
        for (auto i2=list.constBegin(); i2!=list.constEnd(); ++i2) {
            if(i2->first.size() > name_max)
                name_max = i2->first.size();
        }
    }


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
            auto pre = "    " + pair.first;
            pre += QString(name_max-pair.first.size(), ' ');

            std::cout<<pre.toStdString()<<"    Upstream->" <<pair.second<<std::endl<<std::endl;
        }
    }
}







