#include "registerbrowser.h"

RegisterBrowser::RegisterBrowser(QWidget *parent) : QTreeWidget(parent)
{
    this->setColumnCount(2);
    QStringList top;
    top << "Register" << "Value";
    this->setHeaderLabels(top);

    this->m_nodeMap.clear();
}

void RegisterBrowser::onUpdateRegisters(const RegisterGroup &registers)
{
    if (this->m_nodeMap.empty()){
        for (int groupIndex = 0; groupIndex < registers.groupName.size(); groupIndex++){
            QTreeWidgetItem *groupNode = new QTreeWidgetItem();
            this->addTopLevelItem(groupNode);
            groupNode->setText(0, registers.groupName[groupIndex]);
            for (int regIndex = 0; regIndex < registers.registerInfo[groupIndex].registerNames.size(); regIndex++){
                QTreeWidgetItem *registerNode = new QTreeWidgetItem(groupNode);
                registerNode->setText(0, registers.registerInfo[groupIndex].registerNames[regIndex]);
                registerNode->setText(1, registers.registerInfo[groupIndex].values[regIndex]);
                this->m_nodeMap[registers.registerInfo[groupIndex].registerNames[regIndex]] = registerNode;
            }
        }
    }
    else{
        for (int groupIndex = 0; groupIndex < registers.groupName.size(); groupIndex++){
            for (int regIndex = 0; regIndex < registers.registerInfo[groupIndex].registerNames.size(); regIndex++){
                QTreeWidgetItem *registerNode = this->m_nodeMap[registers.registerInfo[groupIndex].registerNames[regIndex]];
                registerNode->setText(0, registers.registerInfo[groupIndex].registerNames[regIndex]);
                registerNode->setText(1, registers.registerInfo[groupIndex].values[regIndex]);
            }
        }
    }
}
