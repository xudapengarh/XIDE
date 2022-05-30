#ifndef SOURCECODEEDITORAREA_H
#define SOURCECODEEDITORAREA_H

#include <QObject>
#include <QWidget>

#include <QObject>
#include <QWidget>
#include <QTextEdit>
#include <QFileInfo>
#include <QPlainTextEdit>

#include <QDebug>

#include "sourcecodelinearea.h"

class SourceCodeEditorArea_bac : public QPlainTextEdit
{
    Q_OBJECT
public:
    SourceCodeEditorArea_bac(QWidget *parent);

public slots:
    void OpenFile(QFileInfo fileInfo);
    void SaveFile();


    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void updateLineNumberArea(const QRect &rect, int dy);



private:
    QFileInfo m_currentFileInfo;
    QWidget *m_lineArea;
};




class LineNumberArea : public QWidget{
    Q_OBJECT

public:
    LineNumberArea(SourceCodeEditorArea_bac *editor) : QWidget(editor) {
        this->m_editorArea = editor;
    }

    QSize sizeHint() const override {
        return QSize(this->m_editorArea->lineNumberAreaWidth(), 0);
    }

protected:
    void paintEvent(QPaintEvent *event) override {
        qDebug() << __FUNCTION__ << " height = " << this->height();
        this->m_editorArea->lineNumberAreaPaintEvent(event);
    }

private:
    SourceCodeEditorArea_bac *m_editorArea;
};

#endif // SOURCECODEEDITORAREA_H
