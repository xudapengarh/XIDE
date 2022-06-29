#ifndef BUILDANDDEBUGTOOLS_H
#define BUILDANDDEBUGTOOLS_H

#include <QObject>
#include <QWidget>

#include <QTextBrowser>
#include <QFileInfo>

#include "runtool.h"
#include "debugtool.h"

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

private:
    QWidget *m_tooBar;
    RunTool *m_runTool;
    DebugTool *m_debugTool;

    QTextBrowser *m_messageBrowser;
    QFileInfo m_targeFile;

    // QWidget interface
protected:
    void resizeEvent(QResizeEvent *event) override;
};

#endif // BUILDANDDEBUGTOOLS_H
