#ifndef SOURCECODELINEAREA_H
#define SOURCECODELINEAREA_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QVector>

#include <QVBoxLayout>

class LineLabel : public QLabel{
    Q_OBJECT
public:
    LineLabel(QWidget *parent = nullptr);
    void SetSize(int width, int height);
};

class SourceCodeLineArea : public QWidget
{
    Q_OBJECT
public:
    SourceCodeLineArea(QWidget *parent = nullptr);


public:
    void SetLineHeight(int height);
    void AddLine();
    void RemoveLine();

private:
    QVBoxLayout *m_vlayout;

    QFont m_fount;

    int m_lineCount;
    int m_labelWidth;
    int m_labelHeight;

    QVector<LineLabel *> m_lineLabels;
};

#endif // SOURCECODELINEAREA_H
