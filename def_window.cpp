#include "def_window.h"

PlgDef::Window::_CustomWindow::_CustomWindow(QWidget *parent):
    QMainWindow(parent)
{}

PlgDef::Window::_CustomWindow::~_CustomWindow() {}

void PlgDef::Window::_CustomWindow::resizeEvent(QResizeEvent *event) {
    this->QMainWindow::resizeEvent(event);
    auto size = event->size();
    emit this->resizeWindow(size.width(), size.height());
}

PlgDef::Window::I_Window::I_Window()
{
}

PlgDef::Window::I_Window::~I_Window() {}

PlgDef::PluginType PlgDef::Window::I_Window::pluginMark(){
    return PlgDef::UI_Window;
}

PlgDef::PluginType PlgDef::Window::I_Window::upStreamMark()
{
    return PlgDef::IO_NoUpStream;
}
