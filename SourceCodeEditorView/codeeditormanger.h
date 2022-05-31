#ifndef CODEEDITORMANGER_H
#define CODEEDITORMANGER_H

#include <QObject>
#include <QWidget>
#include <QTextEdit>
#include <QFileInfo>
#include <QTabWidget>
#include <QMap>

#include "codeeditor.h"

class CodeEditorManger : public QTabWidget
{
    Q_OBJECT
public:
    CodeEditorManger(QWidget *parent);

public slots:
    void onOpenFile(QFileInfo fileInfo);
    void onCloseFile(QFileInfo fileInfo);
    void onSaveFile();

private:

    QMap<QString, CodeEditor*> m_openedEditor;

};

#endif // CODEEDITORMANGER_H
