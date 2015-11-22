#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include "herolist.h"
#include "heroesusedandratemanager.h"
#include "databasemanager.h"
#include "webdatadownloader.h"
#include "heroitemsmanager.h"
#include "webdatasourcemanager.h"

namespace Ui {
class MainWindow;
}
class HeroItems;
class StatusBarSeter;
class QCompleter;

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
	void on_action_set_datasource_triggered();
    void on_table_sort(int);
	void setStatusBarText(const QString &text);

private:
    Ui::MainWindow *ui;
    HeroList m_herolist;
    HeroesUsedAndRateManager m_hrumanager;
    DataBaseManager m_databasemanager;
	WebDataDownloader m_webdatadownloader;
    HeroItemsManager m_heroitemsmanager;
	WebDataSourceManager m_webdatasourcemanager;
 	StatusBarSeter *m_statusbarsetter;
	bool m_table_sortorder;
	QCompleter *m_completer;
	bool m_initing;

	void initStatusBar();
	void initConfigPanel();
    void showItemsX2(const HeroItems &items);
    void updateConfigPanel();
    void setTableWidgetHead();
	void setHeroNmaeCompleter();
};

#endif // MAINWINDOW_H
