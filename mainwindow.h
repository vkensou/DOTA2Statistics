#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "herolist.h"
#include "heroesusedandratemanager.h"
#include "datamanager.h"
#include "heroitemsmanager.h"

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
    void on_cbb_time_currentIndexChanged(int index);
    void on_cbb_server_currentIndexChanged(int index);
    void on_cbb_skill_currentIndexChanged(int index);
    void on_cbb_matchtype_currentIndexChanged(int index);

    void on_btn_calc_clicked();

private:
    Ui::MainWindow *ui;
    HeroList m_herolist;
    HeroesUsedAndRateManager m_hrumanager;
    DataManager datamanager;
    HeroItemsManager heroitemsmanager;

    void showItemsX2(const HeroItems &items);
    void updateConfigPanel();
};

#endif // MAINWINDOW_H
