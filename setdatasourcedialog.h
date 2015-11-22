#ifndef setdatasourcedialog_h__
#define setdatasourcedialog_h__

#include <QDialog>
#include "webdatasourcemanager.h"

namespace Ui {
	class Dialog;
}

class SetDataSourceDlg
	:public QDialog
{
	Q_OBJECT
public:
	explicit SetDataSourceDlg(QWidget *parent = 0);
	~SetDataSourceDlg();

	WebDataSourceManager::WebDataSource getSelected();

private:
	Ui::Dialog *ui;
};

#endif // setdatasourcedialog_h__
