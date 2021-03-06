#include "customdialog.h"

#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QPushButton>
#include <QTableWidget>
#include <QDebug>
#include <QSplitter>

using namespace Core;
using namespace Core::CoreBase;

namespace Cfg = PlgDef::ConfigPort;

CustomDialog::CustomDialog(Core::WsCore *core, Cfg::I_ConfigPort* configPort, QWidget *parent) :
    QDialog(parent),
    mainCfg(configPort),
    layout(new QHBoxLayout(this)),
    tab(new QTabWidget),
    uicfg(new QWidget),
    g_cfg(new QWidget),
    core(core),
    defaultpath(new QLineEdit),
    explor(new QPushButton(tr("替换")))
{
    this->setWindowTitle(tr("配置面板"));
    this->setLayout(this->layout);
    this->layout->addWidget(this->tab);

    this->tab->addTab(g_cfg, tr("框架配置"));
    //CUSTOM GLOBLE ARGUMENTS=============================================================
    auto *base(new QVBoxLayout);
    g_cfg->setLayout(base);
    base->addWidget(new QLabel(tr("软件默认项目路径：")));
    auto row1(new QHBoxLayout);
    row1->addWidget(this->defaultpath);
    auto path = this->mainCfg->getValue(Cfg::DefaultProjectCfgFilePath_Key);
    this->defaultpath->setText(path);
    row1->addWidget(this->explor);
    base->addLayout(row1);
    base->addStretch();


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
        fplayout->outSetFilePathIndependentConfig(fileAtPjt);
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
    queryType(new QComboBox),
    input(new QLineEdit(tr("输入后缀名（aa.bb.cc.xx输入bb.cc.xx）"))),
    click(new QPushButton(tr("查询"))),
    modules(new QTableView),
    msg(new QLabel(":")),
    cfgCheck(new QPushButton(tr("应用"))),
    argslist(new QListView),
    tableModel(nullptr),
    listModel(nullptr)
{
    this->queryType->addItem(QString("文件路径"));
    this->queryType->addItem(QString("文件后缀"));
    this->queryType->addItem(QString("项目默认"));
    this->queryType->setCurrentIndex(1);

    auto _1row = new QGridLayout;
    _1row->addWidget(this->queryType, 0,0,1,1);
    this->connect(this->queryType, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
                  this, &FileParsePanel::slot_QueryTypeChanged);
    _1row->addWidget(this->input,0,1,1,3);
    _1row->addWidget(this->click, 0, 4);
    this->connect(this->click, &QPushButton::pressed, this, &FileParsePanel::slot_Click_Query);
    this->addLayout(_1row);
    this->setStretchFactor(_1row, 0);

    auto _2row = new QSplitter;
    _2row->addWidget(this->modules);
    _2row->addWidget(this->argslist);

    this->addWidget(_2row);

    auto _3row = new QHBoxLayout;
    _3row->addWidget(this->msg);
    _3row->addWidget(this->cfgCheck);
    this->cfgCheck->setEnabled(false);
    this->connect(this->cfgCheck, &QPushButton::pressed, this, &FileParsePanel::slot_ConfigResultSave);
    this->addLayout(_3row);

    this->tableModel = new FPP_TableDataModel(this->core, this->_port);
    this->modules->setModel(tableModel);
    auto *d(new FPP_TableDelegate(this->core));
    this->modules->setItemDelegateForColumn(1,d);

    this->listModel = new FPP_ListDataModel();
    this->argslist->setModel(this->listModel);
    this->listDelegate = new FPP_ListDelegate();
    this->argslist->setItemDelegate(this->listDelegate);

    this->connect(this->tableModel, &FPP_TableDataModel::signal_SaveOperateEnable,
                  this->cfgCheck, &QPushButton::setEnabled);

    auto selectModel = this->modules->selectionModel();
    this->connect(selectModel, &QItemSelectionModel::currentChanged,
                  this, &FileParsePanel::slot_TableCurrentSelectedChanged);
}

FileParsePanel::~FileParsePanel(){
    delete this->queryType;
    delete this->input;
    delete this->click;
    delete this->modules;
    delete this->msg;
    delete this->argslist;
}

void FileParsePanel::config4Keywords(QString key)
{
    Core::CoreBase::PluginListNode *list;
    QString strResult = this->core->service_getPluginlistdefAtCfgport(key, _port, &list);
    if(strResult == QString()){
        strResult = "无定义";
        list = new CoreBase::PluginListNode(tr("填写值__双击编辑__"),nullptr);
    }
    this->msg->setText(strResult);
    this->cfgCheck->setEnabled(false);
    this->tableModel->refreshTableModel(key, list);
}

void FileParsePanel::outSetFilePathIndependentConfig(QString filepath)
{
    this->queryType->setCurrentIndex(0);
    this->input->setText(filepath);
    this->click->click();
}

void FileParsePanel::slot_QueryTypeChanged(int index)
{
    this->input->setReadOnly(false);
    if(index == 0){
        if(this->_port == this->core->instance_GetMainConfigPort()){
            this->core->slot_Recieve_ProcessError(nullptr, "此为全局配置面板，不可配置单个文档！", true);
            this->queryType->setCurrentIndex(2);
        }else{
            this->input->setText("清空后输入文件完全路径（如：/aa/bb.xx)");
        }
    }
    if(index == 1){
        this->input->setText("清空后输入后缀名（如：aa.bb.cc输入bb.cc）");
    }
    if(index == 2){
        this->input->setText(":default");
        this->input->setReadOnly(true);
        this->click->click();
    }
}

void FileParsePanel::slot_Click_Query()
{
    auto text = this->input->text();
    int type = this->queryType->currentIndex();
    QString keyWords;
    switch (type) {
    case 0:{
            auto pjtcfgpath = this->core->service_getManager()
                    ->channel_getChannelId(this->_port);
            auto fpath = this->core->service_ProcFilePath(text, pjtcfgpath);
            fpath = PlgDef::ConfigPort::EncodeFromMsg_4_KeyExport(fpath);
            this->config4Keywords(fpath);
        }break;
    case 1:{
            auto suffix = this->core->service_Proc4Suffix(text);
            suffix = PlgDef::ConfigPort::EncodeFromMsg_4_KeyExport(suffix);
            this->config4Keywords(suffix);
        }break;
    case 2:{
            auto x = PlgDef::ConfigPort::EncodeFromMsg_4_KeyExport("____default");
            this->config4Keywords(x);
        }
    }
    this->modules->resizeColumnsToContents();
}

void FileParsePanel::slot_ConfigResultSave()
{
    if(this->tableModel){
        auto result = this->tableModel->saveConfigResult();
        if(result)
            this->msg->setText("配置成功");
        else
            this->msg->setText("配置结果错误：Table配置未完成");
    }else{
        this->msg->setText("配置过程出错：Table模型未建立");
    }
}

void FileParsePanel::slot_TableCurrentSelectedChanged(const QModelIndex &current, const QModelIndex &)
{
    //=====================================
    auto model = current.model();
    if(model == nullptr)
        return;
    auto node = dynamic_cast<const FPP_TableDataModel*>(model)->getInnerNode(current.row());
    auto name = node->getPluginName();
    auto f = this->core->service_getManager()
            ->factory_GetExistsFactory(name);
    if(!f)
        return;
    if((f->pluginMark() | PlgDef::Feature_Configurable) == f->pluginMark()){
        auto factory = dynamic_cast<PlgDef::I_Configurable*>(f);
        this->listModel->refreshDataModel(node, factory);
        this->listDelegate->refreshListViewDelegate(node, factory);
    }
    else{
        this->core->slot_Recieve_ProcessError(nullptr, "该模块不可配置", true);
    }
    this->modules->resizeColumnsToContents();
}



FPP_TableDataModel::FPP_TableDataModel(Core::WsCore * core,
                                       Cfg::I_ConfigPort* port):
    core(core),
    _port(port)
{}

FPP_TableDataModel::~FPP_TableDataModel()
{
    for(auto it = this->list.constBegin();
        it != this->list.constEnd();
        ++it){
        delete *it;
    }
}

PluginListNode *FPP_TableDataModel::getInnerNode(int row) const
{
    return this->list.at(row);
}

bool FPP_TableDataModel::saveConfigResult()
{
    if(this->list.last()->getPluginName() != PlgDef::ConfigPort::DefaultChannelPreface_Value)
        return false;
    PluginListNode* listdef = nullptr;
    PluginListNode* _previous = nullptr;
    for(int i=0; i<this->list.length(); ++i){
        int target_index = this->list.length()-1 -i;
        auto templateNode = this->list.at(target_index);
        auto newNode = new PluginListNode(templateNode->getPluginName(), _previous);
        newNode->mergeArgsList(*templateNode->getArgsList());
        if(_previous){
            _previous->setNextNode(newNode);
        }else {
            listdef = newNode;
        }
        _previous = newNode;
    }

    this->core->service_setPluginlistdefAtCfgport(listdef, this->keyString, this->_port);
    this->core->slot_Recieve_ProcessNomarlMsg(nullptr, "配置写入完成");

    delete listdef;
    return true;
}

void FPP_TableDataModel::refreshTableModel(QString keyString, PluginListNode *nodelist)
{
    this->beginResetModel();
    this->list.clear();
    for(auto node = nodelist;node != nullptr;node = node->getNextNode()){
        if(this->list.size() > 0)
            this->list.at(0)->setNextNode(nullptr);
        this->list.insert(0,node);
    }
    this->keyString = keyString;
    this->endResetModel();
}

int FPP_TableDataModel::rowCount(const QModelIndex &parent) const
{
    return this->list.length();
}

int FPP_TableDataModel::columnCount(const QModelIndex &parent) const{
    return 3;
}

QVariant FPP_TableDataModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid() || index.row() >= this->list.length() || this->list.at(index.row()) == nullptr)
        return QVariant();
    if(role == Qt::DisplayRole){
        switch (index.column()) {
        case 0:return QString("B.1");
        case 1:return this->list.at(index.row())->getPluginName();
        case 2:{
                if(index.row() != 0){
                    return this->list.at(index.row() -1)->getPluginName();
                }else{
                    return QString("无");
                }
            }
        default:return QVariant();
        }
    }
    return QVariant();
}

