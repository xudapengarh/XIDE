#ifndef LLDBDEBUGER_H
#define LLDBDEBUGER_H

#include <QObject>

#include "debuger.h"
class LLDBDebuger : public Debuger
{
    Q_OBJECT
public:
    LLDBDebuger(QObject *parent = nullptr);

    // Debuger interface
public:

    // Debuger interface
public:
    bool Debug(QString processName) override;
    void SetFrameType(FrameType type) override;
    void Run() override;
    void Continue() override;
    void Next() override;
    void Nexti() override;
    void AddBreakPoint(int breakpoint) override;
    void AddBreakPoint(QString breakPoint) override;

    void Step() override;
    void StepOut() override;

private slots:
    void onProcessReadOutput() override;
    void onProcessReadError() override;

private:
    void OutputFilter() override;
    int AnalysisFrameInfo(QStringList &lines, int index) override;
    int AnalysisRegisterInfo(QStringList &lines, int index) override;
    int AnalysisFrameVariableInfo(QStringList &lines, int index) override;

private:
    void GetRegisters();
    void GetFrameVariable();

private:
    FrameType m_frameType;
    bool m_frameUpdateFlag;
    bool m_registerUpdateFlag;

};

#endif // LLDBDEBUGER_H
