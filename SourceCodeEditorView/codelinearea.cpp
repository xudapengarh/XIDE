#include <QFont>


#include "codelinearea.h"

CodeLineArea::CodeLineArea(QWidget *parent) : QWidget(parent)
{
    this->setMaximumWidth(10);

    // 初始化变量
    this->m_letterWidth = 0;            // 字符宽度

    this->setMaximumWidth(0);
    this->setMinimumWidth(0);
}

void CodeLineArea::onLineAreaUpdate(const EditorAreaAttribute &att)
{
    // 获取更新范围内的 block 的信息
    this->m_att = att;

    // 计算字符宽度
    this->m_letterWidth = fontMetrics().horizontalAdvance(QLatin1Char('9'));

    // 通过代码文件的总行数的位数，计算代码行号区域的宽度
    int areaWidth = 3 * this->m_letterWidth;
    int totalCount = att.totalBlockCount;
    while (totalCount >= 10) {
        totalCount /= 10;
        areaWidth += this->m_letterWidth;
    }
    if(this->width() != areaWidth){
        this->setMaximumWidth(areaWidth);
        this->setMinimumWidth(areaWidth);
    }

    // 更新代码行号区域
    this->UpdateArea();

    // 写入行号信息
    for (int i = 0, lineNumber = att.validBlockNumberSection.begain; i < this->m_itemList.count(); i++, lineNumber++){
        if (att.validBlockNumberSection.Contains(lineNumber)){
            this->m_itemList.at(i)->setText(QString::number(lineNumber));
        }
        else{
            this->m_itemList.at(i)->setText("");
        }
    }
}

void CodeLineArea::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

    this->UpdateArea();
    if (this->width() > 0){
        this->onLineAreaUpdate(this->m_att);
    }
}


void CodeLineArea::UpdateArea()
{
    // 更细行号信息
    if (this->width() > 0){
        int bottom = this->m_att.topMargin;

        // 清空原有的行号信息
        for (int i = this->m_itemList.count() - 1; i >= 0; i--){
            LineItem *item = this->m_itemList[i];
            item->setVisible(false);
            this->m_itemList.removeAt(i);
            delete item;
        }

        // 重新生成行号信息的 item
        for (int i = 0; i < this->m_att.blockHeight.count(); i++){
            LineItem *item = new LineItem(this);
            item->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
            item->setGeometry(this->m_letterWidth, bottom, this->width() - 2 * this->m_letterWidth, this->m_att.blockHeight[i]);
            item->show();
            this->m_itemList.append(item);
            bottom += this->m_att.blockHeight[i];
        }
    }
}


