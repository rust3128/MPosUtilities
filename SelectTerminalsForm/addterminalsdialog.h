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

    void on_pushButtonDeSelectAll_clicked();

    void on_pushButtonSelectAll_clicked();

private:
    void createUITerminal();
    void createUITerminalsRegion();
    void createUIRegion();
    void comboBoxActivatedTerminal(int idx);
    void comboBoxActivatedTerminalRegions(int idx);
    void comboBoxActivatedRegion(int idx);
    void appendTerminals(int idx);
private:
    Ui::AddTerminalsDialog *ui;
    QSqlQueryModel *m_modelTerminals;
    QSortFilterProxyModel *m_proxyModel;
    int m_selectType;
    QList<QModelIndex> m_selectedTerminals;

};

#endif // ADDTERMINALSDIALOG_H
