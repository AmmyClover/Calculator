#include <calculator.h>
#include <QGridLayout>
#include <QtMath>
#include <QDebug>
#include <ButtonWithText.h>

using namespace std;
Calculator::Calculator()
{
    mDisplayUp = new QLineEdit();
    mDisplayDown = new QLineEdit();
    mSign = new QLabel();
    QGridLayout* mainLayout = new QGridLayout;
    mainLayout->setSizeConstraint(QLayout::SetFixedSize);
    this->setStyleSheet("QWidget{background-color: #fff;}");
    mSign->setAlignment(Qt::AlignRight);
    mDisplayDown->setText("0");

    mDisplayUp->setReadOnly(true);
    mDisplayDown->setReadOnly(true);

    mDisplayUp->setAlignment(Qt::AlignRight);
    mDisplayDown->setAlignment(Qt::AlignRight);

    mDisplayUp->setMaxLength(16);
    mDisplayDown->setMaxLength(16);

    QFont font = mDisplayUp->font();
    font.setPointSize(font.pointSize()+8);
    mDisplayUp->setFont(font);
    mDisplayDown->setFont(font);
    mSign->setFont(font);


    for (int i = 0; i< 10; i++)
        mDigitButtons[i] = createButton(QString::number(i), SLOT(digit_clicked()));

    ButtonWithText *pointButton = createButton(".", SLOT(point_clicked()));
    ButtonWithText *changeSignButton = createButton(mChangeSign, SLOT(change_sign_clicked()));
    ButtonWithText *backspaceButton = createButton("Backspace", SLOT(backspace_clicked()));
    ButtonWithText *clearButton = createButton("Clear", SLOT(clear()));
    ButtonWithText *clearAllButton = createButton("Clear All", SLOT(clear_all()));
    ButtonWithText *clearMemoryButton = createButton("MC", SLOT(clear_memory()));
    ButtonWithText *readMemoryButton = createButton("MR", SLOT(read_memory()));
    ButtonWithText *minToMemoryButton = createButton("M+", SLOT(plus_to_memory()));
    ButtonWithText *plusToMemoryButton = createButton("M-", SLOT(min_to_memory()));
    ButtonWithText *divisionButton = createButton(mDivisionSign, SLOT(binary_op_clicked()));
    ButtonWithText *timesButton = createButton(mTimesSign, SLOT(binary_op_clicked()));
    ButtonWithText *minusButton = createButton(mMinusSign, SLOT(binary_op_clicked()));
    ButtonWithText *plusButton = createButton(mPlusSign, SLOT(binary_op_clicked()));
    ButtonWithText *squareButton = createButton(mSquareSign, SLOT(unary_op_clicked()));
    ButtonWithText *powerButton = createButton(mPowerSign, SLOT(unary_op_clicked()));
    ButtonWithText *reciprocalButton = createButton(mReciprocalSign, SLOT(unary_op_clicked()));
    ButtonWithText *equalButton = createButton("=", SLOT(equal_clicked()));

    mainLayout->addWidget(mDisplayUp, 0,0,1,4);
    mainLayout->addWidget(mSign, 1,2,1,1);
    mainLayout->addWidget(mDisplayDown, 2,0,1,4);
    mainLayout->addWidget(backspaceButton, 4,3,1,1);
    mainLayout->addWidget(clearButton, 4,2,1,1);
    mainLayout->addWidget(clearAllButton, 4,0,1,2);
    mainLayout->addWidget(clearMemoryButton, 3,0,1,1);
    mainLayout->addWidget(readMemoryButton, 3,1,1,1);
    mainLayout->addWidget(minToMemoryButton, 3,2,1,1);
    mainLayout->addWidget(plusToMemoryButton, 3,3,1,1);

    for (int i = 1; i < 10; ++i) {
        int row = ((9-i)/3)+7;
        int col = ((i-1)%3);
        mainLayout->addWidget(mDigitButtons[i], row, col);
    }

    mainLayout->addWidget(mDigitButtons[0], 10,1);
    mainLayout->addWidget(pointButton, 10,2);
    mainLayout->addWidget(changeSignButton, 10,0);
    mainLayout->addWidget(divisionButton, 6,3);
    mainLayout->addWidget(timesButton, 7,3);
    mainLayout->addWidget(minusButton, 8,3);
    mainLayout->addWidget(plusButton, 9,3);
    mainLayout->addWidget(squareButton, 6,2);
    mainLayout->addWidget(powerButton, 6,1);
    mainLayout->addWidget(reciprocalButton, 6,0);
    mainLayout->addWidget(equalButton, 10,3);

    setLayout(mainLayout);
    setWindowTitle("Calculator");
}