bool FPP_TableDataModel::setData(const QModelIndex &index, const QVariant &value, int )
{
    emit this->signal_SaveOperateEnable(false);
    int i_index = index.row();
    int count = this->list.length() - i_index;
    this->removeRows(i_index, count, index.parent());
    this->insertRows(i_index, 1, index.parent());
    auto node = new PluginListNode(value.toString(), nullptr);
    this->list.replace(this->list.length()-1, node);

    if(value.toString() == PlgDef::ConfigPort::DefaultChannelPreface_Value){
        emit this->signal_SaveOperateEnable(true);
    }else{
        this->insertRows(i_index+1, 1, index.parent());
    }

    return true;
}

QVariant FPP_TableDataModel::headerData(int section, Qt::Orientation orientation, int role) const{
    if(orientation == Qt::Vertical)
        return QAbstractTableModel::headerData(section, orientation, role);

    switch (section) {
    case 0:return QString("B标志");
    case 1:return QString("模块名");
    case 2:return QString("上游模块");
    default:return QString("col:%1").arg(section);
    }
}

Qt::ItemFlags FPP_TableDataModel::flags(const QModelIndex &index) const{
    if(!index.isValid())
        return Qt::ItemIsEnabled;
    if(index.column() == 1){
        return QAbstractTableModel::flags(index)|Qt::ItemIsEditable;
    }
    return QAbstractTableModel::flags(index) | Qt::ItemIsEnabled;
}

