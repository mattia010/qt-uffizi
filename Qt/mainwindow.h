#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCharts>
#include "set.hpp"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

struct dipinto {
    QString scuola;
    QString autore;
    QString titolo;
    QString data;
    QString sala;

    dipinto() : scuola(""), autore(""), titolo(""), data(""), sala("") {}

    dipinto(QString scuolaParam,
            QString autoreParam,
            QString titoloParam,
            QString dataParam,
            QString salaParam) :
        scuola(scuolaParam),
        autore(autoreParam),
        titolo(titoloParam),
        data(dataParam),
        sala(salaParam) {}

    bool operator==(const dipinto &other) {
        if (scuola != other.scuola
            || autore != other.autore
            || titolo != other.titolo
            || data != other.data
            || sala != other.sala) {
            return false;
        }
        return true;
    }
};


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void on_add_clicked();
    void on_remove_clicked();
    void on_scroll_bar_scrolled(int scrollBarPosition);
    void on_slice_hovered(QPieSlice *slice, bool isHovered);
    void updateUI();

private:
    Ui::MainWindow *ui;

    Set<dipinto> collezione;

    void updatePieChart(const Set<dipinto> &showables);
    void updateBarChart(const Set<dipinto> &showables);
    void updateTable(const Set<dipinto> &showables);
    void updateHorizontalScrollBar(const Set<QString> &showables);
    void readCSV(QString filename);
};
#endif // MAINWINDOW_H
