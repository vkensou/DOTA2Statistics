#include "matchdetailview.h"

#include "ui_view_matchdetail.h"

MatchDetailView::MatchDetailView()
	:ui(new Ui::View_MatchDetail)
{
	ui->setupUi(this);
}

MatchDetailView::~MatchDetailView()
{
	delete ui;
}
