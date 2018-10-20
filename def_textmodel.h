#ifndef DEF_TEXTMODEL_H
#define DEF_TEXTMODEL_H
#include "def_plugin_base.h"
#include "wscore.h"

namespace PlgDef {
    namespace TextModel {
        class I_TextModel:public I_Configurable,public I_Combiantion
        {
            Q_OBJECT
        public:
            I_TextModel();
            virtual ~I_TextModel() override;

            /**
             * @brief 获取一个新的TextModel
             * @param core 调度核心，提供交互接口
             * @param upStream 上游模块，提供内容
             * @param xargs 参数列表，如果提供的话
             * @return 新实例
             */
            virtual I_TextModel* openNewTextModel(Core::WsCore *core, PlgDef::I_PluginBase *upStream, QHash<QString, QString> xargs) = 0;

            /**
             * @brief 重新载入所有Text内容，该内容是无格式的纯文本。此操作后，本组件内部的所有内容完全刷新，之前状态完全作废
             * @param text 目标文字
             */
            virtual void reLoadAllText(QString text)=0;

            /**
             * @brief 获取内容行总数
             * @return 数量
             */
            virtual int getRowsCount()=0;

            /**
             * @brief 获取特定行内容
             * @param row 行序：自0起
             * @return 内容
             */
            virtual QString getLineContent(int row)=0;

            /**
             * @brief 更新特定行内容
             * @param row 行序：自0起
             * @param content 内容
             */
            virtual void updateLineContent(int row, const QString content)=0;

            /**
             * @brief 插入内容到指定行序，原该行序及其后内容向后偏移一行
             * @param row 行序
             * @param content 内容
             */
            virtual void insertLineAt(int row, const QString content)=0;

            /**
             * @brief 删除指定行内容，默认offse=-1，代表自begin起（包括begin）之后的内容行完全删除；
             * removeLines(7,2)：表示删除7、8、9三行
             * @param begin 开始行序
             * @param offset 延伸量
             */
            virtual void removeLines(int begin, int offset = -1)=0;

            // I_PluginBase interface
        public:
            virtual PluginType pluginMark() override final;
        };
    }
}

#endif // DEF_TEXTMODEL_H
