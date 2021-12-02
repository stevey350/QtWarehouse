#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QListWidgetItem>
#include <QDateTime>
#include <QDebug>
#include <QSqlError>
#include <QMessageBox>
#include <QSqlQuery>
#include <QDate>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    contents_create();

    // 物品增减
    query_model_add = new QSqlQueryModel(this);
    query_model_add->setQuery("select id, name, type, state, position, price, supplier, owner, date_enter, date_expiration from goods");
    query_model_add->setHeaderData(0, Qt::Horizontal, tr("ID"));
    query_model_add->setHeaderData(1, Qt::Horizontal, tr("名称"));
    query_model_add->setHeaderData(2, Qt::Horizontal, tr("种类"));
    query_model_add->setHeaderData(3, Qt::Horizontal, tr("状态"));
    query_model_add->setHeaderData(4, Qt::Horizontal, tr("位置"));
    query_model_add->setHeaderData(5, Qt::Horizontal, tr("价格"));
    query_model_add->setHeaderData(6, Qt::Horizontal, tr("供应商"));
    query_model_add->setHeaderData(7, Qt::Horizontal, tr("拥有者"));
    query_model_add->setHeaderData(8, Qt::Horizontal, tr("入库日期"));
    query_model_add->setHeaderData(9, Qt::Horizontal, tr("有效日期"));
//    query_model_add->setHeaderData(10, Qt::Horizontal, tr("变更日期"));
//    query_model_add->setHeaderData(11, Qt::Horizontal, tr("备注"));

    ui->tableView_add->setModel(query_model_add);
    ui->tableView_add->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // 物品总览
    QStringList labels;
    labels << "物品ID" << "名称" << "种类" << "状态" << "所在位置";
    ui->tableWidget->setColumnCount(5);
    ui->tableWidget->setHorizontalHeaderLabels(labels);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // 物品状态更改
    query_model_state = new QSqlQueryModel(this);
//    query_model_state->setQuery("select * from goods");
//    query_model_state->setHeaderData(0, Qt::Horizontal, tr("ID"));
//    query_model_state->setHeaderData(1, Qt::Horizontal, tr("名称"));
//    query_model_state->setHeaderData(2, Qt::Horizontal, tr("种类"));
//    query_model_state->setHeaderData(3, Qt::Horizontal, tr("状态"));
//    query_model_state->setHeaderData(4, Qt::Horizontal, tr("位置"));
//    query_model_state->setHeaderData(5, Qt::Horizontal, tr("价格"));
//    query_model_state->setHeaderData(6, Qt::Horizontal, tr("供应商"));
//    query_model_state->setHeaderData(7, Qt::Horizontal, tr("拥有者"));
//    query_model_state->setHeaderData(8, Qt::Horizontal, tr("入库日期"));
//    query_model_state->setHeaderData(9, Qt::Horizontal, tr("有效日期"));
//    query_model_state->setHeaderData(10, Qt::Horizontal, tr("备注"));

    ui->tableView_state->setModel(query_model_state);
    ui->tableView_state->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// 左侧菜单栏生成
void MainWindow::contents_create()
{
    QFont font;

    //创建QListWidget
//    this->ui->lwt_contents = new QListWidget(this);
    this->ui->lwt_contents->setViewMode(QListView::IconMode);
    this->ui->lwt_contents->setIconSize(QSize(90, 60));
    this->ui->lwt_contents->setMovement(QListView::Static);
    this->ui->lwt_contents->setMaximumWidth(110);      //75
    this->ui->lwt_contents->setSpacing(5);
    connect(this->ui->lwt_contents, SIGNAL(currentRowChanged(int)), this, SLOT(slot_content_changed(int)));

    //创建QListWidgetItem子项
    font.setPointSize(12);                                  //设置字体大小
    QListWidgetItem *lwti_add = new QListWidgetItem(this->ui->lwt_contents);
    lwti_add->setIcon(QIcon(tr(":/images/debug.jpg")));
    lwti_add->setFont(font);                       //设置字体
    lwti_add->setText(tr("物品增加"));
    lwti_add->setTextAlignment(Qt::AlignCenter);
    lwti_add->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);

    QListWidgetItem *lwti_overview = new QListWidgetItem(this->ui->lwt_contents);
    lwti_overview->setIcon(QIcon(tr(":/images/setting.png")));
    lwti_overview->setFont(font);
    lwti_overview->setText(tr("物品总览"));
    lwti_overview->setTextAlignment(Qt::AlignCenter);
    lwti_overview->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);

    QListWidgetItem *lwti_state = new QListWidgetItem(this->ui->lwt_contents);
    lwti_state->setIcon(QIcon(tr(":/images/firewareUpdate.jpg")));
    lwti_state->setFont(font);
    lwti_state->setText(tr("物品状态"));
    lwti_state->setTextAlignment(Qt::AlignCenter);
    lwti_state->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);

    QListWidgetItem *lwti_illustration = new QListWidgetItem(this->ui->lwt_contents);
    lwti_illustration->setIcon(QIcon(tr(":/images/update.png")));
    lwti_illustration->setFont(font);
    lwti_illustration->setText(tr("软件简介"));
    lwti_illustration->setTextAlignment(Qt::AlignCenter);
    lwti_illustration->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
}

