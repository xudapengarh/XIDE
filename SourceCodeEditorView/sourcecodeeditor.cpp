#include <QKeyEvent>
#include <QPlainTextEdit>
#include <QDebug>
#include <QScrollArea>
#include <QHBoxLayout>

#include "sourcecodeeditor.h"

SourceCodeEditor::SourceCodeEditor(QWidget *parent) : QWidget(parent)
{
    this->m_editerArea = new SourceCodeEditorArea_bac(this);
    this->m_lineArea = new SourceCodeLineArea(this);

    QHBoxLayout *hlayout = new QHBoxLayout(this);
    hlayout->addWidget(this->m_lineArea);
    hlayout->addWidget(this->m_editerArea);
    hlayout->setSpacing(0);
}

void SourceCodeEditor::OpenFile(QFileInfo fileInfo)
{
    this->m_editerArea->OpenFile(fileInfo);
    int lines = this->m_editerArea->document()->lineCount();
    for (int i = 0; i < lines; i++){
        this->m_lineArea->AddLine();
    }
}

void SourceCodeEditor::SaveFile()
{

}
