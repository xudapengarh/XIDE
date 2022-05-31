#ifndef SOURCECODEEDITOR_H
#define SOURCECODEEDITOR_H

#include <QObject>
#include <QWidget>
#include <QTextEdit>
#include <QFileInfo>
#include <QPlainTextEdit>

#include "sourcecodelinearea.h"
#include "sourcecodeeditorarea.h"

class SourceCodeEditor : public QWidget
{
    Q_OBJECT
public:
    SourceCodeEditor(QWidget *parent = nullptr);

    void OpenFile(QFileInfo fileInfo);

    void SaveFile();

private:

    SourceCodeEditorArea *m_editorArea;
    SourceCodeLineArea *m_lineArea;
};

#endif // SOURCECODEEDITOR_H