// 界面切换
void MainWindow::slot_content_changed(int row)
{
    if(row > this->ui->stackedWidget->count())
    {
        return;
    }
    this->ui->stackedWidget->setCurrentIndex(row);

    // 将生产日期设置为当前时间
    if(row == 0) {
        ui->ldt_date_enter->setText(getDateTime(Date));
    //    ui->ldt_date_expiration->setText(getDateTime(Date));
    }
    else if(row == 1) {
        // "物品ID" << "名称" << "种类" << "状态" << "所在位置"
        QSqlQuery query;
        query.exec(QString("select id, name, type, state, position from goods"));
        query_show(query);
    }
}

// 获取当前的日期或者时间
QString MainWindow::getDateTime(MainWindow::DateTimeType type)
{
    QDateTime datetime = QDateTime::currentDateTime();
    QString date = datetime.toString("yyyy-MM-dd");
    QString time = datetime.toString("hh:mm:ss");
    QString dateAndTime = datetime.toString("yyyy-MM-dd dddd hh:mm");
    if(type == Date)
        return date;
    else if(type == Time)
        return time;
    else
        return dateAndTime;
}

void MainWindow::on_btn_add_clicked()
{
    QString id, name, type, state, position, supplier, owner, date_enter, date_expiration;
    double price;
    int index1, index2;

    id = ui->ldt_id->text();
    name = ui->ldt_name->text();
    index1 = ui->cmb_type->currentIndex();
    if(index1 == 0) {
        type = tr("工具");
    }
    else if(index1 == 1) {
        type = tr("仪器");
    }
    else if(index1 == 2) {
        type = tr("消耗品");
    }
    index2 = ui->cmb_state->currentIndex();
    if(index2 == 0) {
        state = tr("库房");
    }
    else if(index2 == 1) {
        state = tr("借出");
    }
    else if(index2 == 2) {
        state = tr("损坏");
    }
    position = ui->ldt_position->text();
    price = ui->ldt_price->text().toDouble();
    supplier = ui->ldt_supplier->text();
    owner = ui->ldt_owner->text();
    date_enter = ui->ldt_date_enter->text();
    date_expiration = ui->ldt_date_expiration->text();

    if(index1 <= 1) {
        query_model_add->setQuery(QString("insert into goods values(%1, '%2', '%3', '%4', '%5', %6, '%7', '%8', '%9', default, default, default)")\
                                  .arg(id).arg(name).arg(type).arg(state).arg(position).arg(price).arg(supplier).arg(owner).arg(date_enter));
    }
    else {
        query_model_add->setQuery(QString("insert into goods values(%1, '%2', '%3', '%4', '%5', %6, '%7', '%8', '%9', '%10', default, default)")\
                                  .arg(id).arg(name).arg(type).arg(state).arg(position).arg(price).arg(supplier).arg(owner).arg(date_enter).arg(date_expiration));
    }

    query_model_add->setQuery("select id, name, type, state, position, price, supplier, owner, date_enter, date_expiration from goods");
    if (query_model_add->lastError().isValid())      // 执行SQL语句有问题时，将错误输出 (调试用)
        qDebug() << query_model_add->lastError();
}

void MainWindow::on_btn_delete_clicked()
{
    QString id;

    id = ui->ldt_id->text();
    if(id.isNull()) {
        QMessageBox::critical(this, tr("错误"), tr("请输入待删除记录对应的ID"), QMessageBox::Ok);
    }

    query_model_add->setQuery(QString("delete from goods where id = %1").arg(id));
    query_model_add->setQuery("select id, name, type, state, position, price, supplier, owner, date_enter, date_expiration from goods");
}

// 将查询的结果显示在物品总览的界面上
void MainWindow::query_show(QSqlQuery &query)
{
    int row = 0;
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);
    while(query.next()){
        ui->tableWidget->insertRow(row);

        QTableWidgetItem *newItem0 = new QTableWidgetItem(query.value(0).toString());
        ui->tableWidget->setItem(row, 0, newItem0);

        QTableWidgetItem *newItem1 = new QTableWidgetItem(query.value(1).toString());
        ui->tableWidget->setItem(row, 1, newItem1);

        QTableWidgetItem *newItem2 = new QTableWidgetItem(query.value(2).toString());
        ui->tableWidget->setItem(row, 2, newItem2);

        QTableWidgetItem *newItem3 = new QTableWidgetItem(query.value(3).toString());
        ui->tableWidget->setItem(row, 3, newItem3);

        QTableWidgetItem *newItem4 = new QTableWidgetItem(query.value(4).toString());
        ui->tableWidget->setItem(row, 4, newItem4);

        row++;
    }
}

void MainWindow::on_btn_overview_clicked()
{
    QSqlQuery query;
    query.exec(QString("select id, name, type, state, position from goods"));
    query_show(query);
}

