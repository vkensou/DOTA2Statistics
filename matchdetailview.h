#pragma once

#include "idataview.h"

namespace Ui
{
	class View_MatchDetail;
}

class MatchDetailView
	:public IDataView
{
public:
	MatchDetailView();
	~MatchDetailView();

	void showMatchDetail(int matchid);

private:
	Ui::View_MatchDetail *ui;
};