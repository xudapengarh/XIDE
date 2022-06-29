#include <QDebug>

#include "lldbdebuger.h"

LLDBDebuger::LLDBDebuger(QObject *parent) : Debuger(parent)
{
    connect(this, &QProcess::readyReadStandardOutput, this, &LLDBDebuger::onProcessReadOutput);
    connect(this, &QProcess::readyReadStandardError, this, &LLDBDebuger::onProcessReadError);
}

bool LLDBDebuger::Debug(QString processName)
{
    QStringList param;
    param << processName;
    this->start("lldb", param);
    return this->waitForStarted(1000);
}

void LLDBDebuger::Run()
{
    if (this->state() == QProcess::Running){
        this->write("run\n");
        this->waitForReadyRead();
    }
}

void LLDBDebuger::Continue()
{

}

void LLDBDebuger::Next()
{

}

void LLDBDebuger::Nexti()
{

}

void LLDBDebuger::AddBreakPoint(int breakpoint)
{

}

void LLDBDebuger::AddBreakPoint(QString breakPoint)
{

}

void LLDBDebuger::Step()
{

}

void LLDBDebuger::StepOut()
{

}

void LLDBDebuger::onProcessReadOutput()
{
    QString out = QString::fromLocal8Bit(this->readAllStandardOutput());
    QStringList lines = out.split('\n');
    for (int i = 0; i < lines.size(); i++){
        if(lines[i].contains("frame")){
            i = AnalysisFrameInfo(lines, i);
            continue;
        }
        if(lines[i].contains("General Purpose Registers:")){
            i = AnalysisRegisterInfo(lines, i);
            continue;
        }
    }
}

void LLDBDebuger::onProcessReadError()
{

}

void LLDBDebuger::OutputFilter()
{

}

int LLDBDebuger::AnalysisFrameInfo(QStringList &lines, int index)
{
    FrameInfo frameInfo;
    QRegExp lineInfo("(^ *[->]* *\\d+)(.*)");
    QRegExp frameEnd("^Target *\\d*: *\\(.*\\) * stopped.");
    for (int i = index + 1; i < lines.size(); i++){
        if (lines[i].indexOf(frameEnd) == 0){
            return i + 1;
        }
        if(lines[i].indexOf(lineInfo) != -1){
            QString code = lineInfo.cap(2);
            QString number = lineInfo.cap(1);
            if(number.contains("->")){
                number = number.replace("->", "");
                number = number.replace(" ", "");
                frameInfo.currentLine = number.toInt();
            }
            code = code.replace("  \t", "");
            number = number.replace(" ", "");
            frameInfo.codes.append(code);
            frameInfo.codeLineNumbers.append(number.toInt());
            qDebug() << number << ":" << code;
        }
    }
    return index;
}

int LLDBDebuger::AnalysisRegisterInfo(QStringList &lines, int index)
{

}


