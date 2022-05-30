#include <QFont>


#include "sourcecodelinearea.h"

SourceCodeLineArea::SourceCodeLineArea(QWidget *parent) : QWidget(parent)
{
    this->m_fount.setFamily("Courier");
    this->m_fount.setStyleHint(QFont::Monospace);
    this->m_fount.setFixedPitch(true);
    this->m_fount.setPointSize(14);
    this->setFont(this->m_fount);

    this->m_lineCount = 0;

    this->m_lineLabels.clear();


    QFontMetrics metrics(this->m_fount);
    this->m_labelWidth = 3 * metrics.width("0") + 20;
    this->m_labelHeight = metrics.height();

    this->m_vlayout = new QVBoxLayout(this);

    this->m_vlayout->setSpacing(6);
}


void SourceCodeLineArea::AddLine()
{
    LineLabel *lineLabel = new LineLabel(this);
    lineLabel->setText(QString::number(this->m_lineLabels.count()));
    this->m_vlayout->addWidget(lineLabel);

    this->m_lineLabels.append(lineLabel);

    if(this->m_lineLabels.count() > 100){
        QFontMetrics metrics(this->m_fount);
        this->m_labelWidth = metrics.width(QString::number(this->m_lineLabels.count())) + 20;
        this->setMaximumWidth(this->m_labelWidth);
        lineLabel->setMaximumSize(this->m_labelWidth, this->m_labelHeight);
    }

}

void SourceCodeLineArea::RemoveLine()
{
    LineLabel *lineLabel = this->m_lineLabels.at(this->m_lineLabels.count() - 1);
    this->m_vlayout->removeWidget(lineLabel);
    this->m_lineLabels.removeAt(this->m_lineLabels.count() - 1);
    delete lineLabel;

    if(this->m_lineLabels.count() > 100){
        QFontMetrics metrics(this->m_fount);
        this->m_labelWidth = metrics.width(QString::number(this->m_lineLabels.count())) + 20;
        this->setMaximumWidth(this->m_labelWidth);
        lineLabel->setMaximumSize(this->m_labelWidth, this->m_labelWidth);
    }
}

LineLabel::LineLabel(QWidget *parent) : QLabel(parent)
{
    this->setAlignment(Qt::AlignLeft);
}

void LineLabel::SetSize(int width, int height)
{
    this->setMaximumSize(width, height);
}
