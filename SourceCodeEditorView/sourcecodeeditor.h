#ifndef SOURCECODEEDITOR_H
#define SOURCECODEEDITOR_H

#include <QObject>
#include <QWidget>
#include <QTextEdit>
#include <QFileInfo>

class SourceCodeEditor : public QTextEdit
{
    Q_OBJECT
public:
    SourceCodeEditor(QWidget *parent);

public slots:
    void onOpenFile(QFileInfo fileInfo);
    void onSaveFile();

private:
    QFileInfo m_currentFileInfo;
};

#endif // SOURCECODEEDITOR_H
