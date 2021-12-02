#ifndef BORROW_H
#define BORROW_H

#include <QDialog>

namespace Ui {
class Borrow;
}

class Borrow : public QDialog
{
    Q_OBJECT

public:
    explicit Borrow(QWidget *parent = nullptr);
    ~Borrow();

private:
    Ui::Borrow *ui;
};

#endif // BORROW_H
