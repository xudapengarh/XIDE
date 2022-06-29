#ifndef DEBUGTOOL_H
#define DEBUGTOOL_H

#include <QObject>
#include <QWidget>
#include <QPushButton>
#include <QFileInfo>
#include <QProcess>
#include <QEventLoop>

#include "debuger.h"
/**
 * @brief The DebugTool class 程序调试工具
 */
class DebugTool : public QWidget
{
    Q_OBJECT
public:
    DebugTool(QWidget *parent = nullptr);

    // 设置要运行的程序的目标文件
    inline void SetTargetProject(const QFileInfo &fileInfo){this->m_target = fileInfo;}


signals:
    // 发送程序运行的输出结果
    void programOutputSend(QString);

private slots:
    void onCompliedProcessReadOutput();     // 编译进程正常输出事件处理函数
    void onCompliedProcessReadError();      // 编译进程错误输出事件处理函数
    void onDebugProcessReadOutput();        // 调试进程输出事件处理函数

private slots:
    void onDebug();                         // 运行按钮点击事件处理函数
    void onContinue();                      // 继续按钮点击事件处理函数
    void onStepOver();                      // 下一步按钮点击事件处理函数
    void onStepInto();                      // 进入按钮点击事件处理函数
    void onStepOut();                       // 跳出按钮点击事件处理函数
    void onStepTo();                        // 运行至按钮点击事件处理函数
    void onAsm();

signals:
    void compliteFinished();                // 编译进程结束信号
    void debugFinished();                   // debug 进程结束信号

private:
    void Complie();                             // 编译
    void DebugTarget();                         // 启动调试进程
    void SetExtendButtonVisible(bool visible);  // 设置调试所用到的相关扩展按钮可见性


private:
    QPushButton *m_debug;       // 开始调试
    QPushButton *m_src;         // 源码模式
    QPushButton *m_asm;         // 汇编模式
    QPushButton *m_continue;    // 继续运行
    QPushButton *m_stepOver;    // 下一步
    QPushButton *m_stepInto;    // 进入
    QPushButton *m_stepOut;     // 跳出
    QPushButton *m_stepTo;      // 运行至

    // 要运行的程序的目标文件
    QFileInfo m_target;

    // gcc 进程
    QProcess *m_complieProcess;

    // 调试进程
    QProcess *m_debugProcess;

    // 编译成功标识
    bool m_complieSucessed;

    // 事件循环
    QEventLoop *m_eventLoop;

    QByteArray m_standardOutput;

    QString m_inbuffer;

    Debuger *m_debuger;
};

#endif // DEBUGTOOL_H
