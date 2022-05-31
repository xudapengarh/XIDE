#ifndef CODEEDITORAREA_H
#define CODEEDITORAREA_H

#include <QObject>
#include <QWidget>

#include <QObject>
#include <QWidget>
#include <QTextEdit>
#include <QFileInfo>
#include <QPlainTextEdit>

#include <QDebug>

#include <QList>


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
    QList<int> blockHeight;

    EditorAreaSection validBlockNumberSection;
    EditorAreaSection focusBlockNumberSection;
}EditorAreaAttribute;

class CodeEditorArea : public QPlainTextEdit
{
    Q_OBJECT
public:
    CodeEditorArea(QWidget *parent = nullptr);

public slots:
    void OpenFile(QFileInfo fileInfo);  // 打开文件
    void SaveFile();                    // 保存已打开的文件

signals:
    void lineAreaUpdate(const EditorAreaAttribute &);   // 代码行号刷新信号

protected:
    void resizeEvent(QResizeEvent *event) override;

    void keyPressEvent(QKeyEvent* event) override;

private slots:
    void onUpdateRequest(const QRect &rect, int dy);    // 更新事件处理函数

private:
    QFileInfo m_currentFileInfo;    // 该编辑页面对应的文件
};


#endif // CODEEDITORAREA_H
