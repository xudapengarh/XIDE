#include <QKeyEvent>
#include <QDebug>
#include <QScrollArea>
#include "codeeditormanger.h"
#include "codeeditorarea.h"

CodeEditorManger::CodeEditorManger(QWidget *parent) : QTabWidget(parent)
{
    this->resize(600, 500);
    this->setTabsClosable(true);
    this->setMovable(true);
    this->setUsesScrollButtons(true);

    // 标签切换时，通知外部当前代码文件信息
    connect(this, &CodeEditorManger::currentChanged, this, [=](int index){
        CodeEditor *editor = (CodeEditor *)this->currentWidget();
        emit currentCodeFileChanged(editor->GetCodeFileInfo());
    });
}

void CodeEditorManger::onOpenFile(QFileInfo fileInfo)
{
    this->m_currentCodeFile = fileInfo;
    if(this->m_openedEditor.contains(fileInfo.absoluteFilePath())){
        this->setCurrentWidget(this->m_openedEditor[fileInfo.absoluteFilePath()]);
    }
    else{
        CodeEditor *editor = new CodeEditor(this);
        this->m_openedEditor.insert(fileInfo.absoluteFilePath(), editor);
        this->insertTab(0, editor, fileInfo.fileName());
        editor->OpenFile(fileInfo);
        this->setCurrentWidget(editor);
    }
    emit currentCodeFileChanged(this->m_currentCodeFile);
}

void CodeEditorManger::onCloseFile(QFileInfo fileInfo)
{
    if(this->m_openedEditor.contains(fileInfo.absoluteFilePath())){
        CodeEditor *editor = this->m_openedEditor[fileInfo.absoluteFilePath()];
        this->removeTab(this->indexOf(editor));
        delete editor;
    }
}






