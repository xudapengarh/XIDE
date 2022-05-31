#include <QKeyEvent>
#include <QDebug>
#include <QPainter>
#include <QTextBlock>

#include "codeeditorarea.h"

CodeEditorArea::CodeEditorArea(QWidget *parent) : QPlainTextEdit(parent)
{
    // 设置缩进
    QFontMetrics metrics(this->font());
    this->setTabStopWidth(4 * metrics.width(" "));

    // 设置不自动换行
    this->setLineWrapMode(QPlainTextEdit::NoWrap);

    // 绑定更新事件
    connect(this, &CodeEditorArea::updateRequest, this, &CodeEditorArea::onUpdateRequest);
}

void CodeEditorArea::OpenFile(QFileInfo fileInfo)
{
    this->m_currentFileInfo = fileInfo;

    QFile file;
    file.setFileName(fileInfo.absoluteFilePath());
    if(file.open(QIODevice::ReadOnly))
    {
        //读文件
        QByteArray array =  file.readAll();
        //将数据写进文本框中
        this->setPlainText(QString(array));
    }
    file.close();

    // 更新界面
    emit updateRequest(this->rect(), 0);
}

void CodeEditorArea::SaveFile()
{
    QFile file;
    file.setFileName(this->m_currentFileInfo.absoluteFilePath());
    if(file.open(QIODevice::WriteOnly))
    {
        file.write(this->toPlainText().toUtf8());
    }
    file.close();
}

void CodeEditorArea::resizeEvent(QResizeEvent *event)
{
    QPlainTextEdit::resizeEvent(event);

    // 更新
    emit updateRequest(this->rect(), 0);
}

void CodeEditorArea::keyPressEvent(QKeyEvent *event)
{
    if( (event ->modifiers()& Qt::ControlModifier) != 0 && event ->key() == Qt::Key_S ){
        this->SaveFile();
        return;
    }
    QPlainTextEdit::keyPressEvent( event );
}

void CodeEditorArea::onUpdateRequest(const QRect &rect, int dy)
{
    EditorAreaAttribute att;
    // 获取所有的 block 数量（用于计算行号 widget 的宽度）
    att.totalBlockCount = this->blockCount();
    att.totalBlockCount = att.totalBlockCount > 0 ? att.totalBlockCount : 1;
    QTextBlock block = firstVisibleBlock();

    // 获取上边距
    att.topMargin = (int) blockBoundingGeometry(block).translated(contentOffset()).top();

    // 获取第一个可见范围内的 bolck 的 number
    att.validBlockNumberSection.begain = block.blockNumber() + 1;
    att.validBlockNumberSection.end = block.blockNumber();

    // 判断 block 位置是否超出可见范围内的底部
    int validBlockBottom = att.topMargin;

    while(block.isValid() && validBlockBottom < this->height()){
        // 获取该 block 的高度
        int blockHeight = (int) blockBoundingGeometry(block).translated(contentOffset()).height();

        validBlockBottom += blockHeight;
        att.blockHeight.append(blockHeight);

        block = block.next();

        att.validBlockNumberSection.end++;
    }
    emit lineAreaUpdate(att);
}



