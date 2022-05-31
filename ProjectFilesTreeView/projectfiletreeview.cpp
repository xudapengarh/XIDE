#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMenu>
#include <QScrollBar>
#include <QHeaderView>
#include <QApplication>
#include <QDebug>
#include <QProcess>
#include <QMessageBox>
#include <QFileDialog>
#include <QContextMenuEvent>
#include <QTreeWidgetItem>
#include <QFile>
#include <QHBoxLayout>
#include <QPushButton>
#include <QPainter>
#include <QDesktopServices>
#include <QClipboard>

#include "projectfiletreeview.h"
#include "createnewdialog.h"


ProjectFileTreeView::ProjectFileTreeView(QWidget *parent) : QTreeWidget(parent)
{
    // 设置树的列数量为 1
    this->setColumnCount(1);

    // 自动显示滚动条
    this->setAutoScroll(true);

    // 设置横向滚动条模式
    this->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    this->setRootIsDecorated(false);


    // 隐藏表头
    this->header()->setVisible(false);

    // 展开折叠动画
    this->setAnimated(true);

    // item 点击事件
    connect(this, &QTreeWidget::itemPressed, this, &ProjectFileTreeView::onItemPressed);

    // item 双击事件
    connect(this, &QTreeWidget::itemDoubleClicked, this, &ProjectFileTreeView::onItemDoubleClicked);

    // 操作控件
    this->m_operationItem = new QTreeWidgetItem();
    this->addTopLevelItem(this->m_operationItem);
    this->m_operationItem->setFlags(this->m_operationItem->flags() & ~Qt::ItemIsEnabled & ~Qt::ItemIsSelectable);

    this->m_operation = new ProjectOperation(this);
    this->setItemWidget(this->m_operationItem, 0, this->m_operation);
    connect(this->m_operation, &ProjectOperation::openDir, this, &ProjectFileTreeView::openDir);
    connect(this->m_operation, &ProjectOperation::addDir, this, &ProjectFileTreeView::addDir);
    connect(this->m_operation, &ProjectOperation::addFile, this, &ProjectFileTreeView::addFile);
    connect(this->m_operation, &ProjectOperation::deleteItem, this, &ProjectFileTreeView::deleteItem);

    this->m_dirMenu = new QMenu(this);
    this->m_dirMenu->addAction("新建文件");
    this->m_dirMenu->addAction("新建文件夹");
    this->m_dirMenu->addAction("打开所在文件夹");
    this->m_dirMenu->addSeparator();
    this->m_dirMenu->addAction("重命名");
    this->m_dirMenu->addAction("在文件夹中查找");
    this->m_dirMenu->addSeparator();
    this->m_dirMenu->addAction("删除文件夹");
    connect(this->m_dirMenu, &QMenu::triggered, this, &ProjectFileTreeView::onDirMenuTriggered);

    this->m_fileMenu = new QMenu(this);
    this->m_fileMenu->addAction("打开");
    this->m_fileMenu->addAction("打开所在文件夹");
    this->m_fileMenu->addSeparator();
    this->m_fileMenu->addAction("复制绝对路径");
    this->m_fileMenu->addSeparator();
    this->m_fileMenu->addAction("重命名");
    this->m_fileMenu->addAction("删除");
    connect(this->m_fileMenu, &QMenu::triggered, this, &ProjectFileTreeView::onFileMenuTriggered);
}

void ProjectFileTreeView::Init(const QString &path)
{
    // 根据传入文件夹路径创建根节点
    QDir *rootDir = new QDir(path);
    this->m_root = new QTreeWidgetItem();
    this->addTopLevelItem(this->m_root);
    //    this->m_root->setIcon(0, QApplication::style()->standardIcon((QStyle::StandardPixmap)29));
    this->m_root->setIcon(0, QIcon(QPixmap(":/icons/icons/icon_project.svg")));
    this->m_root->setText(0, rootDir->dirName());
    this->m_root->setToolTip(0, path);

    // 设置操作栏标签
    this->m_operation->SetLabel(rootDir->dirName());

    // 根据传入路径遍历路径下文件夹及文件
    FindFile(rootDir, this->m_root);

    // 防止程序假死
    QApplication::processEvents();

    // 展开根 item
    this->m_root->setExpanded(true);
    this->m_root->setFlags(this->m_root->flags() | Qt::ItemIsEditable);
    this->m_currentItem = this->m_root;
}

