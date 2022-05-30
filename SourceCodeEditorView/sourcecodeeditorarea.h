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


class EditorAreaSection{
public:
    EditorAreaSection(int begain = 0, int end = 0) : begain(begain) , end(end){}
    inline bool Contains(int value) const {return value >= begain && value <= end;}
    int begain;
    int end;
};

typedef struct {
    int totalBlockCount;
    int topMargin;
    int blockHeight;
    EditorAreaSection validBlockNumberSection;
    EditorAreaSection focusBlockNumberSection;
}EditorAreaAttribute;

class SourceCodeEditorArea : public QPlainTextEdit
{
    Q_OBJECT
public:
    SourceCodeEditorArea(QWidget *parent = nullptr);

public slots:
    void OpenFile(QFileInfo fileInfo);
    void SaveFile();

signals:
    void lineAreaUpdate(const EditorAreaAttribute &);

protected:
    void resizeEvent(QResizeEvent *event) override;



private slots:
    void onUpdateRequest(const QRect &rect, int dy);

private:
    QFileInfo m_currentFileInfo;

};


#endif // SOURCECODEEDITORAREA_H
