#ifndef SOURCECODELINEAREA_H
#define SOURCECODELINEAREA_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QVector>

#include <QVBoxLayout>


#include "sourcecodeeditorarea.h"

typedef QLabel LineItem;


class SourceCodeLineArea : public QWidget
{
    Q_OBJECT
public:
    SourceCodeLineArea(QWidget *parent = nullptr);


public slots:
    void onLineAreaUpdate(const EditorAreaAttribute &att);


protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    void UpdateArea();


private:
    int m_topMargin;
    int m_itemHeight;
    int m_itemWidth;
    bool m_inited;
    EditorAreaAttribute m_att;
    QList<QLabel *> m_itemList;         // 保存 item 的list
};

#endif // SOURCECODELINEAREA_H
