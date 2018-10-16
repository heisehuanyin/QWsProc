#include "defaultmenubar.h"

using namespace PlgDef::MenuBar;

DefaultMenuBar::DefaultMenuBar()
{
}

DefaultMenuBar::~DefaultMenuBar(){
    delete this->mbar;
}

const QString DefaultMenuBar::registName(){
    return "DefaultMenuBar";
}

QMenu *DefaultMenuBar::getCustomMenu(){
    return nullptr;
}

void DefaultMenuBar::saveOperation(){}

I_MenuBar *DefaultMenuBar::createNewMenubar(Core::WsCore *core, QString groupId){
    DefaultMenuBar *rtn = new DefaultMenuBar;

    rtn->mbar = new QMenuBar;
    rtn->groupid = groupId;

    return rtn;
}

QMenuBar *DefaultMenuBar::refreshMenuBarWidget(QList<QMenu *> vlist){
    delete this->mbar;
    this->mbar = new QMenuBar;

    for(auto itor = vlist.constBegin();
        itor != vlist.constEnd();
        itor++){
        QMenu * menu = *itor;
        this->mbar->addMenu(menu);
    }

    return this->mbar;
}
