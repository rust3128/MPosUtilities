#ifndef VIEWPROGRESSFORM_H
#define VIEWPROGRESSFORM_H

#include <QWidget>

namespace Ui {
class ViewProgressForm;
}

class ViewProgressForm : public QWidget
{
    Q_OBJECT

public:
    explicit ViewProgressForm(QWidget *parent = nullptr);
    ~ViewProgressForm();

private:
    Ui::ViewProgressForm *ui;
};

#endif // VIEWPROGRESSFORM_H
