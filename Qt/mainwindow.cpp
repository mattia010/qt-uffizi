#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "adddialog.h"

#include <QFile>
#include <QTextStream>
#include <QtCharts>
using namespace QtCharts;

#include "set.hpp"

#define PAGE_STEP 5

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , collezione()
{
    ui->setupUi(this);

    connect(ui->bar_chart_horizontalScrollBar,
            SIGNAL(valueChanged(int)),
            this,
            SLOT(on_scroll_bar_scrolled(int)));
    connect(ui->search_lineEdit,
            SIGNAL(textChanged(QString)),
            this,
            SLOT(updateUI()));
    connect(ui->add_button,
            SIGNAL(clicked(bool)),
            this,
            SLOT(on_add_clicked()));
    connect(ui->remove_button,
            SIGNAL(clicked(bool)),
            this,
            SLOT(on_remove_clicked()));

    readCSV("dipinti_uffizi.csv");

    //inizializza la tabella e i suoi header
    ui->tableWidget->setColumnCount(5);
    ui->tableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem("Scuola", QTableWidgetItem::Type));
    ui->tableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem("Autore", QTableWidgetItem::Type));
    ui->tableWidget->setHorizontalHeaderItem(2, new QTableWidgetItem("Titolo", QTableWidgetItem::Type));
    ui->tableWidget->setHorizontalHeaderItem(3, new QTableWidgetItem("Data", QTableWidgetItem::Type));
    ui->tableWidget->setHorizontalHeaderItem(4, new QTableWidgetItem("Sala", QTableWidgetItem::Type));
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    //inizializza i grafici
    ui->bar_chart_view->setChart(new QChart());
    ui->pie_chart_view->setChart(new QChart());

    updateUI();
}

MainWindow::~MainWindow()
{
    //dealloca gli oggetti dell'header della tabella
    for (int i = 0; i < ui->tableWidget->columnCount(); ++i) {
        delete ui->tableWidget->horizontalHeaderItem(i);
    }

    //dealloca i grafici
    delete ui->bar_chart_view->chart();
    delete ui->pie_chart_view->chart();

    delete ui;
}

void MainWindow::readCSV(QString filename) {
    QFile csv(filename);
    csv.open(QIODevice::ReadOnly);
    QTextStream istream(&csv);
    if (!istream.atEnd()) {
        istream.readLine();
    }

    //lettura e parsing del CSV
    QString line;
    QStringList splitted;
    while (!istream.atEnd()) {
        line = istream.readLine();

        //algo di parsing CSV
        QString buffer;
        bool quotesEven = true;
        for (int i = 0; i < line.size(); ++i) {
            if (line.at(i) == ',' && quotesEven) {
                //trovato un separatore e i doppi apici sono pari, quindi si è sicuri che il campo non è escaped
                splitted.append(buffer);
                buffer = "";
            } else if (line.at(i) == '"') {
                quotesEven = !quotesEven;
            } else {
                buffer.append(line.at(i));
            }
            if (i == line.size() - 1) {
                splitted.append(buffer);
            }
        }

        collezione.add(dipinto(splitted.at(0),
                               splitted.at(1),
                               splitted.at(2),
                               splitted.at(3),
                               splitted.at(4)));

        splitted.clear();
    }
    csv.close();
}

void MainWindow::updateUI() {
    QString lineEditStr = ui->search_lineEdit->text();
    Set<dipinto> showables;
    for (Set<dipinto>::const_iterator it = collezione.begin(); it != collezione.end(); ++it) {
        if (it->titolo.contains(lineEditStr)) {
            showables.add(*it);
        }
    }
    updateTable(showables);
    updatePieChart(showables);
    updateBarChart(showables);
}

void MainWindow::updateTable(const Set<dipinto> &showables) {
    int showablesNum = 0;
    for (Set<dipinto>::const_iterator it = showables.begin(); it != showables.end(); ++it) {
        ++showablesNum;
    }

    ui->tableWidget->clearContents();       //dealloca vecchi elementi
    ui->tableWidget->setRowCount(showablesNum);
    int counter = 0;
    for (Set<dipinto>::const_iterator it = showables.begin(); it != showables.end(); ++it) {
        ui->tableWidget->setItem(counter, 0, new QTableWidgetItem(it->scuola, QTableWidgetItem::Type));
        ui->tableWidget->setItem(counter, 1, new QTableWidgetItem(it->autore, QTableWidgetItem::Type));
        ui->tableWidget->setItem(counter, 2, new QTableWidgetItem(it->titolo, QTableWidgetItem::Type));
        ui->tableWidget->setItem(counter, 3, new QTableWidgetItem(it->data, QTableWidgetItem::Type));
        ui->tableWidget->setItem(counter, 4, new QTableWidgetItem(it->sala, QTableWidgetItem::Type));
        ++counter;
    }

}

