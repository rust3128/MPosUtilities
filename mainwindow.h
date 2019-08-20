#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlQueryModel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void createModels();
private:

private:
    Ui::MainWindow *ui;
    QSqlQueryModel *modelTerminals;

    // QWidget interface
protected:
    void closeEvent(QCloseEvent *event);

};

#endif // MAINWINDOW_H
