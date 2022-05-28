#include <QKeyEvent>
#include <QDebug>
#include "sourcecodeeditorview.h"

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
        SourceCodeEditor *editor = new SourceCodeEditor(this);
        this->m_openedEditor.insert(fileInfo.absoluteFilePath(), editor);
        this->insertTab(0, editor, fileInfo.fileName());
        editor->onOpenFile(fileInfo);
        this->setCurrentWidget(editor);
    }

}

void SourceCodeEditorView::onCloseFile(QFileInfo fileInfo)
{
    if(this->m_openedEditor.contains(fileInfo.absoluteFilePath())){
        SourceCodeEditor *editor = this->m_openedEditor[fileInfo.absoluteFilePath()];
        this->removeTab(this->indexOf(editor));
        delete editor;
    }
}

void SourceCodeEditorView::onSaveFile()
{

}





