

#include <QLabel>
#include <QMouseEvent>
#include <QPainter>
#include <QEvent>
#include <QColor>
#include <QApplication>
#include <include/utility.h>
#include <src/labels_recognize/re_labels.hpp>
#include <QClipboard>
#include <QKeyEvent>


TickLabel::TickLabel(QWidget *parent)
    :QLabel(parent), m_isHovered(false), m_isPressed(false)
{
    
    setAttribute(Qt::WA_Hover);  // 启用鼠标悬停事件
    setTextInteractionFlags(Qt::TextSelectableByMouse);
    // 文字位置：上下居中，靠上
    // this->setAlignment(Qt::AlignVCenter);   // 居中
    // this->setAlignment(Qt::AlignTop);       // 靠上
    // this->setAlignment(Qt::AlignJustify | Qt::AlignTop); // 两端对齐 靠上
    this->setAlignment(Qt::AlignJustify | Qt::AlignVCenter); // 两端对齐 居中
    // this->setStyleSheet("QLabel { text-align: justify; }");
    
    // this->setMargin(0);                     // 设置边距
    this->setContentsMargins(0, 0, 0, 0);  // 去掉上下左右的边距

    // this->setStyleSheet("QLabel { padding-top: 0px; }");  // 去掉额外的内边距，调整样式
    //设置文本框的外观，包括字体的大小和颜色、按钮的背景色
    this->setFrameShape (QFrame::Box);
    // this->setStyleSheet("border-width: 1px;border-style: solid;border-color: rgb(255, 170, 0);");
    this->setStyleSheet("QLabel{color:transparent;\
                        background-color:transparent;}");
}
void TickLabel::set_label_all(void)
{
    this->setAlignment(Qt::AlignJustify | Qt::AlignTop); // 两端对齐 靠上
           // 靠上
    // this->setMargin(0);                     // 设置边距
    // this->setContentsMargins(0, 0, 0, 0);  // 去掉上下左右的边距
    // this->setStyleSheet("QLabel { padding-top: 0px; }");  // 去掉额外的内边距，调整样式
    //设置文本框的外观，包括字体的大小和颜色、按钮的背景色
    

}
// TickLabel::TickLabel()
// : QLabel(nullptr), m_isHovered(false), m_isPressed(false)
// {
//         setAttribute(Qt::WA_Hover);  // 启用鼠标悬停事件
//         setTextInteractionFlags(Qt::TextSelectableByMouse);
//         //设置文本框的外观，包括字体的大小和颜色、按钮的背景色
//         setStyleSheet("QLabel{font:30px;\
//                         color:blue;\
//                         background-color:transparent;}");
// }

// 重写鼠标进入事件，鼠标悬停在标签上时触发
void TickLabel::enterEvent(QEvent *event)
{
    m_isHovered = true;
    setCursor(Qt::IBeamCursor); //设置鼠标样式
    update();  // 更新控件（触发绘制）
    QLabel::enterEvent(event);
}

// 重写鼠标离开事件
void TickLabel::leaveEvent(QEvent *event)
{
    m_isHovered = false;
    setCursor(Qt::ArrowCursor); //设置鼠标样式
    update();
    QLabel::leaveEvent(event);
}
void TickLabel::keyPressEvent(QKeyEvent *event)
{
    if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_C) {
        // 如果按下Ctrl+C，则复制选中的文本
        copySelectedText();
    }
}

void TickLabel::copySelectedText()
{
    // 获取选中的文本
    QString selectedText = this->selectedText();
    
    if (!selectedText.isEmpty()) {
        // 将选中的文本复制到剪贴板
        QApplication::clipboard()->setText(selectedText);
    }
}
// void TickLabel::paintEvent(QPaintEvent *event){
//         QPainter painter(this);
//         painter.setRenderHint(QPainter::Antialiasing);
//         painter.setFont(this->font());
//         painter.setPen(this->palette().color(QPalette::WindowText));
        
//         // 设置文字从顶部开始绘制
//         QRect rect = this->rect();
//         painter.drawText(rect.adjusted(0, 0, 0, -rect.height() + fontMetrics().height()), Qt::AlignTop | Qt::AlignLeft, text());
//     }



// OCR_Labels::OCR_Labels():margin(7)
// {
    
// }

