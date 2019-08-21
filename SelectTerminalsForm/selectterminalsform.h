#ifndef SELECTTERMINALSFORM_H
#define SELECTTERMINALSFORM_H

#include <QWidget>
#include <QSqlQueryModel>

namespace Ui {
class SelectTerminalsForm;
}

class SelectTerminalsForm : public QWidget
{
    Q_OBJECT

public:
    explicit SelectTerminalsForm(QWidget *parent = nullptr);
    ~SelectTerminalsForm();

public slots:
    void slotGetModel(QSqlQueryModel* mod);
private slots:
    void on_toolButtonSelectTerminal_clicked();

private:
    Ui::SelectTerminalsForm *ui;
    QSqlQueryModel *modTerminals;
};

#endif // SELECTTERMINALSFORM_H
