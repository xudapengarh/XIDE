#ifndef TREEMENU_H
#define TREEMENU_H

#include <QObject>
#include <QWidget>
#include <QTreeWidget>
#include <QDialog>
#include <QTreeWidgetItem>
#include <QComboBox>
#include <QDir>

/**
 * @brief The NewBulidFile class 创建新文件类
 */
class NewBulidFile : public QDialog{
    Q_OBJECT
public:
    NewBulidFile(QWidget *parent=0);
    QLineEdit *fileNameEdit;
    QComboBox *fileNameTypeBox;
    QPushButton *okButton;
    QPushButton *cancalButton;
};

/**
 * @brief The NewBuildfolder class 创建新文件夹类
 */
class NewBuildfolder : public QDialog
{
    Q_OBJECT
public:
    NewBuildfolder(QWidget *parent=0);
    QLineEdit *fileNameEdit;
    QPushButton *okButton;
    QPushButton *cancalButton;
};

class TreeMenu : public QTreeWidget
{
    Q_OBJECT
public:
    TreeMenu(QWidget *parent = 0);
    void CreateTopItem(QString path);
    void FindFile(QDir *path,QTreeWidgetItem *parent);
    void buttonCollapseItem();
private:
    QTreeWidgetItem *root; //树的根
    QMenu *dirMenu;  //文件夹菜单栏
    QMenu *fileMenu;  //文件菜单栏
    QTreeWidgetItem *nowItem; //当前光标所在Item
    int nowCol; //当前所在列
    NewBulidFile *newFileWidget;
    NewBuildfolder *newDirWidget;
signals:
    void showTextSignal(QString path,QString name,QTreeWidgetItem *item);
    void collapseItemSignal(const QTreeWidgetItem *item);
    void renameSignal();
private slots:
    void DoubleClickpath(QTreeWidgetItem *item, int column);
    void itemPressedSlot(QTreeWidgetItem * pressedItem, int column);
    void tempActionInformation(QAction *act);
    void bulidNewFileSlot(bool flag);
    void closeBuildFileSlot(bool flag);
    void buildNewDirSlot(bool flag);
    void cancelDirSlot(bool flag);
};

#endif // TREEMENU_H
