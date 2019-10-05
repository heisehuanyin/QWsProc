#include "singalview.h"

using namespace PlgDef::Window;
namespace MBar = PlgDef::MenuBar;
namespace CView = PlgDef::ContentView;

DefaultSingalView::DefaultSingalView():
    I_Window (),
    plgName("DefaultSingalView"),
    window(new _CustomWindow()),
    list(new QHash<PlgDef::ContentView::I_ContentView *, QString>()),
    stack(new QStackedLayout),
    basePanel(new QWidget)
{
    this->connect(this->window, &_CustomWindow::resizeWindow,
                  this, &DefaultSingalView::receiveWindowResize);
    this->window->setCentralWidget(this->basePanel);
    this->basePanel->setLayout(this->stack);
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
    this->window->setWindowTitle(title + " - " + this->plgName);
}

void DefaultSingalView::appendView(const QString viewTitle, CView::I_ContentView *comp)
{
    if(! this->list->contains(comp)){
        this->list->insert(comp, viewTitle);
        this->stack->addWidget(comp->getWidget());
    }
    this->stack->setCurrentWidget(comp->getWidget());
    this->setTitle(viewTitle);
}

void DefaultSingalView::removeView(PlgDef::ContentView::I_ContentView *cmop)
{
    if(this->list->contains(cmop)){
        this->list->remove(cmop);
    }
}

void DefaultSingalView::FocuseAt(PlgDef::ContentView::I_ContentView *comp)
{
    if(this->list->contains(comp)){
        this->stack->setCurrentWidget(comp->getWidget());
        this->setTitle(this->list->value(comp));
    }
}

void DefaultSingalView::replaceMenuBar(QMenuBar *bar)
{
    this->window->setMenuBar(bar);
}

QList<CView::I_ContentView *> DefaultSingalView::getActivedView()
{
    QList<CView::I_ContentView *> list;
    for(auto itor = this->list->constBegin();
        itor != this->list->constEnd();
        ++itor){
        auto item = itor.key();
        if(item->getWidget() == this->stack->currentWidget()){
            list.append(item);
            break;
        }
    }
    return list;
}

void DefaultSingalView::setSize(int width, int height)
{
    this->window->resize(width, height);
}

QWidget *DefaultSingalView::getWidget()
{
    return this->window;
}

QList<PlgDef::ContentView::I_ContentView *> DefaultSingalView::getAllView()
{
    return this->list->keys();
}


