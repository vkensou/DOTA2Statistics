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
#include "idataview.h"
#include "abilitieslist.h"
#include "matchdetailmanager.h"

namespace Ui {
class MainWindow;
}
class StatusBarSeter;
class QCompleter;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
	void matchDblClicked(int matchid);

private slots:
    void on_action_about_triggered();
	void on_action_set_datasource_triggered();
	void on_action_joinDatabase_triggered();
	void on_action_view_heroitems_triggered();
	void on_action_view_playermatchhistory_triggered();
	void on_action_view_matchdetail_triggered();
	void setStatusBarText(const QString &text);
	void on_tabWidget_tabCloseRequested(int index);
	void on_MatchDblClicked(int matchid);

private:
	void initStatusBar();
	IDataView *getDataView(IDataView::ViewType type);
	void tableAddTab(IDataView::ViewType type);

private:
    Ui::MainWindow *ui;
    HeroList m_herolist;
    HeroesUsedAndRateManager m_hrumanager;
    DataBaseManager m_databasemanager;
	WebDataDownloader m_webdatadownloader;
    HeroItemsManager m_heroitemsmanager;
	WebDataSourceManager m_webdatasourcemanager;
 	StatusBarSeter *m_statusbarsetter;
	AbilitiesList m_abilities;
	MatchDetailManager m_matchdetailmanager;

	IDataView *m_dataviews[IDataView::Count];
};

#endif // MAINWINDOW_H
