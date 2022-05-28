#ifndef GDBCONTROLLERVIEW_H
#define GDBCONTROLLERVIEW_H

#include <QObject>
#include <QWidget>
#include <QPushButton>

typedef QPushButton GDBControllerButton;

/**
 * @brief The GDBControllerView class 调试控制类
 */
class GDBControllerView : public QWidget
{
    Q_OBJECT
public:
    GDBControllerView(QWidget *parent = nullptr);



private:
    GDBControllerButton *m_run;
    GDBControllerButton *m_startDebug;
    GDBControllerButton *m_nexti;
};

#endif // GDBCONTROLLERVIEW_H
