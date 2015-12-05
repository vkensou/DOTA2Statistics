#pragma execution_character_set("utf-8")

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "dataconfig.h"
#include "version.h"
#include "statusbarsetter.h"
#include <QMessageBox>
#include "setdatasourcedialog.h"
#include "heroitemsview.h"
#include "playermatchhistoryview.h"
#include <QtGlobal>

const QString key = "387B6D180AD105C6CD289B0556C7A846";


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
	initStatusBar();

	setWindowTitle("DOTA2统计学 V"PRODUCT_VERSION_STR);

    DataConfig::loadCurrent("datastatistics.ini");

	m_webdatasourcemanager.setCurrentSource(WebDataSourceManager::WebDataSource(DataConfig::getCurrentConfig().webdatasource));

    m_databasemanager.opendb();

    m_herolist.load();

	for (int i = 0; i < IDataView::Count; ++i)
	{
		m_dataviews[i] = nullptr;
	}

	tableAddTab(IDataView::View_PlayerMatchHistory);
}

MainWindow::~MainWindow()
{
    m_databasemanager.closedb();
    DataConfig::saveCurrent("datastatistics.ini");
    delete ui;
	delete m_statusbarsetter;
}

void MainWindow::initStatusBar()
{
	m_statusbarsetter = new StatusBarSeter;
	connect(StatusBarSeter::getInstancePtr(), SIGNAL(setStatusBar_impl(QString)), this, SLOT(setStatusBarText(QString)));
	setStatusBarText("Ready");
}

IDataView * MainWindow::getDataView(IDataView::ViewType type)
{
	IDataView *view = m_dataviews[type];
	if (view)
		return view;

	switch (type)
	{
	case IDataView::View_HeroItems:
		view = new HeroItemsView;
		break;
	case IDataView::View_PlayerMatchHistory:
		view = new PlayerMatchHistoryView;
		break;
	}
	m_dataviews[type] = view;
	return view;
}

void MainWindow::tableAddTab(IDataView::ViewType type)
{
	int index;
	if (!m_dataviews[type])
	{
		auto view = getDataView(type);
		index = ui->tabWidget->insertTab(type, view, view->getViewName());
	}
	else
	{
		for (int i = 0; i < ui->tabWidget->count(); ++i)
		{
			if (ui->tabWidget->widget(i) == m_dataviews[type])
			{
				index = i;
				break;
			}
		}
	}
		ui->tabWidget->setCurrentIndex(index);
}

void MainWindow::setStatusBarText(const QString &text)
{
	ui->statusBar->showMessage(text);
}

void MainWindow::on_tabWidget_tabCloseRequested(int index)
{
	auto tab = ui->tabWidget->widget(index);
	auto widget = dynamic_cast<IDataView*>(tab);

	Q_ASSERT(widget);

	m_dataviews[widget->getType()] = 0;
	ui->tabWidget->removeTab(index);
}

void MainWindow::on_action_about_triggered()
{
    QString title = "关于";
	QString text = "版本："PRODUCT_VERSION_STR"\r\nDeveloped by vkensou\r\nPowered by QT";

    QMessageBox::about(NULL, title, text);
}

void MainWindow::on_action_set_datasource_triggered()
{
	SetDataSourceDlg dlg;
	if (dlg.exec())
	{
		auto n = dlg.getSelected();
		WebDataSourceManager::getInstance().setCurrentSource(n);
		DataConfig::getCurrentConfig().webdatasource = (int)n;
		DataConfig::getCurrentConfig().reset();

		for (int i = 0; i < IDataView::Count; ++i)
		{
			if (m_dataviews[i])
				m_dataviews[i]->on_DataSource_Changed();
		}
	}
}

void MainWindow::on_action_view_heroitems_triggered()
{
	tableAddTab(IDataView::View_HeroItems);
}

void MainWindow::on_action_view_playermatchhistory_triggered()
{
	tableAddTab(IDataView::View_PlayerMatchHistory);
}

