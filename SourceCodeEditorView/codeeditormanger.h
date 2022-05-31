#ifndef SOURCECODEEDITORVIEW_H
#define SOURCECODEEDITORVIEW_H

#include <QObject>
#include <QWidget>
#include <QTextEdit>
#include <QFileInfo>
#include <QTabWidget>
#include <QMap>

#include "sourcecodeeditor.h"

class SourceCodeEditorView : public QTabWidget
{
    Q_OBJECT
public:
    SourceCodeEditorView(QWidget *parent);

public slots:
    void onOpenFile(QFileInfo fileInfo);
    void onCloseFile(QFileInfo fileInfo);
    void onSaveFile();

private:

    QMap<QString, SourceCodeEditor*> m_openedEditor;

};

#endif // SOURCECODEEDITORVIEW_H
