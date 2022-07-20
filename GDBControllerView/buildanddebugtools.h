#ifndef BUILDANDDEBUGTOOLS_H
#define BUILDANDDEBUGTOOLS_H

#include <QObject>
#include <QWidget>

#include <QTextBrowser>
#include <QFileInfo>
#include <QTableWidget>

#include "runtool.h"
#include "debugtool.h"
#include "framevariablebrowser.h"

class BuildAndDebugTools : public QWidget
{
    Q_OBJECT
public:
    BuildAndDebugTools(QWidget *parent = nullptr);

signals:
    void targetChanged(QFileInfo);


public slots:
    void onCurrentCodeFileChanged(const QFileInfo &fileInfo);
    void onShowMessage(QString message);


signals:
    void updateFrame(const QString &file, const int &currentLine);
    void updateRegisters(const RegisterGroup &registers);

private slots:
    void onUpdateFrame(const FrameInfo &frame);
    void onUpdateRegisters(const RegisterGroup &registers);

private:
    QWidget *m_tooBar;
    RunTool *m_runTool;
    DebugTool *m_debugTool;

    QTextBrowser *m_messageBrowser;
    FrameVariableBrowser *m_frameVariableBrowser;
    QTableWidget *m_registerBrowser;
    QFileInfo m_targeFile;

    // QWidget interface
protected:
    void resizeEvent(QResizeEvent *event) override;
};

#endif // BUILDANDDEBUGTOOLS_H
