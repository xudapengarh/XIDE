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

signals:
    void currentCodeFileChanged(const QFileInfo &);

public slots:
    void onOpenFile(QFileInfo fileInfo);
    void onCloseFile(QFileInfo fileInfo);
    void onUpdateFrame(const QString &file, const int &currentLine);

private:

    QMap<QString, CodeEditor*> m_openedEditor;
    QFileInfo m_currentCodeFile;
};

#endif // CODEEDITORMANGER_H