void MainWindow::updateBarChart(const Set<dipinto> &showables) {
    //dealloca oggetti già presenti nel bar chart che devono essere rimossi.
    //eliminando il chart si eliminano anche oggetti figli (series e set)
    ui->bar_chart_view->chart()->deleteLater();

    Set<QString> data_set;
    int dataNum = 0;
    for (Set<dipinto>::const_iterator it = showables.begin(); it != showables.end(); ++it) {
        data_set.add((*it).data);
    }
    for (Set<QString>::const_iterator it = data_set.begin(); it != data_set.end(); ++it) {
        ++dataNum;
    }

    //bar series e bar set
    QStackedBarSeries *bar_series = new QStackedBarSeries();
    QBarSet *set = new QBarSet("set");
    set->setColor(QColorConstants::DarkGray);
    int upper_limit_axisY = 0;
    for (Set<QString>::const_iterator it_data = data_set.begin(); it_data != data_set.end(); ++it_data) {
        int data_counter = 0;
        for (Set<dipinto>::const_iterator it_showables = showables.begin(); it_showables != showables.end(); ++it_showables) {
            if ((*it_showables).data == *it_data) {
                ++data_counter;
            }
        }
        *set << data_counter;
        if (upper_limit_axisY < data_counter) {
            upper_limit_axisY = data_counter;
        }
    }
    bar_series->append(set);

    //bar chart
    QChart *bar_chart = new QChart();
    bar_chart->addSeries(bar_series);
    bar_chart->setTitle("Dipinti per data");
    bar_chart->setAnimationOptions(QChart::SeriesAnimations);
    bar_chart->legend()->hide();

    //asse X bar chart
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->setGridLineVisible(false);
    for (Set<QString>::const_iterator it = data_set.begin(); it != data_set.end(); ++it) {
        axisX->append(*it);
    }

    //mostra un numero di colonne accettabile definito da un page step.
    //le colonne non visibili possono essere comunque visite scorrendo la scroll bar.
    if (dataNum > PAGE_STEP) {
        axisX->setRange(axisX->at(0), axisX->at(PAGE_STEP - 1));
    }

    bar_chart->addAxis(axisX, Qt::AlignBottom);
    bar_series->attachAxis(axisX);

    //asse Y bar chart
    QValueAxis *axisY = new QValueAxis();
    axisY->setLabelFormat("%u");
    if (upper_limit_axisY > 5) {
        axisY->setRange(0, upper_limit_axisY);
        axisY->setTickCount(upper_limit_axisY + 1);
    } else {
        axisY->setRange(0, upper_limit_axisY + 2);
        axisY->setTickCount(upper_limit_axisY + 2 + 1);
    }
    bar_chart->addAxis(axisY, Qt::AlignLeft);
    bar_series->attachAxis(axisY);

    ui->bar_chart_view->setChart(bar_chart);

    updateHorizontalScrollBar(data_set);
}

void MainWindow::updatePieChart(const Set<dipinto> &showables) {
    //dealloca grafo (e di conseguenza tutti gli oggetti figli)
    ui->pie_chart_view->chart()->deleteLater();

    //trova tutte le scuole che devono essere mostrate nel grafico
    Set<QString> scuola_set;
    int scuoleNum = 0;
    for (Set<dipinto>::const_iterator it_showables = showables.begin(); it_showables != showables.end(); ++it_showables) {
        scuola_set.add((*it_showables).scuola);
        ++scuoleNum;
    }

    //crea una pie slice per ogni scuola e la inserisce nella serie
    QPieSeries *pie_series = new QPieSeries();
    for (Set<QString>::const_iterator it_scuole = scuola_set.begin(); it_scuole != scuola_set.end(); ++it_scuole) {
        int scuola_counter = 0;
        for (Set<dipinto>::const_iterator it_dipinti = showables.begin(); it_dipinti != showables.end(); ++it_dipinti) {
            if ((*it_dipinti).scuola == *it_scuole) {
                ++scuola_counter;
            }
        }
        QPieSlice *slice = new QPieSlice();
        slice->setValue(scuola_counter);

        //aggiunge una label alla slice e la rende visibile solo se la fetta
        // è abbastanza "grossa" rispetto al totale. Se troppo piccola non è visibile
        // in modo da evitare di sovrapporre troppe label del grafico.
        qreal percentage = (100 * qreal(scuola_counter)) / qreal(scuoleNum);
        percentage *= 10 * 2;               //
        percentage = qRound(percentage);    //tronca alla seconda cifra decimale
        percentage /= 10 * 2;               //
        QString labelStr = *it_scuole;
        labelStr.append(": ").append(QString::number(percentage)).append("%");
        slice->setLabel(labelStr);
        if (scuola_counter >= (scuoleNum / 16)) { //la slice occupa almeno l'8%
            slice->setLabelVisible();
        } else {
            slice->setLabelVisible(false);
        }

        pie_series->append(slice);
    }

    connect(pie_series,
            SIGNAL(hovered(QPieSlice*,bool)),
            this,
            SLOT(on_slice_hovered(QPieSlice*,bool)));

    //crea il grafo vero e proprio utilizzando la serie
    QChart *pie_chart = new QChart();
    pie_chart->addSeries(pie_series);
    pie_chart->setTitle("Dipinti per scuola");
    pie_chart->legend()->hide();
    pie_chart->setAnimationOptions(QChart::SeriesAnimations);

    ui->pie_chart_view->setChart(pie_chart);
}