bool FPP_TableDataModel::insertRows(int row, int count, const QModelIndex &parent)
{
    this->beginInsertRows(parent, row, row + count -1);

    if(row > this->list.length())
        return false;

    for(int i=0; i<count; ++i){
        auto node = new PluginListNode("待编辑",nullptr);
        this->list.insert(row+i,node);
    }

    this->endInsertRows();
    return true;
}

bool FPP_TableDataModel::removeRows(int row, int count, const QModelIndex &parent)
{
    this->beginRemoveRows(parent, row, row + count - 1);

    for(int i=0; i<count; ++i){
        auto node = this->list.takeAt(row);
        delete node;
    }

    this->endRemoveRows();
    return true;
}

FPP_TableDelegate::FPP_TableDelegate(Core::WsCore *core):
    core(core){}

FPP_TableDelegate::~FPP_TableDelegate(){}

QWidget *FPP_TableDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QComboBox *editor(new QComboBox(parent));

    return editor;
}

void FPP_TableDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    auto edr = qobject_cast<QComboBox*>(editor);
    auto mark = PlgDef::UI_ContentView;
    if(index.row() != 0){
        auto x = index.model()->data(index.sibling(index.row()-1,index.column())).toString();
        auto names = this->core->service_getManager()->service_QueryFactoryList(x);
        for(auto it = names.constBegin();
            it != names.constEnd();
            ++it){
            if((*it).first == x){
                mark = (*it).second;
            }
        }
    }

    auto names = this->core->service_getManager()->service_QueryFactoryList(mark);
    for(auto itor = names.constBegin();
        itor != names.constEnd();
        ++itor){
        auto name = (*itor).first;
        edr->addItem(name);
    }
}

