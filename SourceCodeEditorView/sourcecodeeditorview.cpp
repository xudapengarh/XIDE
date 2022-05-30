#include <QKeyEvent>
#include <QDebug>
#include <QScrollArea>
#include "sourcecodeeditorview.h"
#include "sourcecodeeditorarea.h"

SourceCodeEditorView::SourceCodeEditorView(QWidget *parent) : QTabWidget(parent)
{
    this->resize(600, 500);
    this->setTabsClosable(true);
    this->setMovable(true);
    this->setUsesScrollButtons(true);
}

void SourceCodeEditorView::onOpenFile(QFileInfo fileInfo)
{

    if(this->m_openedEditor.contains(fileInfo.absoluteFilePath())){
        this->setCurrentWidget(this->m_openedEditor[fileInfo.absoluteFilePath()]);
    }
    else{
        SourceCodeEditorArea_bac *editor = new SourceCodeEditorArea_bac(this);
        this->m_openedEditor.insert(fileInfo.absoluteFilePath(), editor);
        this->insertTab(0, editor, fileInfo.fileName());
        editor->OpenFile(fileInfo);
        this->setCurrentWidget(editor);
    }

}

void SourceCodeEditorView::onCloseFile(QFileInfo fileInfo)
{
    if(this->m_openedEditor.contains(fileInfo.absoluteFilePath())){
        SourceCodeEditorArea_bac *editor = this->m_openedEditor[fileInfo.absoluteFilePath()];
        this->removeTab(this->indexOf(editor));
        delete editor;
    }
}

void SourceCodeEditorView::onSaveFile()
{

}






