#include <QKeyEvent>
#include <QDebug>
#include <QPainter>
#include <QTextBlock>

#include "sourcecodeeditorarea.h"

int flag = 0;



SourceCodeEditorArea::SourceCodeEditorArea(QWidget *parent) : QPlainTextEdit(parent)
{

    // 设置缩进
    QFontMetrics metrics(this->font());
    this->setTabStopWidth(4 * metrics.width(" "));

    connect(this, &SourceCodeEditorArea::updateRequest, this, &SourceCodeEditorArea::onUpdateRequest);


}

void SourceCodeEditorArea::OpenFile(QFileInfo fileInfo)
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
        //        //一行一行的读
        //        QByteArray array;
        //        while(file.atEnd() == false)
        //        {
        //            array += file.readLine();
        //        }
        //        this->setPlainText(QString(array).toUtf8().data());

    }
    file.close();
    emit updateRequest(this->rect(), 0);

    //    int lineCount = this->document()->lineCount();
    //    for(int  i = 0; i < lineCount; i++){
    //        this->m_lineArea->AddLine();
    //    }
}

void SourceCodeEditorArea::SaveFile()
{
    QFile file;
    file.setFileName(this->m_currentFileInfo.absoluteFilePath());
    if(file.open(QIODevice::WriteOnly))
    {
        file.write(this->toPlainText().toUtf8());
    }
    file.close();
}

void SourceCodeEditorArea::resizeEvent(QResizeEvent *event)
{
    QPlainTextEdit::resizeEvent(event);
    emit updateRequest(this->rect(), 0);
}

void SourceCodeEditorArea::onUpdateRequest(const QRect &rect, int dy)
{
    EditorAreaAttribute att;
    // 获取所有的 block 数量（用于计算行号 widget 的宽度）
    att.totalBlockCount = this->blockCount();
    att.totalBlockCount = att.totalBlockCount > 0 ? att.totalBlockCount : 1;
    QTextBlock block = firstVisibleBlock();

    // 获取上边距
    att.topMargin = (int) blockBoundingGeometry(block).translated(contentOffset()).top();

    // 获取 block 的高度
    att.blockHeight = (int) blockBoundingRect(block).height();
    if (this->blockCount() == 1){
        att.blockHeight -= att.topMargin;
    }

    // 获取第一个可见 block 的 number
    att.validBlockNumberSection.begain = block.blockNumber() + 1;


    // 计算可见范围内的 block 数量
    int validBlockCount = 0;


    while(block.isValid() && validBlockCount * att.blockHeight + att.topMargin < this->height()){
        block = block.next();
        validBlockCount ++;
    }
    att.validBlockNumberSection.end = att.validBlockNumberSection.begain + validBlockCount - 1;

    emit lineAreaUpdate(att);
}



