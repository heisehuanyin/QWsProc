#include "SingalView.h"

using namespace PlgDef::Window;
namespace MBar = PlgDef::MenuBar;
namespace CView = PlgDef::ContentView;

SingalView::SingalView():I_Window ()
{
    this->window = new QMainWindow(nullptr);
    *(this->plgName) = "SingleView";
}

SingalView::~SingalView()
{
    delete this->plgName;
    delete this->window;
}

QString *SingalView::registName()
{
    return this->plgName;
}

PlgDef::PluginType SingalView::pluginMark()
{
    return PlgDef::UI_Window;
}

PlgDef::PluginType SingalView::upStreamMark()
{
    return PlgDef::IO_NoUpStream;
}

QMenu *SingalView::getCustomMenu()
{
    return nullptr;
}

void SingalView::saveOperation()
{
    //TODO saveOperate
}

PlgDef::Window::I_Window *SingalView::openNewWindow(Core::WsCore *core, QString *gid)
{
    SingalView* rtn = new SingalView();
    rtn->core = core;
    rtn->gid = gid;
    rtn->window->show();

    return rtn;
}

QString *SingalView::getGroupID()
{
    return this->gid;
}

void SingalView::setTitle(QString *title)
{
    this->window->setWindowTitle(*(this->plgName) + " - " + *title);
}

void SingalView::placeView(QString *viewTitle, CView::I_ContentView *comp)
{
    this->setTitle(viewTitle);
    this->centralView = comp;
    this->window->setCentralWidget(comp->getWidget());
}

void SingalView::closeContentView(CView::I_ContentView *comp)
{
    if(this->window->centralWidget() != comp->getWidget())
        return;
    this->window->setWindowTitle(*(this->plgName));
    this->centralView = nullptr;
    this->window->setCentralWidget(new QWidget(this->window));
}

void SingalView::service_RefreshMenuBar(MBar::I_MenuBar *bar)
{
    //TODO
}

void SingalView::getActivedView(QList<CView::I_ContentView*> *views)
{
    views->append(this->centralView);
}


