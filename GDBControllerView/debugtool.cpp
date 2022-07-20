#include <QHBoxLayout>
#include <QDebug>

#include "debugtool.h"
#include "lldbdebuger.h"


DebugTool::DebugTool(QWidget *parent) : QWidget(parent)
{
    // 开始调试
    this->m_debug = new QPushButton(this);
    this->m_debug->setIcon(QIcon(QPixmap(":/GDB_Icons/icons/img_debug.svg")));
    this->m_debug->setToolTip("Start Debug");
    connect(this->m_debug, &QPushButton::clicked, this, &DebugTool::onDebug);


    // 源代码模式
    this->m_src = new QPushButton(this);
    this->m_src->setIcon(QIcon(QPixmap(":/GDB_Icons/icons/img_src.svg")));
    this->m_src->setToolTip("Src Code");
    connect(this->m_src, &QPushButton::clicked, this, &DebugTool::onAsm);


    // 汇编模式
    this->m_asm = new QPushButton(this);
    this->m_asm->setIcon(QIcon(QPixmap(":/GDB_Icons/icons/img_asm.svg")));
    this->m_asm->setToolTip("Asm Code");

    // 继续运行
    this->m_continue = new QPushButton(this);
    this->m_continue->setIcon(QIcon(QPixmap(":/GDB_Icons/icons/img_continue.svg")));
    this->m_continue->setToolTip("Continue");
    connect(this->m_continue, &QPushButton::clicked, this, &DebugTool::onContinue);

    // 下一步
    this->m_stepOver = new QPushButton(this);
    this->m_stepOver->setIcon(QIcon(QPixmap(":/GDB_Icons/icons/img_step_over.svg")));
    this->m_stepOver->setToolTip("Step Over");
    connect(this->m_stepOver, &QPushButton::clicked, this, &DebugTool::onStepOver);

    // 进入
    this->m_stepInto = new QPushButton(this);
    this->m_stepInto->setIcon(QIcon(QPixmap(":/GDB_Icons/icons/img_step_into.svg")));
    this->m_stepInto->setToolTip("Step Into");
    connect(this->m_stepInto, &QPushButton::clicked, this, &DebugTool::onStepInto);

    // 跳出
    this->m_stepOut = new QPushButton(this);
    this->m_stepOut->setIcon(QIcon(QPixmap(":/GDB_Icons/icons/img_step_out.svg")));
    this->m_stepOut->setToolTip("Step Out");
    connect(this->m_stepOut, &QPushButton::clicked, this, &DebugTool::onStepOut);

    // 运行至
    this->m_stepTo = new QPushButton(this);
    this->m_stepTo->setIcon(QIcon(QPixmap(":/GDB_Icons/icons/img_step_to.svg")));
    this->m_stepTo->setToolTip("Run To");
    connect(this->m_stepTo, &QPushButton::clicked, this, &DebugTool::onStepTo);

    // 设置布局
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(this->m_debug);
    //    layout->addWidget(this->m_stop);
    layout->addWidget(this->m_src);
    layout->addWidget(this->m_asm);
    layout->addWidget(this->m_continue);
    layout->addWidget(this->m_stepOver);
    layout->addWidget(this->m_stepInto);
    layout->addWidget(this->m_stepOut);
    layout->addWidget(this->m_stepTo);
    layout->setSpacing(5);
    layout->setAlignment(Qt::AlignLeft);
    layout->setMargin(0);

    // 设置扩展按钮不可见
    SetExtendButtonVisible(false);

    // 初始化目标文件信息
    this->m_target.setFile("");

    // 事件循环（防止编译及运行时界面卡顿）
    this->m_eventLoop = new QEventLoop(this);

    // 编译完成，结束循环
    connect(this, &DebugTool::compliteFinished, this->m_eventLoop, &QEventLoop::quit);
    // 运行完成，结束循环
    connect(this, &DebugTool::debugFinished, this->m_eventLoop, &QEventLoop::quit);

    // 编译进程
    this->m_complieProcess = new QProcess(this);
//    this->m_complieProcess->setProcessChannelMode(QProcess::MergedChannels);
    connect(this->m_complieProcess, &QProcess::readyReadStandardOutput, this, &DebugTool::onCompliedProcessReadOutput);
    connect(this->m_complieProcess, &QProcess::readyReadStandardError, this, &DebugTool::onCompliedProcessReadError);
    connect(this->m_complieProcess, static_cast<void(QProcess::*)(int)>(&QProcess::finished), this, [=](int exitCode){
        // 获取编译器退出值 exitCode ：0 编译成功；exitCode = 1 编译失败
        this->m_complieSucessed = exitCode == 0 ? true : false;
        emit compliteFinished();
    });

    // 调试器
    this->m_debuger = new LLDBDebuger(this);
    connect(this->m_debuger, &Debuger::updateFrame, this, &DebugTool::onUpdateFrame);
    connect(this->m_debuger, &Debuger::updateRegisters, this, &DebugTool::onUpdateRegisters);
    connect(this->m_debuger, &Debuger::updateFrameVariable, this, &DebugTool::onUpdateFrameVariable);

    // 初始化编译成功标识
    this->m_complieSucessed = false;


}

