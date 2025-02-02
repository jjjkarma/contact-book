#include <QMainWindow>
#include <QTableWidgetItem>
#include "pull_contacts.h"
#pragma once

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_buttonNew_clicked();
    void on_buttonEdit_clicked();
    void on_tableContacts_itemChanged(QTableWidgetItem *item);
    void on_buttonDelete_clicked();
    void on_buttonSort_clicked();
    void on_buttonFind_clicked();
    void on_buttonShowAll_clicked();
    void on_buttonSave_clicked();
    void on_buttonLoad_clicked();

private:
    Ui::MainWindow *ui;
    Contacts contacts;  // экземпляр класса Contacts для управления списком контакто
    size_t contactsSize() const;
    void refreshTable(); // вспомогательная функция для обновления таблицы контактов

};
