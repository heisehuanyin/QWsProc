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
    std::cout<<"��������:" <<argc <<std::endl;
    for(int i=0;i<argc;++i){
        std::cout<<"<" <<i <<">" <<argv[i] <<std::endl;
    }
    std::cout<<"========================================"<<std::endl;

    if(argc == 2 && QString(argv[1]) == QString("-help")){
        std::cout<<"����ʹ����ʽ��exe [operation] [filepath]"<<std::endl;
        std::cout<<"operation:"<<std::endl;
        std::cout<<"    1. -w ����ͼ�ν��棬�޴�ѡ��������̨��Ĭģʽ��"<<std::endl;
        std::cout<<"    2. -s ��ָ��·���ļ��������������������ͼ�ν��棬���ݻ���ʾ��ͼ�ν����ϣ�"<<std::endl;
        std::cout<<"    3. -r �����ļ����Ⱦ��������á�-s�����ļ���"<<std::endl;
        std::cout<<"filepath:"<<std::endl;
        std::cout<<"    * �ļ�·���������á�-s����־���������ѡ���ȱ�٣�"<<std::endl;

        return 0;
    }

    WsCore *core = new WsCore();

    /*


    QApplication a(argc, argv);

    return a.exec();*/

    return 0;
}
