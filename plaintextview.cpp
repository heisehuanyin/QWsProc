#include "plaintextview.h"
#include "def_textmodel.h"

using namespace PlgDef::ContentView;

PlainTextView::PlainTextView():
    editArea(new QTextEdit)
{

}

PlainTextView::~PlainTextView() {
    delete editArea;
}

QMenu *PlainTextView::getCustomMenu()
{
    return nullptr;
}

void PlainTextView::saveOperation()
{
    auto content = this->editArea->toPlainText();
    this->textmodel->reLoadAllText(content);
}

I_ContentView *PlainTextView::createNewContentView(Core::WsCore *core, PlgDef::I_PluginBase *upStream,
                                                   QHash<QString, QString> xargs, PlgDef::Window::I_Window* win)
{
    auto rtn = new PlainTextView;

    rtn->owner = win;
    rtn->core = core;
    rtn->textmodel = dynamic_cast<PlgDef::TextModel::I_TextModel*>(upStream);

    for(int i=0;i<rtn->textmodel->getRowsCount(); ++i){
        QString line = rtn->textmodel->getLineContent(i);
        rtn->editArea->append(line);
    }

    return rtn;
}

QWidget *PlainTextView::getWidget()
{
    return this->editArea;
}

PlgDef::Window::I_Window *PlainTextView::getOwner()
{
    return this->owner;
}

PlgDef::PluginType PlainTextView::upStreamMark() {
    return PlgDef::IO_TextModel;
}

QWidget *PlainTextView::reloadContentAndReturnSetupWidget()
{
    return nullptr;
}
