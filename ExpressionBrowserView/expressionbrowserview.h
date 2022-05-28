#ifndef EXPRESSIONBROWSERVIEW_H
#define EXPRESSIONBROWSERVIEW_H

#include <QObject>
#include <QWidget>
#include <QTextBrowser>

class ExpressionBrowserView : public QTextBrowser
{
    Q_OBJECT
public:
    ExpressionBrowserView(QWidget *parent = nullptr);
};

#endif // EXPRESSIONBROWSERVIEW_H