void ProjectFileTreeView::CreateProject()
{
    // 创建项目
    CreateNewDialog dialog;
    dialog.CreateDir();
    QString path = dialog.GetAbsolutePath();
    if(!path.isEmpty()){
        Init(path);;
    }
}

void ProjectFileTreeView::OpenProject()
{
    // 打开项目文件夹
    QString path = QFileDialog::getExistingDirectory(this, "选择目录", "./", QFileDialog::ShowDirsOnly);
    if(!path.isEmpty()){
        Init(path);;
    }
}

void ProjectFileTreeView::mouseDoubleClickEvent(QMouseEvent *event)
{
    QString absolutePath = this->currentItem()->toolTip(0);
    QFileInfo fileInfo(absolutePath);
    if (fileInfo.isFile()){
        // 打开文件
        emit openFile(fileInfo);
    }
}



void ProjectFileTreeView::FindFile(QDir *path, QTreeWidgetItem *parent)
{
    QApplication::processEvents();
    // 返回该文件夹下所有文件夹/文件名
    QStringList allDirName = path->entryList();

    // 获取绝对路径
    QString absolutePath = path->absolutePath();

    // 遍历文件夹
    for (int i = 2; i < allDirName.size(); i++){
        QFileInfo fileInfo = QFileInfo(absolutePath + "/" + allDirName[i]);
        if (fileInfo.isFile()){
            // 添加文件至 item
            QTreeWidgetItem *item = new QTreeWidgetItem(parent);
            item->setFlags(item->flags() | Qt::ItemIsEditable);
            item->setIcon(0, GetIcon(fileInfo));

            item->setText(0, allDirName[i]);
            item->setToolTip(0, absolutePath + "/" +allDirName[i]);
        }
        else if(fileInfo.isDir()){
            // 添加文件夹至 item
            QDir *nextDir = new QDir(absolutePath + "/" + allDirName[i]);
            QTreeWidgetItem *item = new QTreeWidgetItem(parent);
            item->setFlags(item->flags() | Qt::ItemIsEditable);
            item->setIcon(0, GetIcon(fileInfo));
            item->setText(0, allDirName[i]);
            item->setToolTip(0, absolutePath + "/" +allDirName[i]);

            // 遍历子文件夹
            FindFile(nextDir, item);
        }
    }
}


void ProjectFileTreeView::onItemPressed(QTreeWidgetItem *item, int column)
{
    this->resizeColumnToContents(0);
    this->m_currentItem = item;
    this->m_currentCol = column;
    QTreeWidgetItem *root = this->m_currentItem;
    while (root->parent() != nullptr){
        root = root->parent();
    }
    this->m_root = root;
    this->m_operation->SetLabel(this->m_root->text(0));
    // 右键事件
    if (qApp->mouseButtons() == Qt::RightButton){

        QString absolutePath = item->toolTip(0);
        QFileInfo fileInfo(absolutePath);
        if (fileInfo.isFile()){

            this->m_fileMenu->exec(QCursor().pos());

        }
        else if (fileInfo.isDir()){
            this->m_dirMenu->exec(QCursor().pos());
        }
    }
    else{
        QString absolutePath = item->toolTip(0);
        QFileInfo fileInfo(absolutePath);
        if (fileInfo.isFile()){
            // 打开文件
            emit openFile(fileInfo);
        }
    }
}

void ProjectFileTreeView::onItemDoubleClicked(QTreeWidgetItem *item, int col)
{
    this->m_currentItem = item;
    this->m_currentCol = col;

    QString absolutePath = item->toolTip(0);
    QFileInfo fileInfo(absolutePath);
    if (fileInfo.isFile()){
        // 打开文件
        emit openFile(fileInfo);
    }
}

