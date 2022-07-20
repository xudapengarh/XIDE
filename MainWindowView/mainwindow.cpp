#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSplitter>
#include <QFileDialog>
#include <QDebug>

#include "mainwindow.h"

#include "../ProjectFilesTreeView/projectfiletreeview.h"
#include "../SourceCodeEditorView/codeeditormanger.h"
#include "../GDBControllerView/buildanddebugtools.h"
#include "../ExpressionBrowserView/expressionbrowserview.h"
#include "../MassageBrowserView/massagebrowserview.h"
#include "../VariablesBrowserView/variablesbrowserview.h"
#include "../ProjectFilesTreeView/createnewdialog.h"
#include "../GDBControllerView/registerbrowser.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->resize(1200, 800);
    this->m_projectFileTreeView = new ProjectFileTreeView(this);

    this->m_sourceCodeEditorView = new CodeEditorManger(this);
    this->m_gdbControllerView = new BuildAndDebugTools(this);
    this->m_registerBrowserView = new RegisterBrowser(this);
//    this->m_massageBrowserView = new MassageBrowserView(this);
//    this->m_variablesBrowserView = new VariablesBrowserView(this);
//    this->m_expressionBrowserView = new ExpressionBrowserView(this);

    QSplitter *hlayout = new QSplitter(Qt::Horizontal, 0);
    hlayout->addWidget(this->m_projectFileTreeView);

    {
        QSplitter *vlayout = new QSplitter(Qt::Vertical, 0);
        vlayout->addWidget(this->m_sourceCodeEditorView);
        vlayout->addWidget(this->m_gdbControllerView);
        vlayout->setStretchFactor(0, 70);
        vlayout->setStretchFactor(1, 30);
        hlayout->addWidget(vlayout);
    }

    hlayout->addWidget(this->m_registerBrowserView);
    hlayout->setStretchFactor(0, 20);
    hlayout->setStretchFactor(1, 60);
    hlayout->setStretchFactor(2, 20);


    this->setCentralWidget(hlayout);

    this->m_menuBar = new QMenuBar(this);
    QMenu *fileMenu = this->m_menuBar->addMenu("文件");
    fileMenu->addAction("新建项目");
    fileMenu->addAction("打开项目");
    fileMenu->addSeparator();
    fileMenu->addAction("最近使用的项目");
    connect(fileMenu, &QMenu::triggered, this, &MainWindow::onFileMenuTriggered);

    this->m_projectFileTreeView->Init("/Users/xudapeng/Projects/GDBTest/");

    connect(this->m_projectFileTreeView, static_cast<void(ProjectFileTreeView::*)(QFileInfo)>(&ProjectFileTreeView::openFile), this->m_sourceCodeEditorView, &CodeEditorManger::onOpenFile);
    connect(this->m_projectFileTreeView, &ProjectFileTreeView::closeFile, this->m_sourceCodeEditorView, &CodeEditorManger::onCloseFile);

    connect(this->m_sourceCodeEditorView, &CodeEditorManger::currentCodeFileChanged, this->m_gdbControllerView, &BuildAndDebugTools::onCurrentCodeFileChanged);
    connect(this->m_gdbControllerView, &BuildAndDebugTools::updateFrame, this->m_sourceCodeEditorView, &CodeEditorManger::onUpdateFrame);
    connect(this->m_gdbControllerView, &BuildAndDebugTools::updateRegisters, this->m_registerBrowserView, &RegisterBrowser::onUpdateRegisters);

//    connect(this->m_gdbControllerView, &BuildAndDebugTools::massageSend, this->m_massageBrowserView, &MassageBrowserView::onShowMassage);
}

MainWindow::~MainWindow()
{
}

void MainWindow::onFileMenuTriggered(QAction *action)
{
    if (action->text() == "新建项目"){
        CreateNewDialog dialog;
        dialog.CreateDir();
        QString path = dialog.GetAbsolutePath();
        if(!path.isEmpty()){
            this->m_projectFileTreeView->Init(path);;
        }
    }
    else if(action->text() == "打开项目"){
        // 打开项目文件夹
        QString path = QFileDialog::getExistingDirectory(this, "选择目录", "./", QFileDialog::ShowDirsOnly);
        if(!path.isEmpty()){
            this->m_projectFileTreeView->Init(path);;
        }
    }
    else if(action->text() == "最近使用的项目"){

    }
}

