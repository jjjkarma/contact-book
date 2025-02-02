#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "newcontactdialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow) {
    ui->setupUi(this);
    connect(ui->tableContacts, &QTableWidget::itemChanged,
            this, &MainWindow::on_tableContacts_itemChanged);

    // настройка таблицы контактов
    ui->tableContacts->setColumnCount(7);
    ui->tableContacts->setHorizontalHeaderLabels(
        QStringList() << "name" << "surname" << "patronymic"
                      << "address" << "date of birth" << "e-mail" << "number");

    ui->tableContacts->setSortingEnabled(true);

    refreshTable();
    // запрещаем редактирование в таблице до нажатия Edit
    ui->tableContacts->setEditTriggers(QAbstractItemView::NoEditTriggers);

}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_buttonNew_clicked()
{
    // создаём диалог
    NewContactDialog dialog(this);

    // показываем диалог и ждём, пока пользователь не закроет
    if (dialog.exec() == QDialog::Accepted) {
        // если пользователь нажал "Create" и диалог принялся, значит у нас есть новый контакт
        Contact created = dialog.getNewContact();
        // добавляем его в наш список и обновляем таблицу
        contacts.AddContact(created);
        refreshTable();
    }
}

void MainWindow::on_buttonEdit_clicked()
{
    // проверка, в каком режиме сейчас таблица
    QAbstractItemView::EditTriggers currentTriggers = ui->tableContacts->editTriggers();

    if (currentTriggers == QAbstractItemView::NoEditTriggers) {
        // режим  чтения, значит включаем редактирование
        ui->tableContacts->setEditTriggers(QAbstractItemView::DoubleClicked |
                                           QAbstractItemView::SelectedClicked);
        ui->buttonEdit->setText("Stop");
    } else {
        // режим редактирования, значит отключаем редактирование
        ui->tableContacts->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ui->buttonEdit->setText("Edit");
    }
}

void MainWindow::on_tableContacts_itemChanged(QTableWidgetItem *item)
{
    static bool inProgress = false;
    if (inProgress) return;

    inProgress = true;       // ставим флаг, что мы сейчас обрабатываем событие

    int row = item->row();
    int col = item->column();

    Contact oldContact = contacts.GetContact(row);
    Contact newContact = oldContact;
    std::string newValue = item->text().toStdString();

    std::string oldValue;
    switch (col) {
    case 0: oldValue = oldContact.GetName();       break;
    case 1: oldValue = oldContact.GetSurname();    break;
    case 2: oldValue = oldContact.GetPatronymic(); break;
    case 3: oldValue = oldContact.GetAddress();    break;
    case 4: oldValue = oldContact.GetDate();       break;
    case 5: oldValue = oldContact.GetEmail();      break;
    case 6: oldValue = oldContact.GetNumber();     break;
    }

    try {
        switch (col) {
        case 0: newContact.SetName(newValue);       break;
        case 1: newContact.SetSurname(newValue);    break;
        case 2: newContact.SetPatronymic(newValue); break;
        case 3: newContact.SetAddress(newValue);    break;
        case 4: newContact.SetDate(newValue);       break;
        case 5: newContact.SetEmail(newValue);      break;
        case 6: newContact.SetNumber(newValue);     break;
        }
    } catch (const std::invalid_argument &e) {
        QMessageBox::warning(this, "Error", e.what());
        item->setText(QString::fromStdString(oldValue));

        inProgress = false;  // снимаем флаг
        return;
    }
    contacts.UpdateContact(row, newContact);
    inProgress = false; // всё ок, снимаем флаг
}

void MainWindow::on_buttonDelete_clicked() {
    // получаем текущую выделенную строку таблицы
    int currentRow = ui->tableContacts->currentRow();
    if (currentRow < 0) {
        QMessageBox::warning(this, "Error", "Choose a contact to delete");
        return;
    }

    try {
        // получаем контакт, который пользователь хочет удалить
        Contact contactToDelete = contacts.GetContact(currentRow);

        // формируем текст подтверждения с именем и фамилией контакта
        QString message = QString("Are you sure you want to delete a contact \"%1 %2\"?")
                              .arg(QString::fromStdString(contactToDelete.GetName()))
                              .arg(QString::fromStdString(contactToDelete.GetSurname()));

        // Показываем диалог подтверждения с кнопками Да и Нет
        QMessageBox::StandardButton reply = QMessageBox::question(
            this,
            "Deleting contact",
            message,
            QMessageBox::Yes | QMessageBox::No
            );

        if (reply == QMessageBox::Yes) {
            contacts.DeleteContact(currentRow);
            QMessageBox::information(this, "Succesful", "Contact deleted");
            refreshTable();
        }
    }
    catch (const std::exception& e) {
        QMessageBox::warning(this, "Error", e.what());
    }
}

void MainWindow::on_buttonSort_clicked() {
    // получаем выбранное поле из comboBox
    QString sortField = ui->comboBoxSortBy->currentText().toLower();

    try {
        contacts.SortContacts(sortField.toStdString());  // сортируем
        QMessageBox::information(this, "Sorting", "Contacts are sorted by " + sortField);
        refreshTable();  // обновляем таблицу после сортировки
    } catch (const std::invalid_argument &e) {
        QMessageBox::warning(this, "Error", e.what());
    }
}


