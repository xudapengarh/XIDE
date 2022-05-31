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
}

void CodeEditorManger::onOpenFile(QFileInfo fileInfo)
{

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

}

void CodeEditorManger::onCloseFile(QFileInfo fileInfo)
{
    if(this->m_openedEditor.contains(fileInfo.absoluteFilePath())){
        CodeEditor *editor = this->m_openedEditor[fileInfo.absoluteFilePath()];
        this->removeTab(this->indexOf(editor));
        delete editor;
    }
}

void CodeEditorManger::onSaveFile()
{

}






