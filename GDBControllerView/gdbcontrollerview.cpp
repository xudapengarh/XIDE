#include <QHBoxLayout>
#include <QProcess>
#include <QDebug>
#include "gdbcontrollerview.h"


BuildAndDebugTools::BuildAndDebugTools(QWidget *parent) : QWidget(parent)
{

    // asm 代码
    this->m_asm = new GDBControllerButton(this);
    this->m_asm->setIcon(QIcon(QPixmap(":/GDB_Icons/icons/img_asm.svg")));
    this->m_asm->setToolTip("Asm Code");
    //    this->m_asm->setMaximumWidth(this->height());

    // 源代码
    this->m_src = new GDBControllerButton(this);
    this->m_src->setIcon(QIcon(QPixmap(":/GDB_Icons/icons/img_src.svg")));
    this->m_src->setToolTip("Src Code");

    // 调试按钮
    this->m_debug = new GDBControllerButton(this);
    this->m_debug->setIcon(QIcon(QPixmap(":/GDB_Icons/icons/img_debug.svg")));
    this->m_debug->setToolTip("Debug");

    // 运行按钮
    this->m_run = new GDBControllerButton(this);
    this->m_run->setIcon(QIcon(QPixmap(":/GDB_Icons/icons/img_run.svg")));
    this->m_run->setToolTip("Run");


    this->m_stepOver = new GDBControllerButton(this);
    this->m_stepOver->setIcon(QIcon(QPixmap(":/GDB_Icons/icons/img_step_over.svg")));
    this->m_stepOver->setToolTip("Step Over");


    this->m_stepInto = new GDBControllerButton(this);
    this->m_stepInto->setIcon(QIcon(QPixmap(":/GDB_Icons/icons/img_step_into.svg")));
    this->m_stepInto->setToolTip("Step Into");


    this->m_stepOut = new GDBControllerButton(this);
    this->m_stepOut->setIcon(QIcon(QPixmap(":/GDB_Icons/icons/img_step_out.svg")));
    this->m_stepOut->setToolTip("Step Out");


    this->m_stepTo = new GDBControllerButton(this);
    this->m_stepTo->setIcon(QIcon(QPixmap(":/GDB_Icons/icons/img_step_to.svg")));
    this->m_stepTo->setToolTip("Run To");


    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(this->m_asm);
    layout->addWidget(this->m_src);
    layout->addWidget(this->m_debug);
    layout->addWidget(this->m_run);
    layout->addWidget(this->m_stepOver);
    layout->addWidget(this->m_stepInto);
    layout->addWidget(this->m_stepOut);
    layout->addWidget(this->m_stepTo);
    //    layout->addStretch();
    layout->setContentsMargins(0, 0, 10, 0);
    layout->setSpacing(5);
    layout->setAlignment(Qt::AlignLeft);
    this->setMaximumHeight(40);
    this->setMinimumHeight(40);

    connect(this->m_run, &QPushButton::clicked, this, &BuildAndDebugTools::onRunCode);


    this->m_process = new QProcess();
    connect(this->m_process, &QProcess::readyReadStandardOutput, this, &BuildAndDebugTools::onProcessReadOutput);
    connect(this->m_process, &QProcess::readyReadStandardError, this, &BuildAndDebugTools::onProcessReadError);
}

void BuildAndDebugTools::onRunCode()
{
    if (this->m_process->state() != QProcess::Running){
        this->m_process->start("bash");
        this->m_process->waitForStarted();
    }

    QString fileSuffix = this->m_currentCodeFileInfo.suffix();

    QString gccCmdType = "";
    if (fileSuffix == "cpp"){
        gccCmdType = "g++ ";
    }

    if (fileSuffix == "c"){
        gccCmdType = "gcc ";
    }

    QString cmd = gccCmdType + m_currentCodeFileInfo.filePath() + " -o " + m_currentCodeFileInfo.absolutePath() + "/" + this->m_currentCodeFileInfo.baseName() + "\n";
    qDebug() << cmd;
    this->m_process->write(cmd.toLocal8Bit());
    qDebug() << cmd;
    cmd = m_currentCodeFileInfo.absolutePath() + "/" + this->m_currentCodeFileInfo.baseName() + "\n";
    this->m_process->write(cmd.toLocal8Bit());
}

void BuildAndDebugTools::onProcessReadOutput()
{
    QByteArray out = this->m_process->readAllStandardOutput();
    emit massageSend(QString::fromLocal8Bit(out));

}

void BuildAndDebugTools::onProcessReadError()
{
    QByteArray out = this->m_process->readAllStandardError();
    this->m_process->kill();
    emit massageSend(QString::fromLocal8Bit(out));
}