void MainWindow::updateHorizontalScrollBar(const Set<QString> &data_set) {
    int dataNum = 0;
    for (Set<QString>::const_iterator it = data_set.begin(); it != data_set.end(); ++it) {
        ++dataNum;
    }

    if (dataNum <= PAGE_STEP) {
        //caso in cui il numero di barre da mostrare è minore del page step
        ui->bar_chart_horizontalScrollBar->setRange(0, 0);
    } else if (dataNum % PAGE_STEP == 0) {
        //caso in cui il numero di barre è maggiore del page step e suo multiplo
        ui->bar_chart_horizontalScrollBar->setRange(0, (dataNum / PAGE_STEP) - 1);
    } else {
        //caso in cui il numero di barre è maggiore del pege step ma non suo multiplo
        ui->bar_chart_horizontalScrollBar->setRange(0, dataNum / PAGE_STEP);
    }
    ui->bar_chart_horizontalScrollBar->setValue(0);
}

void MainWindow::on_scroll_bar_scrolled(int scrollBarPosition) {
    QBarCategoryAxis *axis = dynamic_cast<QBarCategoryAxis*>(ui->bar_chart_view->chart()->axes(Qt::Horizontal).first());

    //calcola i nuovi limiti del range del grafico basandosi sulla posizione della scroll bar
    if (scrollBarPosition == ui->bar_chart_horizontalScrollBar->maximum()) {
        if (axis->count() > PAGE_STEP) {
            axis->setRange(axis->at(axis->count() - PAGE_STEP), axis->at(axis->count() - 1));
        } else {
            axis->setRange(axis->at(0), axis->at(axis->count() - 1));
        }
    } else {
        axis->setRange(axis->at(scrollBarPosition * PAGE_STEP), axis->at(((scrollBarPosition + 1) * PAGE_STEP) - 1));
    }
}

void MainWindow::on_slice_hovered(QPieSlice *slice, bool isHovered) {
    QPieSeries *series = dynamic_cast<QPieSeries*>(ui->pie_chart_view->chart()->series().first());
    QList<QPieSlice*> slices = series->slices();
    for (auto it = slices.begin(); it != slices.end(); ++it) {
        if (isHovered == true) {
            //una slice è diventata hovered
            if (*it != slice) {
                //la slice iterata è quella hovered
                (*it)->setLabelVisible(false);
                (*it)->setExploded(false);
            } else {
                //la slice iterata non è quella hovered
                (*it)->setLabelVisible(true);
                (*it)->setExploded(true);
            }
        } else {
            //una slice non è più hovered
            (*it)->setExploded(false);
            if ((*it)->value() >= series->sum() / 16) {
                (*it)->setLabelVisible(true);
            } else {
                (*it)->setLabelVisible(false);
            }
        }
    }
}

void MainWindow::on_add_clicked() {
    AddDialog *dialog = new AddDialog();
    if (dialog->exec()) {
        QList<QString> list = dialog->getFields();
        collezione.add(dipinto(list.at(0),
                               list.at(1),
                               list.at(2),
                               list.at(3),
                               list.at(4)));
    }
    delete dialog;
    updateUI();
}

void MainWindow::on_remove_clicked() {
    QList<QTableWidgetSelectionRange> selectedRanges = ui->tableWidget->selectedRanges();
    if (!selectedRanges.empty()) {
        for (auto it = selectedRanges.begin(); it != selectedRanges.end(); ++it) {
            for (int i = it->topRow(); i <= it->bottomRow(); ++i) {
                collezione.remove(dipinto(ui->tableWidget->item(i, 0)->text(),
                                          ui->tableWidget->item(i, 1)->text(),
                                          ui->tableWidget->item(i, 2)->text(),
                                          ui->tableWidget->item(i, 3)->text(),
                                          ui->tableWidget->item(i, 4)->text()));
            }
        }
        updateUI();
    }
}