void DebugTool::onDebug()
{
    if(this->m_complieProcess->state() == QProcess::Running || this->m_debuger->state() == QProcess::Running){
        // 设置按钮图标为关闭
        if (this->m_complieProcess->state() == QProcess::Running){  // 编译进程运行中，关闭编译进程
            this->m_complieProcess->kill();
        }
        if (this->m_debuger->state() == QProcess::Running){   // 目标程序运行中，关闭目标程序
            this->m_debuger->kill();
        }
        this->m_debug->setIcon(QIcon(QPixmap(":/GDB_Icons/icons/img_debug.svg")));
        this->m_debug->setToolTip("Start Debug");
        SetExtendButtonVisible(false);
    }
    else{
        Complie();      // 编译
        // 设置扩展按钮不可见
        SetExtendButtonVisible(true);
        // 设置汇编代码模式不可见
        this->m_asm->setVisible(false);
        DebugTarget();  // 调试
        this->m_debug->setIcon(QIcon(QPixmap(":/GDB_Icons/icons/img_stop.svg")));
        this->m_debug->setToolTip("Stop Debug");
    }
}

void DebugTool::onContinue()
{
    this->m_debuger->Continue();
}

void DebugTool::onStepOver()
{
    this->m_debuger->Next();
}

void DebugTool::onStepInto()
{
    this->m_debuger->Step();
}

void DebugTool::onStepOut()
{
    this->m_debuger->StepOut();
}

void DebugTool::onStepTo()
{
    this->m_debuger->StepOut();
}

void DebugTool::onAsm()
{
//    if (this->m_debugProcess->state() == QProcess::Running){

//        this->m_debugProcess->write("register read -all\n");
//        this->m_debugProcess->waitForBytesWritten();
//    }
    this->m_debuger->SetFrameType(FrameType::ASM);
}

void DebugTool::onUpdateFrame(const FrameInfo &frame)
{
    emit updateFrame(frame);
}

void DebugTool::onUpdateRegisters(const RegisterGroup &registers)
{
    emit updateRegisters(registers);
}

void DebugTool::onUpdateFrameVariable(const FrameVariable &variables)
{
    emit updateFrameVariable(variables);
}

void DebugTool::onCompliedProcessReadOutput()
{
    // 捕获编译程序输出
    QByteArray out = this->m_complieProcess->readAllStandardOutput();
    emit programOutputSend(QString::fromLocal8Bit(out));
    emit compliteFinished();
}

void DebugTool::onCompliedProcessReadError()
{
    // 捕获编译程序错误输出
    QByteArray out = this->m_complieProcess->readAllStandardError();
    emit programOutputSend(QString::fromLocal8Bit(out));
    emit compliteFinished();
}

//void DebugTool::onDebugProcessReadOutput()
//{
//    qDebug() << __FUNCTION__;
//    QByteArray outba = this->m_debugProcess->readAllStandardOutput();
//    QString out = QString::fromUtf8(outba);
//    QStringList lines = out.split('\n');
//    for (int i = 0; i < lines.size(); i++){
//        if(lines[i].contains("frame")){
//            i = this->m_debuger->AnalysisFrameInfo(lines, i);
//            continue;
//        }
//        if(lines[i].contains("General Purpose Registers:")){
//            i = this->m_debuger->AnalysisRegisterInfo(lines, i);
//            continue;
//        }
//    }
//    m_inbuffer.append(out);
//    emit programOutputSend(out);
//}


void DebugTool::Complie()
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
    paramList << "-g";
    paramList << this->m_target.filePath();
    paramList << "-o";
    paramList << this->m_target.absolutePath() + "/" + this->m_target.baseName();

    // 执行编译
    this->m_complieProcess->start(pcomplierName, paramList);

    // 等待编译进程结束
    this->m_eventLoop->exec();
}

void DebugTool::DebugTarget()
{
    if (this->m_complieSucessed){
        this->m_debuger->Debug(this->m_target.absolutePath() + "/" + this->m_target.baseName());

        this->m_debuger->AddBreakPoint("main");

        this->m_debuger->Run();
    }
}

void DebugTool::SetExtendButtonVisible(bool visible)
{
    this->m_src->setVisible(visible);
    this->m_asm->setVisible(visible);
    this->m_continue->setVisible(visible);
    this->m_stepOver->setVisible(visible);
    this->m_stepInto->setVisible(visible);
    this->m_stepOut->setVisible(visible);
    this->m_stepTo->setVisible(visible);
}
