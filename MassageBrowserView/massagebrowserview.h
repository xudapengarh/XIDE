#ifndef MASSAGEBROWSERVIEW_H
#define MASSAGEBROWSERVIEW_H

#include <QObject>
#include <QWidget>
#include <QTextBrowser>
class MassageBrowserView : public QTextBrowser
{
    Q_OBJECT
public:
    MassageBrowserView(QWidget *parent = nullptr);

public slots:
    void onShowMassage(QString message);
};

#endif // MASSAGEBROWSERVIEW_H
