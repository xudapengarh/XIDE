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

public:
    void OutputFilter() override;
    int AnalysisFrameInfo(QStringList &lines, int index) override;
    int AnalysisRegisterInfo(QStringList &lines, int index) override;
};

#endif // LLDBDEBUGER_H
