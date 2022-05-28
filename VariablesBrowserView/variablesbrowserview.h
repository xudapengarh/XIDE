#ifndef VARIABLESBROWSERVIEW_H
#define VARIABLESBROWSERVIEW_H

#include <QObject>
#include <QWidget>
#include <QTextBrowser>

class VariablesBrowserView : public QTextBrowser
{
    Q_OBJECT
public:
    VariablesBrowserView(QWidget *parent = nullptr);
};

#endif // VARIABLESBROWSERVIEW_H
