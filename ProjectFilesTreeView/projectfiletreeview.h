#ifndef PROJECTFILETREEVIEW_H
#define PROJECTFILETREEVIEW_H

#include <QObject>
#include <QWidget>
#include <QTreeWidget>
#include <QDialog>
#include <QTreeWidgetItem>
#include <QComboBox>
#include <QDir>
#include <QLabel>
#include <QLineEdit>
#include <QMenu>


/**
 * @brief The ProjectOperation class 项目树操作栏
 */
class ProjectOperation : public QWidget{
    Q_OBJECT
public:
    ProjectOperation(QWidget *parent);

protected:
    void paintEvent(QPaintEvent *event) override;

public :
    void SetLabel(QString label);

signals:
    void addDir();      // 添加目录
    void addFile();     // 添加文件
    void deleteItem();  // 删除 item
    void openDir();     // 打开项目

private:
    QLabel *m_label;
    QPushButton *m_addDir;
    QPushButton *m_addFile;
    QPushButton *m_deleteItem;
    QPushButton *m_openDir;
};

/**
 * @brief The ProjectFileTreeView class 项目文件树视图类
 */
class ProjectFileTreeView : public QTreeWidget
{
    Q_OBJECT
public:
    ProjectFileTreeView(QWidget *parent);
    void Init(const QString &path);
    void CreateProject();
    void OpenProject();


protected:
    // 重载双击事件，屏蔽双击重命名
    void mouseDoubleClickEvent ( QMouseEvent * event) override;


signals:
    void openFile(QFileInfo fileInfo);
    void closeFile(QFileInfo fileInfo);

private:
    void FindFile(QDir *path, QTreeWidgetItem *parent);
    QIcon GetIcon(QFileInfo fileInfo);


private slots:

    void openDir();         // 打开目录
    void addDir();          // 添加目录
    void addFile();         // 添加文件
    void deleteItem();      // 删除 item
    void renameItem();      // 重命名 item
    void searchInDir();     // 在目录中查找
    void searchInFile();    // 在文件中查找
    void openItemDir();     // 打开 item 所在的文件夹
    void openFile();        // 打开文件
    void copyAbsolutePath();// 复制绝对路径

    void onDirMenuTriggered(QAction *action);   // 目录菜单点击响应事件
    void onFileMenuTriggered(QAction *action);  // 文件菜单点击响应事件

    void onItemPressed(QTreeWidgetItem *item, int column);      // item 单击及右键事件
    void onItemDoubleClicked(QTreeWidgetItem* item, int col);   // item 双击事件

private:
    QTreeWidgetItem *m_root;        // 树的根
    QTreeWidgetItem *m_currentItem; // 当前光标选中的 item

    QTreeWidgetItem *m_operationItem;        // 项目操作控件所在的 item
    ProjectOperation *m_operation;           // 项目操作控件

    int m_currentCol;                        // 当前光标所在的列

    QMenu *m_dirMenu;     // 目录操作菜单
    QMenu *m_fileMenu;   // 文件操作菜单
};



#endif // PROJECTFILETREEVIEW_H