void MainWindow::on_buttonFind_clicked() {
    QString queryText = ui->lineEditFind->text().trimmed();
    if(queryText.isEmpty()){
        QMessageBox::warning(this, "Error", "Search line can not be empty");
        return;
    }

    // разбиваем введённый текст на слова для поиска
    std::istringstream iss(queryText.toStdString());
    std::vector<std::string> searchQueries;
    std::string word;
    while (iss >> word) {
        searchQueries.push_back(word);
    }

    // ищем контакты по запросу
    Contacts foundContacts = contacts.FindContacts(searchQueries);

    if(foundContacts.GetSize() == 0) {
        QMessageBox::information(this, "Search", "Nothing found");
        return;
    }

    // обновляем таблицу, показывая только найденные контакты
    ui->tableContacts->blockSignals(true);
    ui->tableContacts->setRowCount(0);
    for (size_t i = 0; i < foundContacts.GetSize(); ++i) {
        try {
            Contact c = foundContacts.GetContact(i);
            int row = ui->tableContacts->rowCount();
            ui->tableContacts->insertRow(row);
            ui->tableContacts->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(c.GetName())));
            ui->tableContacts->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(c.GetSurname())));
            ui->tableContacts->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(c.GetPatronymic())));
            ui->tableContacts->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(c.GetAddress())));
            ui->tableContacts->setItem(row, 4, new QTableWidgetItem(QString::fromStdString(c.GetDate())));
            ui->tableContacts->setItem(row, 5, new QTableWidgetItem(QString::fromStdString(c.GetEmail())));
            ui->tableContacts->setItem(row, 6, new QTableWidgetItem(QString::fromStdString(c.GetNumber())));
        } catch (const std::exception& e) {}
    }
    ui->tableContacts->blockSignals(false);

    QMessageBox::information(this, "Searching", "Contacts are successfuly found");
}

void MainWindow::on_buttonShowAll_clicked() {
    refreshTable();
    QMessageBox::information(this, "Contacts", "All contacts are shown");
}


void MainWindow::on_buttonSave_clicked() {
    // используем QFile для сохранения в файл
    QString filename = "/Users/moverzd/jjjkarma/up-2/contacts_list.txt";
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "Could not open the file for writing");
        return;
    }

    QTextStream out(&file);
    // проходим по всем контактам и сохраняем их
    for (size_t i = 0; i < contactsSize(); ++i) {
        try {
            Contact c = contacts.GetContact(i);
            out << QString::fromStdString(c.GetName()) << ","
                << QString::fromStdString(c.GetSurname()) << ","
                << QString::fromStdString(c.GetPatronymic()) << ","
                << QString::fromStdString(c.GetAddress()) << ","
                << QString::fromStdString(c.GetDate()) << ","
                << QString::fromStdString(c.GetEmail()) << ","
                << QString::fromStdString(c.GetNumber()) << "\n";
        } catch (...) {}
    }
    file.close();
    QMessageBox::information(this, "Saving", "Contacts were saved in file");
}

void MainWindow::on_buttonLoad_clicked() {
    QString filename = "/Users/moverzd/jjjkarma/up-2/contacts_list.txt";
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "Could not open the file for reading");
        return;
    }

    contacts = Contacts();  // очистить текущие контакты
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split(",");

        if (fields.size() >= 7) {

            std::string field0 = fields[0].toStdString();
            std::string field1 = fields[1].toStdString();
            std::string field2 = fields[2].toStdString();
            std::string field3 = fields[3].toStdString();
            std::string field4 = fields[4].toStdString();
            std::string field5 = fields[5].toStdString();
            std::string field6 = fields[6].toStdString();

            try {
                Contact c(field0, field1, field2, field3, field4, field5, field6);
                contacts.AddContact(c);

            } catch (const std::invalid_argument& e) {}
        }
    }
    file.close();
    QMessageBox::information(this, "Uploading", "Contacts were uploaded from file");
    refreshTable();
}

size_t MainWindow::contactsSize() const {
    return contacts.GetSize();
}

void MainWindow::refreshTable() {
    ui->tableContacts->blockSignals(true);
    ui->tableContacts->setRowCount(0);

    for (size_t i = 0; i < contactsSize(); ++i) {
        try {
            Contact c = contacts.GetContact(i);
            int row = ui->tableContacts->rowCount();
            ui->tableContacts->insertRow(row);
            ui->tableContacts->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(c.GetName())));
            ui->tableContacts->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(c.GetSurname())));
            ui->tableContacts->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(c.GetPatronymic())));
            ui->tableContacts->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(c.GetAddress())));
            ui->tableContacts->setItem(row, 4, new QTableWidgetItem(QString::fromStdString(c.GetDate())));
            ui->tableContacts->setItem(row, 5, new QTableWidgetItem(QString::fromStdString(c.GetEmail())));
            ui->tableContacts->setItem(row, 6, new QTableWidgetItem(QString::fromStdString(c.GetNumber())));
        } catch (const std::exception& e) {}
    }

    ui->tableContacts->blockSignals(false);

    // очистка выделения, чтобы ни одна строка не была выбрана по умолчанию
    ui->tableContacts->clearSelection();
}
