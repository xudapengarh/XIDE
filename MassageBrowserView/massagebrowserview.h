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
};

#endif // MASSAGEBROWSERVIEW_H
