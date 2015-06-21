#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "herolist.h"
#include "heroesrateandused.h"

namespace Ui {
class MainWindow;
}
class HeroItems;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_btn_CalcX2_clicked();


    void on_cbb_time_currentIndexChanged(int index);

    void on_cbb_server_currentIndexChanged(int index);

    void on_cbb_skill_currentIndexChanged(int index);

    void on_cbb_matchtype_currentIndexChanged(int index);

    void on_btn_calc_clicked();

private:
    Ui::MainWindow *ui;
    HeroList m_herolist;
    HeroesRateAndUsed m_hru;

    void showItemsX2(const HeroItems &items);
};

#endif // MAINWINDOW_H
