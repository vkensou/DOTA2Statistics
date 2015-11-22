#include "setdatasourcedialog.h"

#include "ui_setdatasource.h"

SetDataSourceDlg::SetDataSourceDlg(QWidget *parent /*= 0*/)
	:QDialog(parent)
	, ui(new Ui::Dialog)
{
	ui->setupUi(this);
	auto e = WebDataSourceManager::getInstance().getWebDataSourceEnumCurrent();
	if (e == WebDataSourceManager::DOTAMAX)
		ui->radioButton->setChecked(true);
	else if (e == WebDataSourceManager::DOTABUFF)
		ui->radioButton_2->setChecked(true);
}

SetDataSourceDlg::~SetDataSourceDlg()
{
	delete ui;
}

WebDataSourceManager::WebDataSource SetDataSourceDlg::getSelected()
{
	if (ui->radioButton->isChecked())
		return WebDataSourceManager::DOTAMAX;
	else if (ui->radioButton_2->isChecked())
		return WebDataSourceManager::DOTABUFF;

	return WebDataSourceManager::DOTAMAX;
}
