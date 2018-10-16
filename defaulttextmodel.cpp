#include "def_channelpreface.h"
#include "defaulttextmodel.h"

#include <QFile>
#include <QTextCodec>
#include <QTextStream>

using namespace PlgDef::TextModel;

#define ENCODING "Encoding"

DefaultTextModel::DefaultTextModel()
{
    auto list = QTextCodec::availableCodecs();

    for(auto itor = list.constBegin();
        itor != list.constEnd();
        ++itor){
        this->insertEnumItem(ENCODING, *itor);
    }
}

QMenu *DefaultTextModel::getCustomMenu()
{
    return nullptr;
}

void DefaultTextModel::saveOperation()
{
    if(!this->file->open(QIODevice::WriteOnly| QIODevice::Text)){
        emit this->signal_PushErrorReport(this, "保存过程中文件打开失败");
    }
    QTextStream out(this->file);
    out.setCodec(this->codec);

    for(auto itor = this->contents.constBegin();
        itor != this->contents.constEnd();
        itor++){
        auto line = *itor;
        out << line << endl;
    }
    this->file->close();
}

I_TextModel *DefaultTextModel::openNewTextModel(Core::WsCore *core, PlgDef::I_PluginBase *upStream, QHash<QString, QString> xargs)
{
    auto rtn = new DefaultTextModel;
    rtn->core = core;
    auto ups = dynamic_cast<PlgDef::ChannelPreface::I_ChannelPreface*>(upStream);
    QString filePath = ups->getFilePath();

    QFile *file = new QFile(filePath);
    rtn->file = file;
    if(!file->open(QIODevice::Text|QIODevice::ReadOnly)){
        emit this->signal_PushErrorReport(this, "打开文件失败路径："+filePath);
        return nullptr;
    }

    QTextCodec *codec = QTextCodec::codecForLocale();

    auto cEncode = xargs.find(ENCODING);
    if(cEncode != xargs.constEnd()){
        auto bytec = cEncode.value().toUtf8();
        codec = QTextCodec::codecForName(bytec);
    }

    QTextStream in(file);
    in.setCodec(codec);
    rtn->codec = codec;

    while (!in.atEnd()) {
        QString line = in.readLine();
        rtn->contents.append(line);
    }
    file->close();

    return rtn;
}

int DefaultTextModel::getRowsCount()
{
    return this->contents.size();
}

QString DefaultTextModel::getLineContent(int row)
{
    return this->contents.at(row);
}

void DefaultTextModel::updateLineContent(int row, const QString content)
{
    this->contents.takeAt(row);
    this->contents.insert(row, content);
}

void DefaultTextModel::insertLineAt(int row, const QString content)
{
    this->contents.insert(row, content);
}

void DefaultTextModel::removeLines(int begin, int offset)
{
    if(offset == -1){
        offset = this->contents.size() - begin;
    }

    for(int i=0; i < offset; ++i){
        this->contents.removeAt(begin);
    }
}

void DefaultTextModel::reLoadAllText(QString text)
{
    QStringList list = text.split("\n");
    this->removeLines(0);
    for(auto itor=list.constBegin();
        itor != list.constEnd();
        ++itor){
        auto x = *itor;
        this->insertLineAt(this->getRowsCount(), x);
    }
}
























