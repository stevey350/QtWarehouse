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

    enum DateTimeType{Time, Date, DateTime};
    QString getDateTime(DateTimeType type);

    void contents_create();

    void query_show(QSqlQuery &query);

private slots:
    void slot_content_changed(int row);

    void on_btn_add_clicked();

    void on_btn_delete_clicked();

    void on_btn_sort_type_clicked();

    void on_btn_sort_state_clicked();

    void on_btn_expire_hint_clicked();

    void on_btn_search_clicked();

    void on_btn_overview_clicked();

    void on_btn_borrow_clicked();

    void on_btn_return_clicked();

    void on_tableWidget_cellDoubleClicked(int row, int column);

    void on_btn_search_state_clicked();

    void on_btn_damage_clicked();

private:
    Ui::MainWindow *ui;

    QSqlQueryModel *query_model_add;
    QSqlQueryModel *query_model_state;
};

#endif // MAINWINDOW_H
