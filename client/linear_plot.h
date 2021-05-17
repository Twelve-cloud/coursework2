#ifndef LINEAR_PLOT_H
#define LINEAR_PLOT_H

#include <QWidget>
#include <QtCharts>

namespace Ui {
class LinearPlot;
}

class LinearPlot : public QWidget
{
    Q_OBJECT

public:
    explicit LinearPlot(QWidget *parent = nullptr);
    ~LinearPlot();

    void createPlot(std::string avgs);

private:
    Ui::LinearPlot *ui;
};

#endif // LINEAR_PLOT_H
