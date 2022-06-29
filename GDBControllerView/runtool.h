#ifndef RUNTOOL_H
#define RUNTOOL_H

#include <QObject>
#include <QWidget>
#include <QFileInfo>
#include <QPushButton>
#include <QProcess>
#include <QEventLoop>

/**
 * @brief The RunTool class 程序运行工具
 */
class RunTool : public QWidget
{
    Q_OBJECT
public:
    RunTool(QWidget *parent = nullptr);

    // 设置要运行的程序的目标文件
    inline void SetTargetProject(const QFileInfo &fileInfo){this->m_target = fileInfo;}

signals:
    // 发送程序运行的输出结果
    void programOutputSend(QString);

private slots:
    void onRun();                           // 运行按钮点击事件处理函数
    void onCompliedProcessReadOutput();     // 编译进程正常输出事件处理函数
    void onCompliedProcessReadError();      // 编译进程错误输出事件处理函数
    void onTragetProcessReadOutput();       // 目标程序进程输出事件处理函数

signals:
    void compliteFinished();                // 编译进程结束信号
    void targetFinished();                  // 目标程序进程结束信号


private:
    void Complie();     // 编译
    void RunTarget();   // 运行

private:
    // 要运行的程序的目标文件
    QFileInfo m_target;

    // gcc 进程
    QProcess *m_complieProcess;

    // 目标程序运行进程
    QProcess *m_targetProcess;

    // 运行按钮
    QPushButton *m_run;

    // 编译成功标识
    bool m_complieSucessed;

    // 事件循环
    QEventLoop *m_eventLoop;
};

#endif // RUNTOOL_H
