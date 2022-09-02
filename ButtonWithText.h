#ifndef BUTTONWITHTEXT_H
#define BUTTONWITHTEXT_H

#include <QToolButton>

class ButtonWithText : public QToolButton
{
    Q_OBJECT
public:
    explicit ButtonWithText(const QString& text, QWidget *parent = nullptr);
    QSize sizeHint() const override;
};

#endif // BUTTONWITHTEXT_H
