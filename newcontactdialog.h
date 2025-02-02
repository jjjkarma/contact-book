#include <QDialog>
#include "contact.h"
#pragma once

namespace Ui {
class NewContactDialog;
}

class NewContactDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewContactDialog(QWidget *parent = nullptr);
    ~NewContactDialog();
    Contact getNewContact() const;

private slots:
    void on_buttonCreate_clicked();

private:
    Ui::NewContactDialog *ui;
    Contact newContact;

};
