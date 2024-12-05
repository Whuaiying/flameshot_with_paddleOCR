#pragma once
#include <QLabel>
#include <QMouseEvent>
#include <QPainter>
#include <QEvent>
#include <QColor>
#include <QApplication>
#include <include/utility.h>
#include <qglobal.h>

class TickLabel : public QLabel   // 一个字符串的label
{
    Q_OBJECT

public:
    explicit TickLabel(QWidget *parent = nullptr);
    int height;
    // ~TickLabel(){}
    // 设置label的各种参数
    void set_label_all(void);
    
protected:
    // 重写鼠标进入事件，鼠标悬停在标签上时触发
    void enterEvent(QEvent *event) override;
    // 重写鼠标离开事件
    void leaveEvent(QEvent *event) override;
    // 重写绘制函数
    // void paintEvent(QPaintEvent *event) override;
    // 如果按下Ctrl+C，则复制选中的文本
    void keyPressEvent(QKeyEvent *event) override;
    


private:
    bool m_isHovered;  // 标记鼠标是否悬停
    bool m_isPressed;  // 标记鼠标是否按下
    // 复制选中文字
    void copySelectedText();
};




class OCR_Labels    // 存储多个TickLabel，放在截图最后保留的控件中
{
    
    public:
    
    int margin; // 外围阴影厚度
    double k;   // 文本框的膨胀系数 1.4
    double fk;  // 字体的膨胀系数
    explicit  OCR_Labels():margin(7), k(1.6),fk(0.95){}
    std::vector<std::vector<double>> GeometryVec; // n (0 0 0 0 0 0)
    // explicit OCR_Labels(){}
    void update(std::vector<PaddleOCR::OCRPredictResult> ocr_result);
    void show(void);
    void change_size(qreal m_currentStepScaleFactor, qreal m_scaleFactor);
    // std::vector<TickLabel*> labels;         // 存label 的指针
    std::vector<TickLabel*> labels;
    std::vector<PaddleOCR::OCRPredictResult> m_ocr_result;
};
