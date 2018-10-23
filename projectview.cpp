#include "projectview.h"
#include "wsnovelmanager.h"

using namespace PlgDef::ContentView;


DefaultProjectView::DefaultProjectView():
    view(nullptr)
{
}

DefaultProjectView::~DefaultProjectView(){
    if(this->view)
        delete this->view;
}

const QString DefaultProjectView::registName(){
    return "DefaultProjectView";
}

QMenu *DefaultProjectView::getCustomMenu(){
    return nullptr;
}

void DefaultProjectView::saveOperation(){}

PlgDef::PluginType DefaultProjectView::upStreamMark(){
    return PlgDef::Service_ProjectManager;
}

QWidget *DefaultProjectView::reloadContentAndReturnSetupWidget(){
    return nullptr;
}

I_ContentView *DefaultProjectView::createNewContentView(Core::WsCore *core, PlgDef::I_PluginBase *upStream, QHash<QString, QString> , PlgDef::Window::I_Window *win){
    auto rtn(new DefaultProjectView);

    rtn->core = core;
    rtn->view = new QTreeView;
    rtn->upStream = dynamic_cast<ProjectManager::I_ProjectManager*>(upStream);
    rtn->view->setModel(rtn->upStream->getTreeModel());
    rtn->win = win;

    this->connect(rtn->view,     &QTreeView::clicked,
                  rtn->upStream, &ProjectManager::I_ProjectManager::slot_clicked);
    this->connect(rtn->view,     &QTreeView::doubleClicked,
                  rtn->upStream, &ProjectManager::I_ProjectManager::slot_doubleClicked);

    return rtn;
}

QWidget *DefaultProjectView::getWidget(){
    return this->view;
}

PlgDef::Window::I_Window *DefaultProjectView::getOwner(){
    return this->win;
}
