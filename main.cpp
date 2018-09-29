#include "SingalView.h"
#include <QApplication>
#include "wscore.h"
#include <iostream>

using Core::WsCore;

void openCtlModel(WsCore *core);
void openGraphicsModel(WsCore *core);
void openCustomFile(WsCore *core);
void runMacroSourceCode(WsCore *core);
void printInfo(WsCore *core);



int main(int argc, char *argv[])
{
    std::cout<<"参数个数:" <<argc <<std::endl;
    for(int i=0;i<argc;++i){
        std::cout<<"<" <<i <<">" <<argv[i] <<std::endl;
    }
    std::cout<<"========================================"<<std::endl;

    if(argc == 2 && QString(argv[1]) == QString("-help")){
        std::cout<<"命令使用形式：exe [operation] [filepath]"<<std::endl;
        std::cout<<"operation:"<<std::endl;
        std::cout<<"    1. -w 开启图形界面，无此选项则开启后台静默模式；"<<std::endl;
        std::cout<<"    2. -s 打开指定路径文件供操作，如果开启的是图形界面，内容会显示在图形界面上；"<<std::endl;
        std::cout<<"    3. -r 运行文件。先决条件采用“-s”打开文件；"<<std::endl;
        std::cout<<"filepath:"<<std::endl;
        std::cout<<"    * 文件路径，当采用“-s”标志打开软件，本选项不可缺少；"<<std::endl;

        return 0;
    }

    WsCore *core = new WsCore();

    /*


    QApplication a(argc, argv);

    return a.exec();*/

    return 0;
}