void ProjectFileTreeView::addDir()
{
    // 获取父节点
    QTreeWidgetItem *parent = nullptr;

    if (QFileInfo(this->m_currentItem->toolTip(0)).isFile()){
        // 当前节点为文件，则父节点为上级目录
        parent = this->m_currentItem->parent();
    }
    else if (QFileInfo(this->m_currentItem->toolTip(0)).isDir()){
        // 当前节点为目录，则父节点为当前节点
        parent = this->m_currentItem;
    }
    else{
        return;
    }

    // 创建新节点，并添加到父节点中
    QTreeWidgetItem *newItem = new QTreeWidgetItem(parent);

    // 展开父节点
    parent->setExpanded(true);

    // 计算默认目录名称
    QString defaultName = "Untiled";
    QString defaultPath = parent->toolTip(0) + "/";
    int index = 1;
    while(QFileInfo(defaultPath + defaultName).exists()){
        defaultName = "Untiled-" + QString::number(index++) ;
    }

    QDir newDir;
    newDir.mkdir(defaultPath + defaultName);

    // 设置节点属性
    newItem->setText(0, defaultName);
    newItem->setToolTip(0, defaultPath + defaultName);
    newItem->setIcon(0, this->GetIcon(QFileInfo(defaultPath + defaultName)));
    newItem->setFlags(this->m_currentItem->flags() | Qt::ItemIsEditable);

    // 重命名目录
    this->m_currentItem = newItem;
    this->setCurrentItem(newItem);
    this->renameItem();
}

void ProjectFileTreeView::openDir()
{
    // 打开目录
    QString path = QFileDialog::getExistingDirectory(this, "选择目录", "./", QFileDialog::ShowDirsOnly);
    if(!path.isEmpty()){
        Init(path);;
    }
}

void ProjectFileTreeView::addFile()
{
    // 获取父节点
    QTreeWidgetItem *parent = nullptr;

    if (QFileInfo(this->m_currentItem->toolTip(0)).isFile()){
        // 当前节点为文件，则父节点为上级目录
        parent = this->m_currentItem->parent();
    }
    else if (QFileInfo(this->m_currentItem->toolTip(0)).isDir()){
        // 当前节点为目录，则父节点为当前节点
        parent = this->m_currentItem;
    }
    else{
        return;
    }

    // 创建新节点，并添加到父节点中
    QTreeWidgetItem *newItem = new QTreeWidgetItem(parent);

    // 展开父节点
    parent->setExpanded(true);

    // 计算文件默认名称
    QString defaultName = "Untiled";
    QString defaultPath = parent->toolTip(0) + "/";
    int index = 1;
    while(QFileInfo(defaultPath + defaultName).exists()){
        defaultName = "Untiled-" + QString::number(index++) ;
    }

    // 创建文件
    QFile file(defaultPath + defaultName);
    file.open(QIODevice::WriteOnly);
    file.close();

    // 设置节点属性
    newItem->setText(0, defaultName);
    newItem->setToolTip(0, defaultPath + defaultName);
    newItem->setIcon(0, this->GetIcon(QFileInfo(defaultPath + defaultName)));
    newItem->setFlags(this->m_currentItem->flags() | Qt::ItemIsEditable);

    this->m_currentItem = newItem;
    this->setCurrentItem(newItem);
    this->renameItem();
}

