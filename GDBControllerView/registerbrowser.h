#ifndef REGISTERBROWSER_H
#define REGISTERBROWSER_H

#include <QObject>
#include <QWidget>
#include <QTableWidget>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QTreeWidget>
#include <QMap>

#include "debuger.h"



class RegisterBrowser : public QTreeWidget
{
    Q_OBJECT
public:
    RegisterBrowser(QWidget *parent = nullptr);

public:
    void onUpdateRegisters(const RegisterGroup &registers);

private:
    QMap<QString, QTreeWidgetItem*> m_nodeMap;
};

#endif // REGISTERBROWSER_H
