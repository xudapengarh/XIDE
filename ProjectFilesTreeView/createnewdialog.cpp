#include <QApplication>
#include <QFileDialog>
#include <QMessageBox>

#include <QVBoxLayout>
#include <QHBoxLayout>

#include "createnewdialog.h"



CreateNewDialog::CreateNewDialog(QWidget *parent) : QDialog(parent)
{
    QApplication::processEvents();

    this->m_relativePath = "";
    this->m_absolutePath = "";

    this->m_rootPath = new QLabel(this);
    this->m_newPath = new QLineEdit(this);

    // 新建文件类型
    this->m_fileNameTypeBox = new QComboBox(this);
    this->m_fileNameTypeBox->addItem(".h");
    this->m_fileNameTypeBox->addItem(".c");
    this->m_fileNameTypeBox->addItem(".cpp");
    this->m_fileNameTypeBox->addItem(".s");

    // 路径选择
    this->m_pathSelectButton = new QPushButton(this);
    this->m_pathSelectButton->setText("选择路径");
    this->m_pathSelectButton->setVisible(false);
    connect(this->m_pathSelectButton, &QPushButton::clicked, this, [this](){
        QString path = this->m_rootPath->text();
        path = QFileDialog::getExistingDirectory(this, "选择目录", path, QFileDialog::ShowDirsOnly);
        if(!path.isEmpty()){
            this->m_rootPath->setText(path);
        }
    });

    // 取消
    this->m_cancalButton = new QPushButton(this);
    this->m_cancalButton->setText("取消");
    connect(this->m_cancalButton, &QPushButton::clicked, this, [this](){
        this->m_newPath->setText("");
        this->m_rootPath->setText("");
        this->close();
    });

    // 确定
    this->m_okButton = new QPushButton(this);
    this->m_okButton->setText("确定");
    connect(this->m_okButton, &QPushButton::clicked, this, [this](){
        QString path = this->m_newPath->text();
        if (this->m_fileNameTypeBox->isVisible()){
            path += this->m_fileNameTypeBox->currentText();
            QFile file(this->m_rootPath->text() + path);
            if (file.exists()){
                QMessageBox::warning(this,tr("创建文件"),tr("文件已经存在！"));
            }
            else{
                file.open(QIODevice::WriteOnly);
                this->m_relativePath = path;
                this->m_absolutePath = this->m_rootPath->text() + path;
                file.close();
                this->close();
            }
        }
        else{
            QDir dir;
            if (dir.exists(this->m_rootPath->text() + path)){
                QMessageBox::warning(this,tr("创建文件夹"),tr("文件夹已经存在！"));
            }
            else{
                dir.mkdir(this->m_rootPath->text() + path);
                this->m_relativePath = path;
                this->m_absolutePath = this->m_rootPath->text() + path;
                this->close();
            }
        }
    });

    QVBoxLayout *vlayout = new QVBoxLayout(this);
    {
        QHBoxLayout *hlayout = new QHBoxLayout();
        QLabel *label = new QLabel(this);
        label->setText("名称 : ");
        hlayout->addWidget(label);
        hlayout->addWidget(this->m_newPath);
        hlayout->addWidget(this->m_fileNameTypeBox);
        vlayout->addLayout(hlayout);
    }

    {
        QHBoxLayout *hlayout = new QHBoxLayout();
        QLabel *label = new QLabel(this);
        label->setText("路径 : ");
        hlayout->addWidget(label);
        hlayout->addWidget(this->m_rootPath);
        hlayout->addWidget(this->m_pathSelectButton);
        vlayout->addLayout(hlayout);
    }

    {
        QHBoxLayout *hlayout = new QHBoxLayout();
        hlayout->addWidget(this->m_cancalButton);
        hlayout->addWidget(this->m_okButton);
        vlayout->addLayout(hlayout);
    }
}

void CreateNewDialog::CreateDir(QString rootPath)
{
    this->m_fileNameTypeBox->setVisible(false);
    if(rootPath.isEmpty()){
        this->m_pathSelectButton->setVisible(true);
        rootPath = QFileDialog::getExistingDirectory(this, "选择目录", "./", QFileDialog::ShowDirsOnly);
        if (rootPath.isEmpty()){
            return;
        }
    }
    else{
        this->m_pathSelectButton->setVisible(false);
    }

    this->m_rootPath->setText(rootPath + "/");
    this->exec();
}

void CreateNewDialog::CreateFile(QString rootPath)
{
    if(rootPath.isEmpty()){
        this->m_pathSelectButton->setVisible(true);
        rootPath = QFileDialog::getExistingDirectory(this, "选择目录", "./", QFileDialog::ShowDirsOnly);
        if (rootPath.isEmpty()){
            return;
        }
    }
    else{
        this->m_pathSelectButton->setVisible(false);
    }
    this->m_rootPath->setText(rootPath + "/");
    this->exec();
}
