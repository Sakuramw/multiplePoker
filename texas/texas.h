#ifndef TEXAS_H
#define TEXAS_H

#include <QWidget>

namespace Ui {
class Texas;
}

class Texas : public QWidget
{
    Q_OBJECT

public:
    explicit Texas(QWidget *parent = 0);
    ~Texas();
    void shuffle();
    void m_sleep(int msec);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_cal_clicked();

private:
    Ui::Texas *ui;
    QStringList poker;
    QVector<QStringList* > allPossible;
    QVector<int >sortInt;
};

#endif // TEXAS_H
