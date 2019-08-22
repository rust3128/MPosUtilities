#ifndef ADDTERMINALSDIALOG_H
#define ADDTERMINALSDIALOG_H

#include <QDialog>
#include <QSqlQueryModel>
#include <QSortFilterProxyModel>

namespace Ui {
class AddTerminalsDialog;
}

class AddTerminalsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddTerminalsDialog(QSqlQueryModel *model, int selectType, QWidget *parent = nullptr);
    ~AddTerminalsDialog();
    QList<QModelIndex> getSelectedTerminals();

private slots:
    void on_comboBoxRegions_activated(int index);
    void on_tableViewTerminals_doubleClicked(const QModelIndex &idx);

    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    void createUI();
private:
    Ui::AddTerminalsDialog *ui;
    QSqlQueryModel *m_modelTerminals;
    QSortFilterProxyModel *m_proxyModel;
    int m_selectType;
    QList<QModelIndex> m_selectedTerminals;
};

#endif // ADDTERMINALSDIALOG_H