void OCR_Labels::update(std::vector<PaddleOCR::OCRPredictResult> ocr_result)
{
    m_ocr_result = ocr_result;
    std::vector<std::vector<int>> box;   // bounding box [[0,1],[2,3]]  // 这是一个框
    std::string str_re;                 //字符结果
    for(int i=0;i<ocr_result.size();i++)
    {
        TickLabel *label =new TickLabel();
        box = ocr_result[i].box;   // bounding box [[0,1],[2,3]]
        //          1 point         2 point
        //          3 point         4 point
        str_re = ocr_result[i].text;     // 文本结果
        int x = box[0][0];
        int y = box[0][1];
        int width = box[1][0]-box[0][0];
        int height = box[3][1]-box[0][1];
        label->height = height;
        // 设置位置
        // label->setGeometry(input_x, input_y, input_width, input_height);
        double input_height = k * height;
        double input_y = y - 0.5*(k-1) * height;
        label->setGeometry(x+margin, input_y+margin, width, input_height);
        std::cout<<x+margin<<","<<y+margin<<","<<width<<","<<height<<std::endl;
        // label->setFixedWidth(width);
        // double m_x=input_x;
        // double m_y=input_y;
        // double m_w=input_width;
        // double m_h=input_height;
        double m_x=x;
        double m_y=y;
        double m_w=width;
        double m_h=height;
        std::vector<double> SingleGeometry = {m_x,m_y,m_w,m_h}; // 保存double类型的变量
        GeometryVec.push_back(SingleGeometry);

        QString str_q1 = QString::fromStdString(str_re);
        label->setText(str_q1);          // 填入文字
        std::stringstream ss;
	    ss << "QLabel{font:"<<round(fk*height)<< "px;color:transparent;}";
        std::cout<<"******************************"<<ss.str()<<std::endl;
        QString str_q2 = QString::fromStdString(ss.str());
        label->setStyleSheet(str_q2); // 设置字体大小，颜色
        // label->setAlignment(Qt::AlignCenter);
        // label->set_label_all();
        labels.push_back(label);
    }
}
// 打印ocr_result 的结果
void OCR_Labels::show(void){    

    std::vector<std::vector<int>> box;   // bounding box [[0,1],[2,3]]  // 这是一个框
    std::string str_re;                 //字符结果
    for (int i=0;i<m_ocr_result.size();i++)
    {
        // std::cout<<result0[i]<<std::endl;
        box = m_ocr_result[i].box;   // bounding box [[0,1],[2,3]]
        if (box.size() > 0) {
        std::cout << "det boxes: [";
        for (int n = 0; n < box.size(); n++) {
            std::cout << '[' << box[n][0] << ',' << box[n][1] << "]";
            if (n != box.size() - 1) {
            std::cout << ',';
            }
        }
        std::cout <<"]";
        }
        if (m_ocr_result[i].score != -1.0) {
        str_re = m_ocr_result[i].text;     // 文本结果
        std::cout << "rec text: " << str_re
                    << " rec score: " << m_ocr_result[i].score << " ";
        }
        std::cout << std::endl;
    }

}
void OCR_Labels::change_size(qreal m_currentStepScaleFactor, qreal m_scaleFactor){  //m_currentStepScaleFactor 是相对原始图片的大小
    for (int i = 0; i < labels.size(); i++)
    {
        // QRect frameRect = labels[i]->frameGeometry();
        
        // double;
        double m_x = GeometryVec[i][0] * m_currentStepScaleFactor * m_scaleFactor;
        double m_y = GeometryVec[i][1] * m_currentStepScaleFactor * m_scaleFactor;
        double m_w = GeometryVec[i][2] * m_currentStepScaleFactor * m_scaleFactor;
        double m_h = GeometryVec[i][3] * m_currentStepScaleFactor * m_scaleFactor;
        double input_height = k * m_h;
        double input_y = m_y - 0.5*(k-1) * m_h;

        labels[i]->setGeometry(m_x+margin, input_y+margin, m_w,input_height);

        std::cout<<"["<<m_currentStepScaleFactor<<" "<<m_scaleFactor<<"]"<<std::endl;
        std::cout<<"["<<m_x<<" "<<m_y<<" "<<m_w<<" "<<m_h<<"]"<<std::endl;
        std::stringstream ss;
	    ss << "QLabel{font:"<<int(round(fk*m_h))<< "px;color:transparent;}";
        std::cout<<"******************************"<<ss.str()<<std::endl;
        QString str_q2 = QString::fromStdString(ss.str());
        labels[i]->setStyleSheet(str_q2); // 设置字体大小，颜色
        labels[i]->show();
    }
    
}