void ProjectFileTreeView::deleteItem()
{
    // 删除 item
    QString absolutePath = this->m_currentItem->toolTip(0);
    QFileInfo fileInfo(absolutePath);
    // 删除文件
    if (fileInfo.isFile()){
        if(QMessageBox::Yes == QMessageBox::question(nullptr, "删除文件", absolutePath + "\n\n将被删除，是否确认操作？")){
            emit closeFile(fileInfo);
            QFile::remove(absolutePath);
            QTreeWidgetItem *item = this->m_currentItem;
            this->m_currentItem = this->m_currentItem->parent();
            this->m_currentItem->removeChild(item);
            delete item;
        }
    }
    // 删除目录
    else if (fileInfo.isDir()){
        if(QMessageBox::Yes == QMessageBox::question(nullptr, "删除目录", absolutePath + "\n\n将被删除，是否确认操作？")){

            QDir dir(absolutePath);
            if(this->m_currentItem->childCount() > 0){
                QTreeWidgetItem *itParent = this->m_currentItem;
                QTreeWidgetItem *it = this->m_currentItem->child(0);
                // 遍历该节点内的文件夹/文件
                while(itParent != this->m_currentItem->parent()){
                    if(it->childCount() > 0){
                        itParent = it;
                        it = it->child(0);
                    }
                    else {
                        QFileInfo file(it->toolTip(0));
                        emit closeFile(file);
                        itParent->removeChild(it);
                        delete it;
                        if (itParent->childCount() > 0){
                            it = itParent->child(0);
                        }
                        else{
                            it = itParent;
                            itParent = itParent->parent();
                        }
                    }
                };
            }
            dir.removeRecursively();
            QTreeWidgetItem *item = this->m_currentItem;
            if(item->parent()){
                this->m_currentItem = this->m_currentItem->parent();
                this->m_currentItem->removeChild(item);
                delete item;
            }
            else{
                this->m_currentItem->parent()->removeChild(this->m_currentItem);
                delete this->m_currentItem;
                this->m_currentItem = nullptr;
            }
        }
    }
}

void ProjectFileTreeView::renameItem()
{
    // 修改 item
    this->editItem(this->m_currentItem);

    // 为 item 的 itemChanged 事件创建响应链接
    auto conn = std::make_shared<QMetaObject::Connection>();
    *conn = connect(this, &ProjectFileTreeView::itemChanged, this, [this, conn](QTreeWidgetItem *item, int column){
        // 删除 item 的 itemChanged 事件响应链接
        disconnect(*conn);

        // 保存原文件信息
        QFileInfo fileInfo(item->toolTip(0));


        if(item->text(0).isEmpty()){
            // 新名称为空，取消重命名并设置 item 的 text 为原名称
            QMessageBox::information(nullptr, "提示", "名称不能为空！");
            item->setText(0, fileInfo.fileName());
            return;
        }

        // 新文件信息
        QFileInfo newkFile(fileInfo.absolutePath() + "/" + item->text(0));


        if(newkFile.exists()){
            // 名称被占用，取消重命名并设置 item 的 text 为原名称
            QMessageBox::information(nullptr, "重命名失败", "名称 \"" + newkFile.fileName() + "\" 已被占用，请选取其他名称");
            item->setText(0, fileInfo.fileName());
            return;
        }

        if (fileInfo.isFile()){
            // 文件重命名
            QFile::rename(fileInfo.absoluteFilePath(), newkFile.absoluteFilePath());
        }
        if (fileInfo.isDir()){
            // 目录重命名
            QDir dir;
            dir.rename(fileInfo.absoluteFilePath(), newkFile.absoluteFilePath());
        }

        // 更新 item 信息
        item->setToolTip(0, newkFile.absoluteFilePath());
        item->setIcon(0, this->GetIcon(newkFile));

    });
}

void ProjectFileTreeView::searchInDir()
{

}

void ProjectFileTreeView::searchInFile()
{

}

void ProjectFileTreeView::openItemDir()
{
    QFileInfo fileInfo(this->m_currentItem->toolTip(0));
    QDesktopServices::openUrl(QUrl("file:///" + fileInfo.absolutePath()));
}

void ProjectFileTreeView::openFile()
{
    QString absolutePath = this->m_currentItem->toolTip(0);
    QFileInfo fileInfo(absolutePath);
    if (fileInfo.isFile()){
        // 打开文件
        emit openFile(fileInfo);
    }
}

void ProjectFileTreeView::copyAbsolutePath()
{
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(this->m_currentItem->toolTip(0));
}

void ProjectFileTreeView::onDirMenuTriggered(QAction *action)
{
    // 新建文件
    if (action->text() == "新建文件"){
        this->addFile();
    }
    // 新建文件夹
    if (action->text() == "新建文件夹"){
        this->addDir();
    }
    // 打开所在文件夹
    if (action->text() == "打开所在文件夹"){
        this->openItemDir();
    }
    // 重命名
    if (action->text() == "重命名"){
        this->renameItem();
    }
    // 在文件夹中查找
    if (action->text() == "在文件夹中查找"){
        this->searchInDir();
    }
    // 删除文件夹
    if (action->text() == "删除文件夹"){
        this->deleteItem();
    }
}