void MainWindow::on_btn_sort_type_clicked()
{
    // "物品ID" << "名称" << "种类" << "状态" << "所在位置"
    QSqlQuery query;
    query.exec(QString("select id, name, type, state, position from goods order by type"));
    query_show(query);
}

void MainWindow::on_btn_sort_state_clicked()
{
    // "物品ID" << "名称" << "种类" << "状态" << "所在位置"
    QSqlQuery query;
    query.exec(QString("select id, name, type, state, position from goods order by state"));
    query_show(query);
}

void MainWindow::on_btn_expire_hint_clicked()
{
    QDate date;
    date = QDate::fromString(getDateTime(Date), "yyyy-MM-dd");      // 获取当前时间
    date = date.addDays(ui->ldt_days->text().toInt());              // 在当前时间基础上增加N天

    QString date_plused;
    date_plused = date.toString("yyyy-MM-dd");                      // 将QDate数据类型的日期转换成QString数据类型的日期

    QSqlQuery query;
    query.exec(QString("select id, name, type, state, position from goods where type='消耗品' and to_days(date_expiration)<to_days('%1')").arg(date_plused));
    query_show(query);
}

void MainWindow::on_btn_search_clicked()
{
    QString id, name;

    id = ui->ldt_id_2->text();
    name = ui->ldt_name_2->text();

    if(!id.isEmpty() && name.isEmpty()) {
        QSqlQuery query;
        query.exec(QString("select id, name, type, state, position from goods where id=%1").arg(id));
        query_show(query);
    }
    else if(id.isEmpty() && !name.isEmpty()) {
        QSqlQuery query;
        query.exec(QString("select id, name, type, state, position from goods where name='%1'").arg(name));
        query_show(query);
    }
    else {
        QMessageBox::critical(this, tr("错误"), tr("请输入待查找的关键字"), QMessageBox::Ok);
    }
}

void MainWindow::on_tableWidget_cellDoubleClicked(int row, int column)
{
    column = column+0;
    QString id;

    id = ui->tableWidget->item(row, 0)->text();         // 获取双击对应的物品ID
    this->ui->stackedWidget->setCurrentIndex(2);        // 跳转到物品状态的界面

    query_model_state->setQuery(QString("select * from goods where id = %1").arg(id));
    query_model_state->setHeaderData(0, Qt::Horizontal, tr("ID"));
    query_model_state->setHeaderData(1, Qt::Horizontal, tr("名称"));
    query_model_state->setHeaderData(2, Qt::Horizontal, tr("种类"));
    query_model_state->setHeaderData(3, Qt::Horizontal, tr("状态"));
    query_model_state->setHeaderData(4, Qt::Horizontal, tr("位置"));
    query_model_state->setHeaderData(5, Qt::Horizontal, tr("价格"));
    query_model_state->setHeaderData(6, Qt::Horizontal, tr("供应商"));
    query_model_state->setHeaderData(7, Qt::Horizontal, tr("拥有者"));
    query_model_state->setHeaderData(8, Qt::Horizontal, tr("入库日期"));
    query_model_state->setHeaderData(9, Qt::Horizontal, tr("有效日期"));
    query_model_state->setHeaderData(10, Qt::Horizontal, tr("变更日期"));
    query_model_state->setHeaderData(11, Qt::Horizontal, tr("备注"));
}

void MainWindow::on_btn_search_state_clicked()
{
    QString id;

    id = ui->ldt_id_3->text();
    query_model_state->setQuery(QString("select * from goods where id = %1").arg(id));
}

void MainWindow::on_btn_borrow_clicked()
{
    // 获取借出人名字
    QString name;
    name = ui->ldt_name_state->text();

    // 获取ID
    int id = query_model_state->data(query_model_state->index(0, 0)).toInt();

    // 获取当前时间
    QString date;
    date = getDateTime(Date);

    query_model_state->setQuery(QString("update goods set state='借出', owner='%1', date_change='%2' where id = %3").arg(name).arg(date).arg(id));
    query_model_state->setQuery(QString("select * from goods where id = %1").arg(id));
}

void MainWindow::on_btn_return_clicked()
{
    // 获取ID
    int id = query_model_state->data(query_model_state->index(0, 0)).toInt();

    // 获取当前时间
    QString date;
    date = getDateTime(Date);

    query_model_state->setQuery(QString("update goods set state='库存', owner='admin', date_change='%1' where id = %2").arg(date).arg(id));
    query_model_state->setQuery(QString("select * from goods where id = %1").arg(id));
}


void MainWindow::on_btn_damage_clicked()
{
    // 损坏原因
    QString reason;
    reason = ui->ldt_reason->text();

    // 获取ID
    int id = query_model_state->data(query_model_state->index(0, 0)).toInt();

    // 获取当前时间
    QString date;
    date = getDateTime(Date);

    query_model_state->setQuery(QString("update goods set state='损坏', other='%1', date_change='%2' where id = %3").arg(reason).arg(date).arg(id));
    query_model_state->setQuery(QString("select * from goods where id = %1").arg(id));
}
