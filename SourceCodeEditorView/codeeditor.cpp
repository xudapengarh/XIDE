#include <QKeyEvent>
#include <QPlainTextEdit>
#include <QDebug>
#include <QScrollArea>
#include <QHBoxLayout>

#include "codeeditor.h"
#include "codeeditorarea.h"
#include "codelinearea.h"

CodeEditor::CodeEditor(QWidget *parent) : QWidget(parent)
{

    // 设置字体
    QFont font;
    font.setFamily("Courier");
    font.setStyleHint(QFont::Monospace);
    font.setFixedPitch(true);
    font.setPointSize(14);
    this->setFont(font);

    this->m_editorArea = new CodeEditorArea(this);
    this->m_lineArea = new CodeLineArea(this);
    this->m_editorArea->setFont(font);
    this->m_lineArea->setFont(font);



    connect(this->m_editorArea, &CodeEditorArea::lineAreaUpdate, this->m_lineArea, &CodeLineArea::onLineAreaUpdate);

    QGridLayout *glayout = new QGridLayout(this);
    glayout->addWidget(this->m_lineArea, 0, 0);
    glayout->addWidget(this->m_editorArea, 0, 1);
    glayout->setSpacing(0);
    glayout->setMargin(0);
}

void CodeEditor::OpenFile(QFileInfo fileInfo)
{
    this->m_editorArea->OpenFile(fileInfo);
}

void CodeEditor::SaveFile()
{

}
