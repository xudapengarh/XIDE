#include <QDebug>
#include <QString>
#include "lldbdebuger.h"

LLDBDebuger::LLDBDebuger(QObject *parent) : Debuger(parent)
{
    this->m_frameType = FrameType::SRC;
    connect(this, &QProcess::readyReadStandardOutput, this, &LLDBDebuger::onProcessReadOutput);
    connect(this, &QProcess::readyReadStandardError, this, &LLDBDebuger::onProcessReadError);
    this->m_registerUpdateFlag = true;
}

bool LLDBDebuger::Debug(QString processName)
{
    QStringList param;
    param << "--debug";
    param << processName;
    this->start("lldb", param);
    return this->waitForStarted(1000);
}

void LLDBDebuger::SetFrameType(FrameType type)
{
    this->m_frameType = type;
}

void LLDBDebuger::Run()
{
    if (this->state() == QProcess::Running){
        this->write("run\n");
        this->waitForReadyRead();
    }
    //    GetRegisters();
}

void LLDBDebuger::Continue()
{
    if (this->state() == QProcess::Running){
        this->write("continue\n");
        this->waitForReadyRead();
    }
    //    GetRegisters();
}

void LLDBDebuger::Next()
{
    if (this->state() == QProcess::Running){
        this->write("next\n");
        this->waitForReadyRead();
    }
    //    GetRegisters();
}

void LLDBDebuger::Nexti()
{
    if (this->state() == QProcess::Running){
        this->write("nexti\n");
        this->waitForReadyRead();
    }
    //    GetRegisters();
}

void LLDBDebuger::AddBreakPoint(int breakpoint)
{

}

void LLDBDebuger::AddBreakPoint(QString breakPoint)
{
    if (this->state() == QProcess::Running){
        QString cmd = "breakpoint set -n " + breakPoint + "\n";
        this->write(cmd.toLocal8Bit());
        this->waitForReadyRead();
    }
    //    GetRegisters();
}

void LLDBDebuger::Step()
{
    if (this->state() == QProcess::Running){

        this->write("step\n");
        this->waitForReadyRead();
    }
    //    GetRegisters();
}

void LLDBDebuger::StepOut()
{
    if (this->state() == QProcess::Running){
        this->write("finish\n");
        this->waitForReadyRead();
    }
    //    GetRegisters();
}

void LLDBDebuger::onProcessReadOutput()
{
    QString out = QString::fromLocal8Bit(this->readAllStandardOutput());
    if (out.contains("lldb")){
        return;
    }
    QStringList lines = out.split('\n');
    for (int i = 0; i < lines.size(); i++){
        if(lines[i].contains("frame")){
            i = AnalysisFrameInfo(lines, i);
            GetRegisters();
            continue;;
        }
        if(lines[i].contains("Registers:")){
            i = AnalysisRegisterInfo(lines, i);
            GetFrameVariable();
            continue;;
        }
        i = AnalysisFrameVariableInfo(lines, i);
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

    if (index < lines.size() && lines[index].contains(" at ")){
        FrameInfo frameInfo;
        QStringList fileInfo = lines[index].split("at ")[1].split(":");
        frameInfo.file = fileInfo[0];
        frameInfo.currentLine = fileInfo[1].toInt();
        emit updateFrame(frameInfo);
        return lines.size();
    }
    else{
        return lines.size();
    }


}

int LLDBDebuger::AnalysisRegisterInfo(QStringList &lines, int index)
{

    RegisterGroup registerGroup;
    QRegExp normalreg("(\\w+) = (\\w+)");
    QRegExp vecreg("(\\w*) = (\\{\\w*.*\\})");
    for (int i = index; i < lines.size(); i++){
        if (lines[i].contains("Registers:")){
            QString group = lines[i].replace(":", "");
            Register registerInfo;
            registerGroup.groupName.append(group);
            registerGroup.registerInfo.append(registerInfo);
            continue;
        }
        if(lines[i].indexOf(normalreg) != -1){
            registerGroup.registerInfo.last().registerNames.append(normalreg.cap(1).replace("\n", ""));
            registerGroup.registerInfo.last().values.append(normalreg.cap(2).replace("\n", "").toUtf8());
        }
        if(lines[i].indexOf(vecreg) != -1){
            registerGroup.registerInfo.last().registerNames.append(vecreg.cap(1));
            QString value = vecreg.cap(2);

            value = value.replace("{", "");
            value = value.replace("}", "");
            QStringList values = value.split(" ");
            QByteArray bt;
            for (int j = 0; j < values.size(); j++){
                bt.append(values[j].toUtf8());
            }
            registerGroup.registerInfo.last().values.append(bt);
        }
    }
    emit updateRegisters(registerGroup);
    return lines.size();
}

int LLDBDebuger::AnalysisFrameVariableInfo(QStringList &lines, int index)
{
    FrameVariable variable;
    QRegExp reg("\\((.*)\\) (\\w+) = (.*)");
    for (int i = index; i < lines.size(); i++){
        if(lines[i].indexOf(reg) == -1){
            if (variable.variableName.size() > 0){
                emit updateFrameVariable(variable);
            }
            return i;
        }
        else{
            variable.variableType.append(reg.cap(1));
            variable.variableName.append(reg.cap(2));
            variable.variableValue.append(reg.cap(3));
        }
    }
    if (variable.variableName.size() > 0){
        emit updateFrameVariable(variable);
    }
    return lines.size();
}

void LLDBDebuger::GetRegisters()
{
    //    this->m_registerUpdateFlag = false;
    if (this->state() == QProcess::Running){
        this->write("register read -all\n");
        this->waitForReadyRead();
    }
}

void LLDBDebuger::GetFrameVariable()
{
    if (this->state() == QProcess::Running){
        this->write("frame variable\n");
        this->waitForReadyRead();
    }
}


