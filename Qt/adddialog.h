#ifndef ADDDIALOG_H
#define ADDDIALOG_H

#include <QWidget>
#include <QDialog>

namespace Ui {
class AddDialog;
}

class AddDialog : public QDialog
{
    Q_OBJECT

public:
    AddDialog(QDialog *parent = nullptr);
    ~AddDialog();

private:
    Ui::AddDialog *ui;

public:
    QList<QString> getFields();
};

#endif // ADDDIALOG_H
