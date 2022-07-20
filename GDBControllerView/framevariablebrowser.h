#ifndef FRAMEVARIABLEBROWSER_H
#define FRAMEVARIABLEBROWSER_H

#include <QObject>
#include <QWidget>
#include <QTreeWidget>
#include "debuger.h"

class FrameVariableBrowser : public QTreeWidget
{
    Q_OBJECT
public:
    FrameVariableBrowser(QWidget *parent = nullptr);

public:
    void onUpdateFrameVariable(const FrameVariable &variables);

private:
    QMap<QString, QTreeWidgetItem*> m_nodeMap;

};

#endif // FRAMEVARIABLEBROWSER_H
