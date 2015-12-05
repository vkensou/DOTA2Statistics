#pragma once

#include <QWidget>
#include "idataview.h"

namespace Ui {
	class View_HeroItems;
} // namespace Ui
class HeroItems;
class QCompleter;

class View_HeroItems
	:public IDataView
{
	Q_OBJECT
public:
	View_HeroItems(QWidget *parent = nullptr);
	~View_HeroItems();

	virtual int getType() override;
	virtual const QString & getViewName() override;

private slots:
	void on_btn_calc_clicked();
	void on_table_sort(int);

private:
	void initConfigPanel();
	void updateConfigPanel();
	void showItemsX2(const HeroItems &items);
	void setTableWidgetHead();
	void setHeroNmaeCompleter();
	void updateConfig();


private:
	Ui::View_HeroItems *ui;
	bool m_table_sortorder;
	QCompleter *m_completer;
};