#include <QGridLayout>
#include <QEventLoop>
#include <QDebug>

#include "runtool.h"

RunTool::RunTool(QWidget *parent) : QWidget(parent)
{
    // 初始化目标文件信息
    this->m_target.setFile("");

    // 事件循环（防止编译及运行时界面卡顿）
    this->m_eventLoop = new QEventLoop(this);
    // 编译完成，结束循环
    connect(this, &RunTool::compliteFinished, this->m_eventLoop, &QEventLoop::quit);
    // 运行完成，结束循环
    connect(this, &RunTool::targetFinished, this->m_eventLoop, &QEventLoop::quit);

    // 编译进程
    this->m_complieProcess = new QProcess(this);
    connect(this->m_complieProcess, &QProcess::readyReadStandardOutput, this, &RunTool::onCompliedProcessReadOutput);
    connect(this->m_complieProcess, &QProcess::readyReadStandardError, this, &RunTool::onCompliedProcessReadError);
    connect(this->m_complieProcess, static_cast<void(QProcess::*)(int)>(&QProcess::finished), this, [=](int exitCode){
        // 获取编译器退出值 exitCode ：0 编译成功；exitCode = 1 编译失败
        this->m_complieSucessed = exitCode == 0 ? true : false;
        emit compliteFinished();
    });

    // 目标程序运行进程
    this->m_targetProcess = new QProcess(this);
    connect(this->m_targetProcess, &QProcess::readyReadStandardOutput, this, &RunTool::onTragetProcessReadOutput);
    connect(this->m_targetProcess, static_cast<void(QProcess::*)(int)>(&QProcess::finished), this, [=](int exitCode){
        emit targetFinished();
    });



    // 初始化运行按钮
    this->m_run = new QPushButton(this);
    this->m_run->setIcon(QIcon(QPixmap(":/GDB_Icons/icons/img_run.svg")));
    this->m_run->setToolTip("Run");
    connect(this->m_run, &QPushButton::clicked, this, &RunTool::onRun);

    // 设置布局
    QHBoxLayout *glayout = new QHBoxLayout(this);
    glayout->addWidget(this->m_run);
    glayout->setMargin(0);

    // 初始化编译成功标识
    this->m_complieSucessed= false;
}

void RunTool::onRun()
{
    // 判断目标程序是否 正在编译或者运行
    if(this->m_complieProcess->state() == QProcess::Running || this->m_targetProcess->state() == QProcess::Running){
        // 设置按钮图标为关闭
        this->m_run->setIcon(QIcon(QPixmap(":/GDB_Icons/icons/img_run.svg")));
        if (this->m_complieProcess->state() == QProcess::Running){  // 编译进程运行中，关闭编译进程
            this->m_complieProcess->kill();
        }
        if (this->m_targetProcess->state() == QProcess::Running){   // 目标程序运行中，关闭目标程序
            this->m_targetProcess->kill();
        }
    }
    else{
        // 设置按钮图标为运行
        this->m_run->setIcon(QIcon(QPixmap(":/GDB_Icons/icons/img_stop.svg")));
        Complie();      // 编译
        RunTarget();    // 运行
        this->m_run->setIcon(QIcon(QPixmap(":/GDB_Icons/icons/img_run.svg")));
    }
}

void RunTool::onCompliedProcessReadOutput()
{
    // 捕获编译程序输出
    QByteArray out = this->m_complieProcess->readAllStandardOutput();
    emit programOutputSend(QString::fromLocal8Bit(out));
    emit compliteFinished();
}

void RunTool::onCompliedProcessReadError()
{
    // 捕获编译程序错误输出
    QByteArray out = this->m_complieProcess->readAllStandardError();
    emit programOutputSend(QString::fromLocal8Bit(out));
    emit compliteFinished();
}


void RunTool::onTragetProcessReadOutput()
{
    // 捕获目标程序输出
    QByteArray out = this->m_targetProcess->readAllStandardOutput();
    emit programOutputSend(QString::fromLocal8Bit(out));
    emit targetFinished();
}

void RunTool::Complie()
{
    // 判断目标代码文件是否存在
    if (this->m_target.filePath().isEmpty() || this->m_target.isDir()){
        emit programOutputSend("编译目标不存在");
        return;
    }

    if (this->m_complieProcess->state() == QProcess::Running){
        return;
    }

    // 获取文件后缀， 根据文件类型初始化编译命令
    QString fileSuffix = this->m_target.suffix();

    QString pcomplierName = "";

    if (fileSuffix == "cpp"){   // C++ 源代码
        pcomplierName = "g++";
    }

    if (fileSuffix == "c"){     // C 源代码
        pcomplierName = "gcc";
    }

    if (pcomplierName.isEmpty()){   // 不能识别的源代码
        emit programOutputSend(this->m_target.filePath() + " 不是可识别的源代码文件");
        return;
    }

    // 编译器参数列表
    QStringList  paramList;
    paramList << this->m_target.filePath();
    paramList << "-o";
    paramList << this->m_target.absolutePath() + "/" + this->m_target.baseName();

    // 执行编译
    this->m_complieProcess->start(pcomplierName, paramList);

    // 等待编译进程结束
    this->m_eventLoop->exec();
}

void RunTool::RunTarget()
{
    //判断是否编译成功
    if (this->m_complieSucessed){
        // 程序路径名称
        QString processName = this->m_target.absolutePath() + "/" + this->m_target.baseName();

        // 运行程序
        this->m_targetProcess->start(processName);

        // 等待程序结束
        this->m_eventLoop->exec();
    }
}
