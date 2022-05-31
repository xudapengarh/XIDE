#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>


class ProjectFileTreeView;      // 代码文件树视图
class CodeEditorManger;     // 代码编辑视图
class GDBControllerView;        // GDB 控制视图
class VariablesBrowserView;     // 变量监视视图
class MassageBrowserView;       // 消息显示视图
class ExpressionBrowserView;    // 表达式监视视图
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onFileMenuTriggered(QAction *action);


private:

    ProjectFileTreeView *m_projectFileTreeView;      // 代码文件树视图
    CodeEditorManger *m_sourceCodeEditorView;    // 源代码编辑视图
    GDBControllerView *m_gdbControllerView;           // GDB 控制视图
    MassageBrowserView *m_massageBrowserView;         // 消息显示视图
    ExpressionBrowserView *m_expressionBrowserView;   // 表达式监视视图
    VariablesBrowserView *m_variablesBrowserView;     // 变脸监视视图

private:
    QMenuBar *m_menuBar;
};
#endif // MAINWINDOW_H
