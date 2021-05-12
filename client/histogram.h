#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <QWidget>
#include <QtCharts>

namespace Ui {
class Histogram;
}

class Histogram : public QMainWindow
{
    Q_OBJECT

public:
    explicit Histogram(QWidget *parent = nullptr);
    ~Histogram();

    void createHistogram(std::string avgs, std::string risks);

private:
    Ui::Histogram *ui;
};

#endif // HISTOGRAM_H
