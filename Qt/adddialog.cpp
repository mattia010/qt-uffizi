#include "adddialog.h"
#include "ui_adddialog.h"

AddDialog::AddDialog(QDialog *parent)
    : QDialog(parent)
    , ui(new Ui::AddDialog)
{
    ui->setupUi(this);

    QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    this->setSizePolicy(sizePolicy);
    this->setMinimumSize(this->size());
    this->setMaximumSize(this->size());
    this->setSizeGripEnabled(false);

    connect(ui->confirmButton,
            SIGNAL(clicked(bool)),
            this,
            SLOT(accept()));
    connect(ui->cancelButton,
            SIGNAL(clicked(bool)),
            this,
            SLOT(reject()));
}

AddDialog::~AddDialog()
{
    delete ui;
}

QList<QString> AddDialog::getFields() {
    QList<QString> list;
    list.append(ui->scuola_lineEdit->text());
    list.append(ui->autore_lineEdit->text());
    list.append(ui->titolo_lineEdit->text());
    list.append(ui->data_lineEdit->text());
    list.append(ui->sala_lineEdit->text());

    return list;
}
