#ifndef DEBUGER_H
#define DEBUGER_H

#include <QObject>
#include <QWidget>
#include <QProcess>
#include <QVariant>

typedef  struct{
    int currentLine;
    QString file;
}FrameInfo;

typedef struct{
    QVector<QString> registerNames;
    QVector<QByteArray> values;
}Register;

typedef struct{
    QVector<QString> groupName;
    QVector<Register> registerInfo;
}RegisterGroup;

typedef struct{
    QVector<QString> variableType;
    QVector<QString> variableName;
    QVector<QString> variableValue;
}FrameVariable;

typedef enum{
    SRC,    // 源代码
    ASM     // 汇编代码
}FrameType;

/**
 * @brief The Debuger class 调试器
 */
class Debuger : public QProcess
{
    Q_OBJECT


public:
    /**
     * @brief Debuger 初始化调试器
     * @param parent
     */
    Debuger(QObject *parent = nullptr);

    /**
     * @brief Debug         开始调试
     * @param processName   [in] : 可执行文件路径
     * @return
     */
    virtual bool Debug(QString processName) = 0;

    /**
     * @brief SetFrameType 设置 Frame 类型（源代码/汇编代码）
     * @param type         [in] : SRC ：源代码；ASM ： 汇编代码
     */
    virtual void SetFrameType(FrameType type) = 0;

    /**
     * @brief Run 运行
     */
    virtual void Run() = 0;

    /**
     * @brief Continue 继续执行
     */
    virtual void Continue() = 0;

    /**
     * @brief Next 下一步/跳过函数
     */
    virtual void Next() = 0;

    /**
     * @brief Step 进入函数
     */
    virtual void Step() = 0;

    /**
     * @brief StepOut 跳出函数
     */
    virtual void StepOut() = 0;

    /**
     * @brief Nexti 下一步指令
     */
    virtual void Nexti() = 0;

    /**
     * @brief AddBreakPoint 添加断点
     * @param breakpoint    [in] : 断点行号
     */
    virtual void AddBreakPoint(int breakpoint) = 0;

    /**
     * @brief AddBreakPoint 添加断点
     * @param breakPoint    [in] : 断点函数
     */
    virtual void AddBreakPoint(QString breakPoint) = 0;

signals:
    /**
     * @brief updateFrame   更新代码
     * @param frame         [out]: 代码信息
     */
    void updateFrame(const FrameInfo &frame);

    /**
     * @brief updateRegisters   更新寄存器
     * @param registers         [out]: 寄存器信息
     */
    void updateRegisters(const RegisterGroup &registers);

    /**
     * @brief updateFrameVariable   更新帧变量
     * @param variables             [out]: 帧变量信息
     */
    void updateFrameVariable(const FrameVariable &variables);

private slots:
    /**
     * @brief onProcessReadOutput   调试器进程标准输出响应函数
     */
    virtual void onProcessReadOutput() = 0;

    /**
     * @brief onProcessReadError    调试器进程错输出响应函数
     */
    virtual void onProcessReadError() = 0;


private:
    virtual void OutputFilter() = 0;

    /**
     * @brief AnalysisFrameInfo 代码信息解析函数
     * @param lines             [in] : 代码字符串列表
     * @param index             [in] : 代码区域开始下标
     * @return                  代码区域末尾下标
     */
    virtual int AnalysisFrameInfo(QStringList &lines, int index) = 0;

    /**
     * @brief AnalysisRegisterInfo  寄存器信息解析函数
     * @param lines                 [in] : 寄存器信息字符串列表
     * @param index                 [in] : 寄存器信息字符串起始下标
     * @return                      寄存器信息字符串结束下标
     */
    virtual int AnalysisRegisterInfo(QStringList &lines, int index) = 0;

    /**
     * @brief AnalysisFrameVariableInfo 帧变量信息解析函数
     * @param lines                     [in] : 帧变量信息字符串起列表
     * @param index                     [in] : 帧变量字符串起始下标
     * @return                          帧变量信息字符串结束下标
     */
    virtual int AnalysisFrameVariableInfo(QStringList &lines, int index) = 0;
};

#endif // DEBUGER_H