void ProjectFileTreeView::onFileMenuTriggered(QAction *action)
{
    // 打开文件
    if (action->text() == "打开"){
        this->openFile();
    }

    // 打开所在文件夹
    if (action->text() == "打开所在文件夹"){
        this->openItemDir();
    }

    // 复制绝对路径
    if (action->text() == "复制绝对路径"){
        this->copyAbsolutePath();
    }

    // 重命名
    if (action->text() == "重命名"){
        this->renameItem();
    }

    // 删除文件
    if (action->text() == "删除"){
        this->deleteItem();
    }
}

QIcon ProjectFileTreeView::GetIcon(QFileInfo fileInfo)
{
    if (fileInfo.isDir()){
        return QIcon(QPixmap(":/icons/icons/icon_dir.svg"));
    }

    QString fileSuffix = fileInfo.suffix();
    if (fileSuffix == "cpp"){
        return QIcon(QPixmap(":/icons/icons/icon_file_cpp.svg"));
    }

    if (fileSuffix == "h"){
        return QIcon(QPixmap(":/icons/icons/icon_file_hpp.svg"));
    }

    if (fileSuffix == "c"){
        return QIcon(QPixmap(":/icons/icons/icon_file_c.svg"));
    }

    if (fileSuffix == "txt"){
        return QIcon(QPixmap(":/icons/icons/icon_file_txt.svg"));
    }
    return QIcon(QPixmap(":/icons/icons/icon_file.svg"));
}

ProjectOperation::ProjectOperation(QWidget *parent) : QWidget(parent)
{


    this->m_label = new QLabel(this);

    this->m_openDir = new QPushButton(this);
    this->m_openDir->setToolTip(tr("打开目录"));
    this->m_openDir->setIcon(QIcon(QPixmap(":/icons/icons/icon_dir_open.svg")));
    this->m_openDir->setContentsMargins(0, 0, 0, 0);
    this->m_openDir->setMaximumSize(this->m_openDir->height() * 1.5, this->m_openDir->height());
    connect(this->m_openDir, &QPushButton::clicked, [this](){
        emit this->openDir();
    });

    this->m_addDir = new QPushButton(this);
    this->m_addDir->setToolTip(tr("添加目录"));
    this->m_addDir->setIcon(QIcon(QPixmap(":/icons/icons/icon_dir_add.svg")));
    this->m_addDir->setMaximumSize(this->m_addDir->height() * 1.5, this->m_addDir->height());
    connect(this->m_addDir, &QPushButton::clicked, [this](){
        emit this->addDir();
    });

    this->m_addFile = new QPushButton(this);
    this->m_addFile->setToolTip(tr("添加文件"));
    this->m_addFile->setIcon(QIcon(QPixmap(":/icons/icons/icon_file_add.svg")));
    this->m_addFile->setMaximumSize(this->m_addFile->height() * 1.5, this->m_addFile->height());
    connect(this->m_addFile, &QPushButton::clicked, [this](){
        emit this->addFile();
    });

    this->m_deleteItem = new QPushButton(this);
    this->m_deleteItem->setToolTip(tr("删除"));
    this->m_deleteItem->setIcon(QIcon(QPixmap(":/icons/icons/icon_item_delete.svg")));
    this->m_deleteItem->setMaximumSize(this->m_deleteItem->height() * 1.5, this->m_deleteItem->height());
    connect(this->m_deleteItem, &QPushButton::clicked, [this](){
        emit this->deleteItem();
    });

    QHBoxLayout *hlayout = new QHBoxLayout(this);
    hlayout->addWidget(this->m_label);
    hlayout->addStretch();
    hlayout->addWidget(m_openDir);
    hlayout->addWidget(m_addDir);
    hlayout->addWidget(m_addFile);
    hlayout->addWidget(m_deleteItem);
    hlayout->setMargin(6);
    hlayout->setSpacing(6);
}

void ProjectOperation::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    p.setPen(QColor("gray"));
    p.drawLine(-100, height() -1, width(), height() -1);
}

void ProjectOperation::SetLabel(QString label)
{
    this->m_label->setText(label);
}
