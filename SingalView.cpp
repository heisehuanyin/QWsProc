#include "SingalView.h"

using namespace PlgDef::Window;
namespace MBar = PlgDef::MenuBar;
namespace CView = PlgDef::ContentView;

DefaultSingalView::DefaultSingalView():I_Window (),
    plgName("DefaultSingalView"),
    window(new _CustomWindow()),
    list(new QList<PlgDef::ContentView::I_ContentView *>())
{

}

DefaultSingalView::~DefaultSingalView()
{
    delete this->window;
}

const QString DefaultSingalView::registName()
{
    return this->plgName;
}


PlgDef::PluginType DefaultSingalView::upStreamMark()
{
    return PlgDef::IO_NoUpStream;
}

QMenu *DefaultSingalView::getCustomMenu()
{
    return nullptr;
}

void DefaultSingalView::saveOperation()
{
    //TODO saveOperate
}

PlgDef::Window::I_Window *DefaultSingalView::openNewWindow(Core::WsCore *core, const QString gid)
{
    DefaultSingalView* rtn = new DefaultSingalView();
    rtn->core = core;
    rtn->gid = gid;
    rtn->window->show();

    return rtn;
}

const QString DefaultSingalView::getGroupID()
{
    return this->gid;
}

void DefaultSingalView::setTitle(const QString title)
{
    this->window->setWindowTitle(this->plgName + " - " + title);
}

void DefaultSingalView::placeView(const QString viewTitle, CView::I_ContentView *comp)
{
    this->setTitle(viewTitle);
    this->centralView = comp;
    this->window->setCentralWidget(comp->getWidget());
}

void DefaultSingalView::closeContentView(CView::I_ContentView *comp)
{
    if(this->window->centralWidget() != comp->getWidget())
        return;
    this->window->setWindowTitle(this->plgName);
    this->centralView = nullptr;
    this->window->setCentralWidget(new QWidget(this->window));
}

void DefaultSingalView::service_RefreshMenuBar(MBar::I_MenuBar *bar)
{
    //TODO
}

QList<CView::I_ContentView *> * DefaultSingalView::getActivedView()
{
    this->list->clear();
    this->list->append(this->centralView);
    return this->list;
}


