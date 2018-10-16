#include "singalview.h"

using namespace PlgDef::Window;
namespace MBar = PlgDef::MenuBar;
namespace CView = PlgDef::ContentView;

DefaultSingalView::DefaultSingalView():
    I_Window (),
    plgName("DefaultSingalView"),
    window(new _CustomWindow()),
    list(new QList<PlgDef::ContentView::I_ContentView *>())
{
    this->connect(this->window, &_CustomWindow::resizeWindow,
                  this, &DefaultSingalView::receiveWindowResize);
}

DefaultSingalView::~DefaultSingalView()
{
    delete this->window;
    delete this->list;
}

const QString DefaultSingalView::registName()
{
    return this->plgName;
}

QMenu *DefaultSingalView::getCustomMenu()
{
    return nullptr;
}

void DefaultSingalView::saveOperation()
{
    //TODO saveOperate
}

PlgDef::Window::I_Window *DefaultSingalView::openNewWindow(Core::WsCore *core, const QString gid, int width, int height)
{
    DefaultSingalView* rtn = new DefaultSingalView();
    rtn->core = core;
    rtn->gid = gid;
    rtn->window->resize(width, height);
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

void DefaultSingalView::service_ReplaceMenuBar(QMenuBar *bar)
{
    this->window->setMenuBar(bar);
}

QList<CView::I_ContentView *> * DefaultSingalView::getActivedView()
{
    this->list->clear();
    this->list->append(this->centralView);
    return this->list;
}

void DefaultSingalView::setSize(int width, int height)
{
    this->window->resize(width, height);
}

QWidget *DefaultSingalView::getWidget()
{
    return this->window;
}

QList<PlgDef::ContentView::I_ContentView *> *DefaultSingalView::getAllView()
{
    return this->getActivedView();
}


