#include "wsnovelmanager.h"

#include "def_channelpreface.h"

#include <QFile>
#include <QFileInfo>
#include <QTextStream>

#define XML_GROUPS_TAG "group"
#define XML_FILES_TAG "node"
#define XML_NAME_KEY "name"
#define XML_GROUPSPATH_KEY "grouppath"
#define XML_FILEPATH_KEY "filepath"

using namespace PlgDef::ProjectManager;

WsNovelProjectManager::WsNovelProjectManager():
    customMenu(new QMenu("WsNovel")),
    doc(nullptr),
    upStream(nullptr),
    core(nullptr),
    model(new QStandardItemModel)
{
    this->connect(model, &QAbstractItemModel::rowsInserted,
                  this, &WsNovelProjectManager::slot_NodeIsInserted);
    this->connect(model, &QAbstractItemModel::rowsRemoved,
                  this, &WsNovelProjectManager::slot_NodeIsRemoved);
    this->connect(model, &QAbstractItemModel::rowsMoved,
                  this, &WsNovelProjectManager::slot_NodeIsMoved);
    this->connect(model, &QAbstractItemModel::dataChanged,
                  this, &WsNovelProjectManager::slot_NodeIsModified);
}

WsNovelProjectManager::~WsNovelProjectManager()
{
    if(doc)
        delete doc;
    if(upStream)
        delete upStream;
}

void WsNovelProjectManager::parseDocForDetail(QDomElement elmp, QStandardItem *itemp)
{
    auto children = elmp.childNodes();
    for(int i=0;i<children.length();++i){
        auto elm = children.at(i).toElement();
        auto item = new WsNovelBase::NodeWithDomElement(this->doc, elm);

        if(elm.tagName() == XML_GROUPS_TAG){
            itemp->appendRow(item);
            this->parseDocForDetail(elm, item);
        }else if(elm.tagName() == XML_FILES_TAG){
            itemp->appendRow(item);
        }
    }
}

void WsNovelProjectManager::slot_NodeIsRemoved(const QModelIndex &parent, int first, int last)
{

}

void WsNovelProjectManager::slot_NodeIsInserted(const QModelIndex &parent, int first, int last)
{

}

void WsNovelProjectManager::slot_NodeIsMoved(const QModelIndex &parent, int start, int end, const QModelIndex &destination, int row)
{

}

void WsNovelProjectManager::slot_NodeIsModified(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles)
{

}

const QString WsNovelProjectManager::registName(){
    return "WsNovelProjectManager";
}

QMenu *WsNovelProjectManager::getCustomMenu()
{
    return this->customMenu;
}

void WsNovelProjectManager::saveOperation()
{
    if(!this->doc){
        emit this->signal_PushErrorReport(this, "保存过程出现问题：doc为nullptr");
    }
    if(!this->upStream){
        emit this->signal_PushErrorReport(this, "保存过程出现问题：upstream为nullptr");
    }
    auto filePath = this->upStream->getFilePath();
    QFile xpoint(filePath);
    if(!xpoint.open(QIODevice::WriteOnly | QIODevice::Text)){
        emit this->signal_PushErrorReport(this, "目标文件写入过程中打开文件出错");
    }
    QTextStream out(&xpoint);
    this->doc->save(out, 4);
    xpoint.close();
}

PlgDef::PluginType WsNovelProjectManager::upStreamMark()
{
    return PlgDef::IO_ChannelPreface;
}

QWidget *WsNovelProjectManager::reloadContentAndReturnSetupWidget()
{
    return nullptr;
}

QString WsNovelProjectManager::suffix()
{
    return "wsnovel";
}

I_ProjectManager *WsNovelProjectManager::openExistProject(Core::WsCore *core, PlgDef::I_PluginBase *upStream, QHash<QString, QString> xargs)
{
    auto rtn(new WsNovelProjectManager);

    rtn->upStream = dynamic_cast<PlgDef::ChannelPreface::I_ChannelPreface*>(upStream);
    rtn->core = core;
    rtn->doc = new QDomDocument(rtn->upStream->getFilePath());

    QFile b_in(rtn->upStream->getFilePath());
    if(!b_in.open(QIODevice::ReadOnly|QIODevice::Text)){
        emit this->signal_PushErrorReport(this, "项目文件打开过程中出错");
    }
    if(!rtn->doc->setContent(&b_in)){
        b_in.close();
        emit this->signal_PushErrorReport(this, "项目文件读取过程中出错");
    }

    QStandardItem *root = new QStandardItem(b_in.fileName());
    rtn->model->invisibleRootItem()->appendRow(root);
    rtn->parseDocForDetail(rtn->doc->documentElement(), root);
    return rtn;
}

