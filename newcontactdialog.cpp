#include "newcontactdialog.h"
#include "ui_newcontactdialog.h"
#include <QMessageBox>

NewContactDialog::NewContactDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewContactDialog)
{
    ui->setupUi(this);
}

NewContactDialog::~NewContactDialog()
{
    delete ui;
}

Contact NewContactDialog::getNewContact() const
{
    return newContact; // вернём созданный контакт
}

void NewContactDialog::on_buttonCreate_clicked()
{
    // считываем ввод
    std::string name       = ui->lineEditName->text().toStdString();
    std::string surname    = ui->lineEditSurname->text().toStdString();
    std::string patronymic = ui->lineEditPatronymic->text().toStdString();
    std::string address    = ui->lineEditAddress->text().toStdString();
    std::string date       = ui->lineEditDate->text().toStdString();
    std::string email      = ui->lineEditEmail->text().toStdString();
    std::string number     = ui->lineEditNumber->text().toStdString();

    try {
        Contact c(name, surname, patronymic, address, date, email, number);
        newContact = c; // сохраняем внутрь класса
        QMessageBox::information(this, "Success", "Contact are added successfuly");

        accept();
    }
    catch (const std::invalid_argument &e) {
        // если поля некорректны, покажем сообщение и не закроем диалог
        QMessageBox::warning(this, "Error", e.what());
    }
}
