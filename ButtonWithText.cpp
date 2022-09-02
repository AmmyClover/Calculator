#include <ButtonWithText.h>

ButtonWithText::ButtonWithText(const QString &text, QWidget *parent)
    : QToolButton(parent)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    setText(text);
}

QSize ButtonWithText::sizeHint() const
{
    QSize size = QToolButton::sizeHint();
    size.rheight()+=30;
    size.rwidth() = qMax(size.width(), size.height());
    return size;
}
