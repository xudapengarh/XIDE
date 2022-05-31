#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QObject>
#include <QWidget>
#include <QTextEdit>
#include <QFileInfo>
#include <QPlainTextEdit>

#include "codelinearea.h"
#include "codeeditorarea.h"

class CodeEditor : public QWidget
{
    Q_OBJECT
public:
    CodeEditor(QWidget *parent = nullptr);

    void OpenFile(QFileInfo fileInfo);

    void SaveFile();

private:

    CodeEditorArea *m_editorArea;
    CodeLineArea *m_lineArea;
};

#endif // CODEEDITOR_H