void Calculator::digit_clicked()
{

    ButtonWithText* btn = (ButtonWithText*) sender();
    int digit = btn->text().toUInt();
    qDebug() << "Digit" << digit;
    if(mDisplayDown->text()=="0")
        mDisplayDown->clear();
    mDisplayDown->setText(mDisplayDown->text()+QString::number(digit));
}

void Calculator::point_clicked()
{
    if(!mDisplayDown->text().contains('.'))
    {
        mDisplayDown->setText(mDisplayDown->text()+'.');
    }
}

void Calculator::change_sign_clicked()
{
    QString text = mDisplayDown->text();
    double value = text.toDouble();
    if (value > 0.0)
    {
        text.prepend('-');
    }else if (value< 0.0)
    {
        text.remove(0,1);
    }
    mDisplayDown->setText(text);
}

void Calculator::backspace_clicked()
{
    QString text = mDisplayDown->text();
    text.chop(1);
    if (text.isEmpty())
        text = "0";
    mDisplayDown->setText(text);
}

void Calculator::clear_all()
{
    mDisplayUp->clear();
    mSign->clear();
    mDisplayDown->setText("0");
}

void Calculator::clear()
{
    mDisplayDown->setText("0");
}

void Calculator::clear_memory()
{
    mSumInMemory = 0.0;
}

void Calculator::read_memory()
{

    mDisplayUp->clear();
    mSign->clear();
    mDisplayDown->setText(QString::number(mSumInMemory));
}

void Calculator::plus_to_memory()
{
    equal_clicked();
    mSumInMemory += mDisplayDown->text().toDouble();
}

void Calculator::min_to_memory()
{
    equal_clicked();
    mSumInMemory -= mDisplayDown->text().toDouble();
}

void Calculator::binary_op_clicked()
{
    ButtonWithText* btn = (ButtonWithText*) sender();
    QString op = btn->text();

    double operand = mDisplayDown->text().toDouble();

    if(mDisplayDown->text() == "0")
        return;

    mSign->setText(op);

    if (mDisplayDown->text()== "")
        return;

    mDisplayDown->clear();

    if(!mPendingOp.isEmpty())
    {
        if(!calculate(operand, mPendingOp))
        {
            abort_op();
            return;
        }
        mPendingOp.clear();
    }
    else
    {
        mDisplayUp->setText(QString::number(operand));
    }
    mPendingOp = op;
}

void Calculator::unary_op_clicked()
{
    ButtonWithText* btn = (ButtonWithText*) sender();
    QString op = btn->text();
    double operand = mDisplayDown->text().toDouble();
    double result = 0.0;
    if (op == mSquareSign)
    {
        if(operand < 0.0)
        {
            abort_op();
            return;
        }
        else
        {
        result = std::sqrt(operand);
        }
    }
    else if (op == mPowerSign)
    {
        result = std::pow(operand, 2.0);
    }
    else if (op == mReciprocalSign)
    {
        if (operand == 0.0)
        {
            abort_op();
            return;
        }
        result = 1.0/operand;
    }
    mDisplayDown->setText(QString::number(result));
}

void Calculator::equal_clicked()
{
    double operand = mDisplayDown->text().toDouble();
    if(!mPendingOp.isEmpty())
    {
        if(!calculate(operand, mPendingOp))
        {
            abort_op();
            return;
        }
        mPendingOp.clear();
    }
    mDisplayDown->setText(mDisplayUp->text());
    mDisplayUp->clear();
    mSign->clear();
}

bool Calculator::calculate(double operand, const QString &op)
{
    double tempTotal = mDisplayUp->text().toDouble();

    if (op == mPlusSign)
    {
        tempTotal += operand;
    } else if (op == mMinusSign)
    {
        tempTotal -= operand;
    }else if (op == mTimesSign)
    {
        tempTotal *= operand;
    }else if (op == mDivisionSign)
    {
        if (operand == 0.0)
            return false;
        tempTotal /= operand;
    }
    mDisplayUp->setText(QString::number(tempTotal));
    return true;
}

void Calculator::abort_op()
{
    mDisplayUp->setText("Error");
    mDisplayDown->setText("Error");
}


ButtonWithText* Calculator::createButton(const QString& text, const char* member)
{
    ButtonWithText* btn = new ButtonWithText(text);
    connect(btn, SIGNAL(clicked()), this, member);
    QFont font = btn->font();
    font.setPointSize(font.pointSize()+4);
    btn->setFont(font);
    btn->setStyleSheet("background-color: rgb(42, 32, 32);");
    return btn;
}
