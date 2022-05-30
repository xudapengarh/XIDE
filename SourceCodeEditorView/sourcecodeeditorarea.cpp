#include <QKeyEvent>
#include <QDebug>
#include <QPainter>
#include <QTextBlock>

#include "sourcecodeeditorarea.h"

int flag = 0;



SourceCodeEditorArea_bac::SourceCodeEditorArea_bac(QWidget *parent) : QPlainTextEdit(parent)
{
    // 设置字体
    QFont font;
    font.setFamily("Courier");
    font.setStyleHint(QFont::Monospace);
    font.setFixedPitch(true);
    font.setPointSize(14);
    this->setFont(font);

    // 设置缩进
    QFontMetrics metrics(font);
    this->setTabStopWidth(4 * metrics.width(" "));

    // 创建行号控件
    this->m_lineArea = new LineNumberArea(this);

    // 关联数据内容行数变化，重新计算行号控件宽度
    connect(this, &SourceCodeEditorArea_bac::blockCountChanged, this, &SourceCodeEditorArea_bac::updateLineNumberAreaWidth);

    // 关联更新信号
    connect(this, &SourceCodeEditorArea_bac::updateRequest, this, &SourceCodeEditorArea_bac::updateLineNumberArea);
    //    connect(this, &SourceCodeEditorArea::cursorPositionChanged, this, &SourceCodeEditorArea::highlightCurrentLine);

    // 初始化行号控件
    updateLineNumberAreaWidth(0);
}

void SourceCodeEditorArea_bac::OpenFile(QFileInfo fileInfo)
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

    //    int lineCount = this->document()->lineCount();
    //    for(int  i = 0; i < lineCount; i++){
    //        this->m_lineArea->AddLine();
    //    }
}

void SourceCodeEditorArea_bac::SaveFile()
{
    QFile file;
    file.setFileName(this->m_currentFileInfo.absoluteFilePath());
    if(file.open(QIODevice::WriteOnly))
    {
        file.write(this->toPlainText().toUtf8());
    }
    file.close();
}

void SourceCodeEditorArea_bac::lineNumberAreaPaintEvent(QPaintEvent *event)
{

    qDebug() << __FUNCTION__ << " : " << flag++;
    // 行号控件绘制
    QPainter painter(this->m_lineArea);

    // 获取第一个可视的 block (即可见范围内的第一行)
    QTextBlock block = firstVisibleBlock();

    // 返回该 block 所在的行号（从 0 开始）
    int blockNumber = block.blockNumber();


    int blockHeight = (int) blockBoundingRect(block).height();      // block 的高度

    // 根据该 block 的大小计算显示行号的位置
    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + blockHeight;

    // 计算可视范围内的 block 数量（行数）
    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            // 绘制行号
            QString number = QString::number(blockNumber + 1);
            painter.setPen(Qt::black);
            painter.drawText(0, top, this->m_lineArea->width(), fontMetrics().height(),
                             Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + blockHeight;
        ++blockNumber;
    }
}

int SourceCodeEditorArea_bac::lineNumberAreaWidth()
{
    qDebug() << __FUNCTION__ << " : " << flag++;
    // 根据行数的位数计算行号显示区域的宽度
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }

    int space = 3 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;
    return space;
}

void SourceCodeEditorArea_bac::resizeEvent(QResizeEvent *event)
{
    qDebug() << __FUNCTION__ << " : " << flag++;
    QPlainTextEdit::resizeEvent(event);

    QRect cr = contentsRect();

    // 放置行号显示区域的位置
    this->m_lineArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

void SourceCodeEditorArea_bac::updateLineNumberAreaWidth(int newBlockCount)
{
    qDebug() << __FUNCTION__ << " : " << flag++;
    // 设置代码区域的左边距
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void SourceCodeEditorArea_bac::updateLineNumberArea(const QRect &rect, int dy)
{
    qDebug() << __FUNCTION__ << " : " << flag++;
    // 根据代码滚动位置，滚动行号
    if (dy)
        this->m_lineArea->scroll(0, dy);
    else
        this->m_lineArea->update(0, rect.y(), this->m_lineArea->width(), rect.height());

    if (rect.contains(viewport()->rect())){
        updateLineNumberAreaWidth(0);
    }
}

