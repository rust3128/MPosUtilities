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
    void on_toolButtonSelectTermRegions_clicked();

    void on_toolButtonSelectRegion_clicked();

    void on_pushButtonSelectAll_clicked();

    void on_pushButtonDeSelectAll_clicked();

    void on_pushButtonDeleteSelected_clicked();

private:
    void createUI();
    void fillingTerminals(QList<QModelIndex> listIdx);
private:
    Ui::SelectTerminalsForm *ui;
    QSqlQueryModel *modTerminals;
};

#endif // SELECTTERMINALSFORM_H