void FPP_TableDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    auto edr = qobject_cast<QComboBox*>(editor);
    auto text = edr->currentText();
    model->setData(index, text);
}

void FPP_TableDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}

FPP_ListDataModel::FPP_ListDataModel():
    target(nullptr),
    factory(nullptr)
{}

FPP_ListDataModel::~FPP_ListDataModel()
{

}

void FPP_ListDataModel::refreshDataModel(PluginListNode *target, PlgDef::I_Configurable *factory)
{
    this->beginResetModel();
    this->target = target;
    this->factory = factory;
    this->endResetModel();
}

int FPP_ListDataModel::rowCount(const QModelIndex &) const
{
    if(!this->factory)
        return 0;
    auto args = this->factory->getArgsList();
    return args.size() * 2;
}

QVariant FPP_ListDataModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();
    if(role == Qt::DisplayRole){
        auto args = this->factory->getArgsList();
        auto keys = args.keys();

        switch (index.row() % 2) {
        case 0:return keys.at(index.row()/2);
        case 1:{
                auto key = keys.at(index.row()/2);
                auto value = this->target->getArgsList()->value(key);
                return value;
            }
        default:return QVariant();
        }
    }
    return QVariant();
}

bool FPP_ListDataModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(role == Qt::DisplayRole){
        auto vstr = value.toString();
        auto key = this->data(index.sibling(index.row()-1,index.column()), Qt::DisplayRole);
        auto x = this->target->getArgsList();
        x->insert(key.toString(), vstr);
    }
    return true;
}

Qt::ItemFlags FPP_ListDataModel::flags(const QModelIndex &index) const
{
    if(!index.isValid())
        return Qt::ItemIsEnabled;
    if((index.row()%2) == 1){
        return QAbstractListModel::flags(index)|Qt::ItemIsEditable;
    }
    return QAbstractListModel::flags(index)|Qt::ItemIsEnabled;
}

FPP_ListDelegate::FPP_ListDelegate()
{
}

FPP_ListDelegate::~FPP_ListDelegate()
{

}

void FPP_ListDelegate::refreshListViewDelegate(PluginListNode *target, PlgDef::I_Configurable *factory)
{
    this->target = target;
    if(factory)
        this->defargslist = factory->getArgsList();
    else
        this->defargslist.clear();
}

QWidget *FPP_ListDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &, const QModelIndex &index) const
{
    auto key = index.model()->data(index.sibling(index.row()-1, index.column()));
    auto argsdef = this->defargslist.value(key.toString());
    auto type = argsdef.value("type");
    if(type.toInt() == PlgDef::I_Configurable::ENUM_ITEM){
        return new QComboBox(parent);
    }else{
        return new QLineEdit(parent);
    }
}

void FPP_ListDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    auto key = index.model()->data(index.sibling(index.row()-1, index.column()));
    auto argsdef = this->defargslist.value(key.toString());
    auto value = this->target->getArgsList()->value(key.toString());

    if(editor->inherits("QComboBox")){
        auto ed = qobject_cast<QComboBox*>(editor);
        ed->addItem(value);

        for(auto it=argsdef.constBegin();
            it != argsdef.constEnd();
            ++it){
            if(it.key() == "type")
                continue;
            ed->addItem(it.value());
        }
        ed->setCurrentIndex(0);
    }else{
        auto input = qobject_cast<QLineEdit*>(editor);
        input->setText(value);
    }
}

void FPP_ListDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    auto key = model->data(index.sibling(index.row()-1, index.column()));
    QString textValue;
    if(editor->inherits("QComboBox")){
        auto ed = qobject_cast<QComboBox*>(editor);
        textValue = ed->currentText();
    }else {
        auto input = qobject_cast<QLineEdit*>(editor);
        textValue = input->text();
    }
    if(textValue == QString())
        return;
    this->target->insertArgs(key.toString(),textValue);
}

void FPP_ListDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}
