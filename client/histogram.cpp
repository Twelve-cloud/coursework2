#include "histogram.h"
#include "ui_histogram.h"
#include <string>

void getFields(std::string& str, const std::size_t& argc, ...);

Histogram::Histogram(QWidget *parent) : QMainWindow(parent), ui(new Ui::Histogram)
{
    ui->setupUi(this);
    connect(ui->okeyButton, &QPushButton::clicked, this, [=](){delete ui->layout->takeAt(0); QWidget::close();});
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

    QBarSet *fc = new QBarSet((std::string(company[0]) + ", средняя цена: " + std::string(avarage[0]).substr(0, 6)).c_str());
    QBarSet *sc = new QBarSet((std::string(company[1]) + ", средняя цена: " + std::string(avarage[1]).substr(0, 6)).c_str());
    QBarSet *tc= new QBarSet((std::string(company[2]) + ", средняя цена: " + std::string(avarage[2]).substr(0, 6)).c_str());
    QBarSet *foc = new QBarSet((std::string(company[3]) + ", средняя цена: " + std::string(avarage[3]).substr(0, 6)).c_str());
    QBarSet *fic = new QBarSet((std::string(company[4]) + ", средняя цена: " + std::string(avarage[4]).substr(0, 6)).c_str());

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

    std::vector<double> avg = {atof(avarage[0]), atof(avarage[1]), atof(avarage[2]), atof(avarage[3]), atof(avarage[4])};

    std::map<double, double> avg_risk;

    for (std::size_t i = 0; i < 5; i++)
    {
        avg_risk.insert(std::pair<double, double>(atof(avarage[i]), atof(risk[i])));
    }

    double avgOfGoodCompany = 0;
    for (std::size_t i = 30; i < 100; i+=30)
    {
        bool isGood = false;
        std::size_t j = 0;
        for (const auto& k : avg_risk)
        {
            if (k.second < i)
            {
                avgOfGoodCompany = k.first;
                isGood = true;
                break;
            }
            j++;
        }

        if(isGood == true)
        {
            break;
        }
    }

    std::size_t indexOfGoodCompany = -1;
    for (int i = 0; i < 5; i++)
    {
        if (avgOfGoodCompany == avg[i])
        {
            indexOfGoodCompany = i;
            break;
        }

    }

    ui->chosenCompanyLabel->setText(QString("Выбранная компания для этой услуги: ") + QString(company[indexOfGoodCompany]));
}

Histogram::~Histogram()
{
    delete ui;
}
