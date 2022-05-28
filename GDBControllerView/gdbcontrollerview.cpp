#include <QHBoxLayout>

#include "gdbcontrollerview.h"


GDBControllerView::GDBControllerView(QWidget *parent) : QWidget(parent)
{
    this->m_run = new GDBControllerButton(this);
    this->m_startDebug = new GDBControllerButton(this);
    this->m_nexti = new GDBControllerButton(this);

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(this->m_run);
    layout->addWidget(this->m_startDebug);
    layout->addWidget(this->m_nexti);
    layout->addStretch();
    layout->setContentsMargins(10, 0, 10, 0);
    this->setMaximumHeight(40);
    this->setMinimumHeight(40);
}
