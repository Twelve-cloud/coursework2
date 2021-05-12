#include "histogram.h"
#include "ui_histogram.h"

void getFields(std::string& str, const std::size_t& argc, ...);

Histogram::Histogram(QWidget *parent) : QMainWindow(parent), ui(new Ui::Histogram)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint);

}

void Histogram::createHistogram(std::string avgs, std::string risks)
{
    char company[5][32], avarage[5][32], risk[5][32];
    for (std::size_t i = 0; i < 5; i++)
    {
        getFields(avgs, 3, avarage[i], company[i]);
        getFields(risks, 2, risk[i]);
    }

    QBarSet *fc = new QBarSet(company[0]);
    QBarSet *sc = new QBarSet(company[1]);
    QBarSet *tc= new QBarSet(company[2]);
    QBarSet *foc = new QBarSet(company[3]);
    QBarSet *fic = new QBarSet(company[4]);

    *fc << atof(risk[0]);
    *sc << atof(risk[1]);
    *tc << atof(risk[2]);
    *foc << atof(risk[3]);
    *fic << atof(risk[4]);

    QBarSeries *series = new QBarSeries();
    series->append(fc);
    series->append(sc);
    series->append(tc);
    series->append(foc);
    series->append(fic);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Риск при выборе решения через среднее арифметическое по цене");
    chart->setAnimationOptions(QChart::SeriesAnimations);


    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(0, 100);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignRight);

    QChartView* chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    ui ->layout ->addWidget(chartView);
}

Histogram::~Histogram()
{
    delete ui;
}
