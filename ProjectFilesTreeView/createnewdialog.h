#ifndef CREATENEWDIALOG_H
#define CREATENEWDIALOG_H

#include <QObject>
#include <QWidget>
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>

class CreateNewDialog : public QDialog{
    Q_OBJECT
public:
    CreateNewDialog(QWidget *parent = nullptr);

    /**
     * @brief CreateDir 创建新文件夹
     * @param rootPath  [in]: 父目录
     */
    void CreateDir(QString rootPath = "");


    /**
     * @brief CreateFile 创建新文件
     * @param rootPath   [in]: 父目录
     */
    void CreateFile(QString rootPath = "");

    /**
     * @brief GetNewPath 获取新文件夹/文件路径
     * @return 新文件夹/文件路径
     */
    inline QString GetRelativePath(){return m_relativePath;}

    /**
     * @brief GetAbsolutePath 获取新文件夹/文件相对路径
     * @return 新文件夹/文件路径
     */
    inline QString GetAbsolutePath(){return this->m_absolutePath;}


private:
    QLabel *m_rootPath;             // 上级目录
    QLineEdit *m_newPath;           // 新建文件/文件夹名称
    QComboBox *m_fileNameTypeBox;   // 文件类型

    QPushButton *m_pathSelectButton;// 路径选择按钮
    QPushButton *m_okButton;        // 确定
    QPushButton *m_cancalButton;    // 取消

    QString m_relativePath;         // 相对路径
    QString m_absolutePath;         // 绝对路径
};


#endif // CREATENEWDIALOG_H