QString WsNovelProjectManager::getEmptyDocument()
{
    QString content = "<?xml version='1.0'?>"
                      "<wsnovel>"
                      "</wsnovel>";
    return content;
}

void travel4AvaliableGroups(QString groupsTemp, QDomElement elm, QList<QString> &groupNames){
    auto list = elm.childNodes();
    for(int i=0; i<list.length(); ++i){
        auto celm = list.at(i).toElement();
        if(celm.tagName() == XML_GROUPS_TAG){
            groupsTemp += "/" + celm.attribute(XML_NAME_KEY);
            travel4AvaliableGroups(groupsTemp, celm, groupNames);
        }else if (celm.tagName() == XML_FILES_TAG) {
            groupNames.append(groupsTemp);
        }
    }
}

QList<QString> WsNovelProjectManager::getAvaliableGroups()
{
    QList<QString> rtn;

    auto root = this->doc->documentElement();
    QString m(":");
    travel4AvaliableGroups(m, root, rtn);

    return rtn;
}

QStandardItem* loop_Search4Target(QStringList gens, QStandardItem* parent){
    auto nodeName = gens.takeAt(0);
    for(int i=0; i<parent->rowCount(); ++i){
        auto x = dynamic_cast<WsNovelBase::NodeWithDomElement*>(parent->child(i));
        if(x->getNodeName() == nodeName){
            if(gens.size()==0){
                return x;
            }else{
                loop_Search4Target(gens, x);
            }
        }
    }
    return nullptr;
}

QString WsNovelProjectManager::translate_Insert(bool transOnly, QString fileName, QString groupName)
{
    if(!this->getAvaliableGroups().contains(groupName))
        return QString();

    auto filepath = this->upStream->getFilePath();
    auto fullpath = QFileInfo(filepath).canonicalPath() + "/" + fileName;

    if(!transOnly){
        auto pjtRoot = this->model->invisibleRootItem()->child(0);
        groupName.replace(":","");
        auto gens = groupName.split("/");

        auto elm = this->doc->createElement(XML_FILES_TAG);
        elm.setAttribute(XML_NAME_KEY, fileName);
        elm.setAttribute(XML_FILEPATH_KEY, fullpath);

        auto target(new WsNovelBase::NodeWithDomElement(this->doc, elm));
        auto ptarget = loop_Search4Target(gens, pjtRoot);
        ptarget->appendRow(target);
    }

    return fullpath;
}

QAbstractItemModel *WsNovelProjectManager::getTreeModel()
{
    return this->model;
}

QMenu *WsNovelProjectManager::getContextMenu(QModelIndex &index)
{
    return nullptr;
}


void WsNovelProjectManager::slot_FileIsOpening(QString filePath)
{

}

void WsNovelProjectManager::slot_FileIsClosed(QString filePath)
{

}

void WsNovelProjectManager::slot_doubleClicked(const QModelIndex &index)
{

}

void WsNovelProjectManager::slot_clicked(const QModelIndex &index)
{

}

WsNovelBase::NodeWithDomElement::NodeWithDomElement(QDomDocument *doc, QDomElement elm):
    QStandardItem (elm.attribute(XML_NAME_KEY)),
    doc(doc),
    elm(elm){}

WsNovelBase::NodeWithDomElement::~NodeWithDomElement()
{}

QString WsNovelBase::NodeWithDomElement::getNodeName(){
    return this->elm.attribute(XML_NAME_KEY);
}

QDomDocument* WsNovelBase::NodeWithDomElement::getDocument(){
    return this->doc;
}

QDomElement WsNovelBase::NodeWithDomElement::getElement(){
    return this->elm;
}
