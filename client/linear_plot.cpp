#include "linear_plot.h"
#include "ui_linear_plot.h"
#include <fstream>
#include "streamtable.h"

void getFields(std::string& str, const std::size_t& argc, ...);

LinearPlot::LinearPlot(QWidget *parent) : QWidget(parent), ui(new Ui::LinearPlot)
{
    ui->setupUi(this);
    connect(ui->okeyButton, &QPushButton::clicked, this, [=](){delete ui->layout->takeAt(0); QWidget::close();});
    setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint);
}

LinearPlot::~LinearPlot()
{
    delete ui;
}

void LinearPlot::createPlot(std::string avgs)
{
    char company[5][32], avarage[5][32];
    for (std::size_t i = 0; i < 5; i++)
    {
        getFields(avgs, 3, avarage[i], company[i]);
    }


    std::ofstream fout("current_price.txt");
    StreamTable output(fout);
    output.AddCol(40);
    output.AddCol(40);
    output.MakeBorderExt(true);
    output.SetDelimRow(true, '-');
    output.SetDelimCol(true, '|');

    fout << "Avarages, Current Prices" << std::endl;
    for(int i = 0; i < 5; i++)
    {
        fout << "Company: " << company[i] << ", " << "Current Price: " << avarage[i] << std::endl;
    }

    ui ->label->setText(std::string("0 " + std::string(company[0]) + ", 1 " + company[1] + ", 2 " + company[2] + ", 3 " + company[3] + ", 4 " + company[4]).c_str());
    ui ->label ->setStyleSheet("color: black");
    QLineSeries* ser = new QLineSeries();


    for (int i = 0 ; i < 5; i++)
    {
        ser->append(i, atof(avarage[i]));
    }

    QChart *chart = new QChart();
    chart->addSeries(ser);
    chart->setTitle("Текущая цена услуги");
    chart->setAnimationOptions(QChart::SeriesAnimations);


    QValueAxis *axisX = new QValueAxis();
      axisX->setTitleText("Компании");
      axisX->setLabelFormat("%i");
      axisX->setTickCount(1);
      chart->addAxis(axisX, Qt::AlignBottom);
      ser->attachAxis(axisX);

      QValueAxis *axisY = new QValueAxis();
      axisY->setTitleText("Текущая цена");
      axisY->setLabelFormat("%g");
      axisY->setTickCount(5);
      chart->addAxis(axisY, Qt::AlignLeft);
      ser->attachAxis(axisY);


      chart->legend()->setVisible(true);

    QChartView* chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    ui ->layout ->addWidget(chartView);
}
