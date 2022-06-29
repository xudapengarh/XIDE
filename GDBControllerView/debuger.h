#ifndef DEBUGER_H
#define DEBUGER_H

#include <QObject>
#include <QWidget>
#include <QProcess>
#include <QVariant>

typedef  struct{
    QVector<QString> codes;
    QVector<int> codeLineNumbers;
    int currentLine;
}FrameInfo;

typedef struct{
    QVector<QString> registerNames;
//    QVector<char[16]> values;
}RegisterInfo;

class Debuger : public QProcess
{
    Q_OBJECT


public:
    Debuger(QObject *parent = nullptr);

    virtual bool Debug(QString processName) = 0;

    virtual void Run() = 0;

    virtual void Continue() = 0;

    virtual void Next() = 0;

    virtual void Step() = 0;

    virtual void StepOut() = 0;

    virtual void Nexti() = 0;

    virtual void AddBreakPoint(int breakpoint) = 0;

    virtual void AddBreakPoint(QString breakPoint) = 0;

signals:
    void updateFrame(FrameInfo *frame);
    void updateRegisters(RegisterInfo *registers);

private slots:
    virtual void onProcessReadOutput() = 0;
    virtual void onProcessReadError() = 0;


public:
    virtual void OutputFilter() = 0;
    virtual int AnalysisFrameInfo(QStringList &lines, int index) = 0;
    virtual int AnalysisRegisterInfo(QStringList &lines, int index) = 0;
};

#endif // DEBUGER_H
