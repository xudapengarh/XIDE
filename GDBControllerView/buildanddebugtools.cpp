#include <QGridLayout>

#include "buildanddebugtools.h"

BuildAndDebugTools::BuildAndDebugTools(QWidget *parent) : QWidget(parent)
{
    this->m_tooBar = new QWidget(this);
    this->m_tooBar->setMinimumHeight(30);
    this->m_tooBar->setMaximumHeight(30);
    this->m_runTool = new RunTool(this->m_tooBar);
    this->m_debugTool = new DebugTool(this->m_tooBar);
    {
        QHBoxLayout *hlayout = new QHBoxLayout(this->m_tooBar);
        hlayout->addWidget(this->m_runTool);
        hlayout->addWidget(this->m_debugTool);
        hlayout->addStretch();
        hlayout->setMargin(0);
        hlayout->setSpacing(0);
    }

    this->m_messageBrowser = new QTextBrowser(this);
    this->m_frameVariableBrowser = new FrameVariableBrowser(this);

    QGridLayout *glayout = new QGridLayout(this);
    glayout->addWidget(this->m_tooBar, 0, 0, 1, 2);
    glayout->addWidget(this->m_messageBrowser, 1, 0, 1, 1);
    glayout->addWidget(this->m_frameVariableBrowser, 1, 2, 1, 1);
    glayout->setSpacing(10);
    glayout->setMargin(0);

    this->setMinimumHeight(40);
    this->m_messageBrowser->setMinimumHeight(0);

    this->m_targeFile.setFile("");

    connect(this->m_runTool, &RunTool::programOutputSend, this, &BuildAndDebugTools::onShowMessage);
    connect(this->m_debugTool, &DebugTool::programOutputSend, this, &BuildAndDebugTools::onShowMessage);
    connect(this->m_debugTool, &DebugTool::updateFrame, this, &BuildAndDebugTools::onUpdateFrame);
    connect(this->m_debugTool, &DebugTool::updateRegisters, this, &BuildAndDebugTools::onUpdateRegisters);
    connect(this->m_debugTool, &DebugTool::updateFrameVariable, this->m_frameVariableBrowser, &FrameVariableBrowser::onUpdateFrameVariable);
}

void BuildAndDebugTools::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    if (this->height() < 50){
        this->resize(this->width(), 40);
        this->m_messageBrowser->setVisible(false);
    }
    else{
        this->m_messageBrowser->setVisible(true);
    }
}

void BuildAndDebugTools::onCurrentCodeFileChanged(const QFileInfo &fileInfo){
    this->m_targeFile = fileInfo;
    this->m_runTool->SetTargetProject(this->m_targeFile);
    this->m_debugTool->SetTargetProject(this->m_targeFile);
}

void BuildAndDebugTools::onShowMessage(QString message)
{
    this->m_messageBrowser->append(message);
}

void BuildAndDebugTools::onUpdateFrame(const FrameInfo &frame)
{
    emit updateFrame(frame.file, frame.currentLine);
}

void BuildAndDebugTools::onUpdateRegisters(const RegisterGroup &registers)
{
    emit updateRegisters(registers);
}
