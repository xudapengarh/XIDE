#ifndef CODELINEAREA_H
#define CODELINEAREA_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QVector>

#include <QVBoxLayout>


#include "codeeditorarea.h"

typedef QLabel LineItem;


class CodeLineArea : public QWidget
{
    Q_OBJECT
public:
    CodeLineArea(QWidget *parent = nullptr);

public slots:
    void onLineAreaUpdate(const EditorAreaAttribute &att);


protected:
    void resizeEvent(QResizeEvent *event) override;



private:
    void UpdateArea();


private:
    int m_letterWidth;
    EditorAreaAttribute m_att;
    QList<QLabel *> m_itemList;         // 保存 item 的list
};

#endif // CODELINEAREA_H
