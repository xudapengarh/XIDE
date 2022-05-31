#include <QFont>


#include "sourcecodelinearea.h"

SourceCodeLineArea::SourceCodeLineArea(QWidget *parent) : QWidget(parent)
{
    this->setMaximumWidth(10);

    // 初始化变量

    this->m_topMargin = 0;
    this->m_itemHeight = 0;             // item 高度
    this->m_itemWidth = 0;              // item 宽度

    this->setMaximumWidth(0);
    this->setMinimumWidth(0);
    this->m_inited = false;
}

void SourceCodeLineArea::onLineAreaUpdate(const EditorAreaAttribute &att)
{
    this->m_att = att;
    this->m_inited = true;

    bool updateFlag = false;
    if (this->m_itemHeight != att.blockHeight){
        this->m_itemHeight = att.blockHeight;
        updateFlag = true;
    }

    int letterWidth = fontMetrics().horizontalAdvance(QLatin1Char('9'));
    int digits = 1;

    int totalCount = att.totalBlockCount;
    while (totalCount >= 10) {
        totalCount /= 10;
        ++digits;
    }


    if (digits * letterWidth + letterWidth != this->m_itemWidth){
        this->m_itemWidth = digits * letterWidth + letterWidth;
        updateFlag = true;
    }

    if (att.topMargin != this->m_topMargin){
        this->m_topMargin = att.topMargin;
        updateFlag = true;
    }

    if (updateFlag){
        this->UpdateArea();
    }

    for (int i = 0, lineNumber = att.validBlockNumberSection.begain; i < this->m_itemList.count(); i++, lineNumber++){
        if (att.validBlockNumberSection.Contains(lineNumber)){
            this->m_itemList.at(i)->setText(QString::number(lineNumber));
        }
        else{
            this->m_itemList.at(i)->setText("");
        }
    }
}

void SourceCodeLineArea::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    this->UpdateArea();
    if (this->m_inited){
        this->onLineAreaUpdate(this->m_att);
    }
}

void SourceCodeLineArea::UpdateArea()
{
    if (this->m_itemHeight > 0 && this->m_itemHeight > 0){
        this->setMaximumWidth(this->m_itemWidth);
        this->setMinimumWidth(this->m_itemWidth);
        int bottom = this->m_topMargin + 1;
        for (int i = this->m_itemList.count() - 1; i >= 0; i--){
            LineItem *item = this->m_itemList[i];
            item->setVisible(false);
            this->m_itemList.removeAt(i);
            delete item;
        }
        while(bottom < this->height()){
            LineItem *item = new LineItem(this);
            item->setAlignment(Qt::AlignRight);
            item->setGeometry(0, bottom, this->m_itemWidth -  fontMetrics().horizontalAdvance(QLatin1Char('9')), this->m_itemHeight);
            item->show();
            bottom += this->m_itemHeight;
            this->m_itemList.append(item);
        }
    }
}


