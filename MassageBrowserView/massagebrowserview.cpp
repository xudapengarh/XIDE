#include "massagebrowserview.h"

MassageBrowserView::MassageBrowserView(QWidget *parent) : QTextBrowser(parent)
{

}

void MassageBrowserView::onShowMassage(QString message)
{
    this->setText(message);
}
