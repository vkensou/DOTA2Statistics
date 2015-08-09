#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include "herolist.h"
#include "heroesusedandratemanager.h"
#include "datamanager.h"
#include "heroitemsmanager.h"

namespace Ui {
class MainWindow;
}
class HeroItems;
class StatusBarSeter;

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
    void on_action_about_triggered();
    void on_table_sort(int);
	void setStatusBarText(const QString &text);

private:
    Ui::MainWindow *ui;
    HeroList m_herolist;
    HeroesUsedAndRateManager m_hrumanager;
    DataManager m_datamanager;
    HeroItemsManager m_heroitemsmanager;
    bool m_table_sortorder;
	StatusBarSeter *m_statusbarsetter;

	void initStatusBar();
    void showItemsX2(const HeroItems &items);
    void updateConfigPanel();
    void setTableWidgetHead();
};

#endif // MAINWINDOW_H
