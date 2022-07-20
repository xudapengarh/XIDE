#include "framevariablebrowser.h"

FrameVariableBrowser::FrameVariableBrowser(QWidget *parent) : QTreeWidget(parent)
{
    this->setColumnCount(3);
    QStringList top;
    top << "Type" << "Variable" << "Value";
    this->setHeaderLabels(top);
}

void FrameVariableBrowser::onUpdateFrameVariable(const FrameVariable &variables)
{
    while (this->topLevelItemCount() > 0){
        QTreeWidgetItem *node = this->topLevelItem(0);
        this->removeItemWidget(node, 0);
        delete node;
    }
    for (int i = 0; i < variables.variableName.size(); i++){
        QTreeWidgetItem *node = new QTreeWidgetItem();
        node->setText(0, variables.variableType[i]);
        node->setText(1, variables.variableName[i]);
        node->setText(2, variables.variableValue[i]);
        this->addTopLevelItem(node);
    }

}
