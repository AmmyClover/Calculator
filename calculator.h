#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <ButtonWithText.h>
class Calculator : public QWidget
{
    Q_OBJECT

public:
    Calculator();
private slots:
    void digit_clicked();
    void point_clicked();
    void change_sign_clicked();
    void backspace_clicked();
    void clear_all();
    void clear();
    void clear_memory();
    void read_memory();
    void plus_to_memory();
    void min_to_memory();
    void binary_op_clicked();
    void unary_op_clicked();
    void equal_clicked();
private:
    double mSumInMemory;
    QString mPendingOp;
    QLineEdit* mDisplayUp;
    QLineEdit* mDisplayDown;
    QLabel* mSign;

    ButtonWithText* mDigitButtons[10];

    // signs:
    QString mSquareSign = QChar(0x0000221A);
    QString mPowerSign = 'x' + QChar(0x000000b2);
    QString mReciprocalSign = "1/x";
    QString mDivisionSign = QChar(0x000000F7);
    QString mTimesSign = QChar(0x000000D7);
    QString mPlusSign = "+";
    QString mMinusSign = "-";
    QString mChangeSign = QChar(0x000000b1);

    bool calculate(double operand, const QString& op);
    void abort_op();
    ButtonWithText* createButton(const QString& text, const char* member);
};

#endif // CALCULATOR_H
