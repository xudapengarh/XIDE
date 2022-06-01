#ifndef GDBCONTROLLERVIEW_H
#define GDBCONTROLLERVIEW_H

#include <QObject>
#include <QWidget>
#include <QPushButton>
#include <QFileInfo>
#include <QProcess>

typedef QPushButton GDBControllerButton;

/**
 * @brief The GDBControllerView class 调试控制类
 */
class BuildAndDebugTools : public QWidget
{
    Q_OBJECT
public:
    BuildAndDebugTools(QWidget *parent = nullptr);

signals:
    void massageSend(QString);

public slots:
    void onCurrentCodeFileChanged(const QFileInfo & fileInfo);


private:
    void onRunCode();
    void onProcessReadOutput();
    void onProcessReadError();

private:
    GDBControllerButton *m_asm;
    GDBControllerButton *m_src;
    GDBControllerButton *m_debug;
    GDBControllerButton *m_run;
    GDBControllerButton *m_stepOver;
    GDBControllerButton *m_stepInto;
    GDBControllerButton *m_stepOut;
    GDBControllerButton *m_stepTo;

    QFileInfo m_currentCodeFileInfo;


private:
    QProcess *m_process;
};



#endif // GDBCONTROLLERVIEW_H
