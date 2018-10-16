#include "customdialog.h"

#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>
#include <QTableWidget>

using namespace Core;
using namespace Core::CustomBase;

namespace Cfg = PlgDef::ConfigPort;

CustomDialog::CustomDialog(Core::WsCore *core, Cfg::I_ConfigPort* configPort, QWidget *parent) :
    QDialog(parent),
    mainCfg(configPort),
    layout(new QHBoxLayout(this)),
    tab(new QTabWidget),
    uicfg(new QWidget),
    g_cfg(new QWidget),
    core(core)
{
    this->setWindowTitle(tr("配置面板"));
    this->setLayout(this->layout);
    this->layout->addWidget(this->tab);

    this->tab->addTab(g_cfg, tr("框架配置"));
    //CUSTOM GLOBLE ARGUMENTS=============================================================





    this->tab->addTab(uicfg, tr("UI组件配置"));
    //CUSTOM UICFG========================================================================
    QVBoxLayout* baselayout = new QVBoxLayout;
    QGridLayout* grid = new QGridLayout;
    this->uicfg->setLayout(baselayout);
    baselayout->addLayout(grid);

    auto lists = this->core->service_getManager()->service_QueryFactoryList();
    QList<QPair<QString,PlgDef::PluginType>> list;

    grid->addWidget(new QLabel(tr("默认窗口：")), 0,0);
    auto defwin = new QComboBox();
    if(lists.contains(PlgDef::UI_Window)){
        list = lists.find(PlgDef::UI_Window).value();
        for(auto itor = list.constBegin();
            itor != list.constEnd();
            ++itor){
            defwin->addItem((*itor).first);
        }}
    grid->addWidget(defwin, 0, 1, 1, 3);
    this->connect(defwin, &QComboBox::currentTextChanged, this, &CustomDialog::defaultWindowType);

    grid->addWidget(new QLabel(tr("默认菜单栏：")), 1,0);
    auto defmbar = new QComboBox();
    if(lists.contains(PlgDef::UI_MenuBar)){
        list = lists.find(PlgDef::UI_MenuBar).value();
        for(auto itor = list.constBegin();
            itor != list.constEnd();
            ++itor){
            defmbar->addItem((*itor).first);
        }}
    grid->addWidget(defmbar, 1, 1, 1, 3);
    this->connect(defmbar, &QComboBox::currentTextChanged, this, &CustomDialog::defaultMenuBarType);

    grid->addWidget(new QLabel(tr("默认工具栏：")), 2,0);
    auto deftbar =new QComboBox();
    if(lists.contains(PlgDef::UI_ToolBar)){
        list = lists.find(PlgDef::UI_ToolBar).value();
        for(auto itor=list.constBegin();
            itor != list.constEnd();
            ++itor){
            deftbar->addItem((*itor).first);
        }
    }
    grid->addWidget(deftbar, 2, 1, 1, 3);
    this->connect(deftbar, &QComboBox::currentTextChanged, this, &CustomDialog::defaultToolBarType);

    grid->addWidget(new QLabel(tr("默认状态栏：")), 3,0);
    auto defsbar = new QComboBox();
    if(lists.contains(PlgDef::UI_StateBar)){
        list = lists.find(PlgDef::UI_ToolBar).value();
        for(auto itor=list.constBegin();
            itor != list.constEnd();
            ++itor){
            defsbar->addItem((*itor).first);
        }
    }
    grid->addWidget(defsbar, 3, 1, 1, 3);
    this->connect(defsbar, &QComboBox::currentTextChanged, this, &CustomDialog::defaultStatusBarType);

    grid->addWidget(new QLabel(tr("默认浮动面板：")), 4,0);
    auto defpal = new QComboBox();
    if(lists.contains(PlgDef::UI_FloatPanel)){
        list = lists.find(PlgDef::UI_ToolBar).value();
        for(auto itor=list.constBegin();
            itor != list.constEnd();
            ++itor){
            defpal->addItem((*itor).first);
        }
    }
    grid->addWidget(defpal, 4, 1, 1, 3);
    this->connect(defpal, &QComboBox::currentTextChanged, this, &CustomDialog::defaultFloatPanelType);
    baselayout->addStretch();

    // CUSTOM FILEPARSER
    this->setFileParserCustomPane(mainCfg, tr("全局文件解析"));


    this->tab->setCurrentIndex(0);
}

CustomDialog::~CustomDialog()
{
}

void CustomDialog::setFileParserCustomPane(PlgDef::ConfigPort::I_ConfigPort *customTarget, const QString title, QString fileAtPjt)
{
    auto cfg_p = new QWidget;

    auto fplayout = this->getFileParsePanel(customTarget);
    cfg_p->setLayout(fplayout);
    if(fileAtPjt != QString()){
        fplayout->config4File(fileAtPjt);
    }

    int index = this->tab->count();
    this->tab->addTab(cfg_p, title);
    this->tab->setCurrentIndex(index);
}

