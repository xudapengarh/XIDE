#include <QKeyEvent>
#include <QDebug>

#include "sourcecodeeditor.h"

SourceCodeEditor::SourceCodeEditor(QWidget *parent)
{

}

void SourceCodeEditor::onOpenFile(QFileInfo fileInfo)
{
    this->m_currentFileInfo = fileInfo;
    QFile file;
    file.setFileName(fileInfo.absoluteFilePath());
    if(file.open(QIODevice::ReadOnly))
    {
        //读文件
        QByteArray array =  file.readAll();
        //将数据写进文本框中
        this->setPlainText(QString(array));
        //        //一行一行的读
        //        QByteArray array;
        //        while(file.atEnd() == false)
        //        {
        //            array += file.readLine();
        //        }
        //        this->setPlainText(QString(array).toUtf8().data());

    }
    file.close();
}

void SourceCodeEditor::onSaveFile()
{
    QFile file;
    file.setFileName(this->m_currentFileInfo.absoluteFilePath());
    if(file.open(QIODevice::WriteOnly))
    {
        file.write(this->toPlainText().toUtf8());
    }
    file.close();
}