void CustomDialog::defaultWindowType(const QString type)
{
    this->core->instance_GetMainConfigPort()
            ->setKeyValue(PlgDef::ConfigPort::DefaultWindowType_Key, type);
}

void CustomDialog::defaultMenuBarType(const QString type)
{
    this->core->instance_GetMainConfigPort()
            ->setKeyValue(PlgDef::ConfigPort::DefaultMenuBarType_Key,type);
}

void CustomDialog::defaultToolBarType(const QString type)
{

}

void CustomDialog::defaultStatusBarType(const QString type)
{

}

void CustomDialog::defaultFloatPanelType(const QString type)
{

}

FileParsePanel *CustomDialog::getFileParsePanel(PlgDef::ConfigPort::I_ConfigPort *cfg_port)
{
    return new FileParsePanel(this->core, cfg_port);
}

FileParsePanel::FileParsePanel(Core::WsCore *core, PlgDef::ConfigPort::I_ConfigPort *cfg_port):
    _port(cfg_port),
    core(core),
    isSuffix(new QCheckBox(tr("使用文件路径"))),
    input(new QLineEdit(tr("输入后缀名（aa.bb.cc.xx输入bb.cc.xx）"))),
    click(new QPushButton(tr("查询"))),
    branches(new QTableView),
    addBranchMod(new QPushButton(tr("添加插件节点"))),
    addViewBranch(new QPushButton(tr("添加监视分支"))),
    removeBranch(new QPushButton("删除节点及子节点")),
    arguments(new QListView)
{

    auto _1row = new QGridLayout;
    _1row->addWidget(this->isSuffix, 0,0,1,1);
    this->connect(this->isSuffix, &QCheckBox::stateChanged, this, &FileParsePanel::cfgCheck);
    _1row->addWidget(this->input,0,1,1,3);
    _1row->addWidget(this->click, 0, 4);
    this->addLayout(_1row);
    this->setStretchFactor(_1row, 0);

    auto _2row = new QGridLayout;
    _2row->addWidget(this->branches, 0, 0, 1, 3);
    _2row->addWidget(this->arguments, 0, 3, 2, 1);
    _2row->addWidget(this->addBranchMod, 1, 0, 1, 1);
    _2row->addWidget(this->addViewBranch, 1, 1, 1, 1);
    _2row->addWidget(this->removeBranch, 1, 2, 1, 1);

    this->addLayout(_2row);



    this->addBranchMod->hide();
    this->addViewBranch->hide();
    this->removeBranch->hide();
}

FileParsePanel::~FileParsePanel(){
    delete this->isSuffix;
    delete this->input;
    delete this->click;
    delete this->branches;
    delete this->addBranchMod;
    delete this->addViewBranch;
    delete this->removeBranch;
    delete this->arguments;
}

void FileParsePanel::config4File(QString key)
{
    this->addBranchMod->setVisible(true);
    this->addViewBranch->setVisible(true);
    this->removeBranch->setVisible(true);
}

void FileParsePanel::cfgCheck(int state)
{
    if(state == Qt::Checked){
        if(this->_port == this->core->instance_GetMainConfigPort()){
            this->core->slot_Recieve_ProcessError(nullptr, "此为全局配置面板，不可配置单个文档！", true);
            this->isSuffix->setCheckState(Qt::Unchecked);
        }else{
            this->input->setText("输入文件完全路径（/aa/bb.xx)");
        }
    }else{
        this->input->setText("输入后缀名（aa.bb.cc.xx输入bb.cc.xx）");
    }
}

void FileParsePanel::setBranchTable()
{

}

void FileParsePanel::setArgsList()
{

}

void FileParsePanel::addBranchModule()
{

}

void FileParsePanel::addViewBranchM()
{

}

void FileParsePanel::removeBranchNext()
{

}

int FPP_BranchesDataModel::columnCount(const QModelIndex &parent) const{
    return 3;
}

QVariant FPP_BranchesDataModel::headerData(int section, Qt::Orientation orientation, int role) const{
    if(orientation == Qt::Vertical)
        return QVariant();
    switch (section) {
    case 0:return QString("分支标志");
    case 1:return QString("模块名");
    default:return QString("上游模块");
    }
}

Qt::ItemFlags FPP_BranchesDataModel::flags(const QModelIndex &index) const{
    if(!index.isValid())
        return Qt::ItemIsEnabled;
    return QAbstractItemModel::flags(index) | Qt::ItemIsEnabled;
}
