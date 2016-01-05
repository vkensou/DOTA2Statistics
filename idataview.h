#pragma once

#include <QWidget>


class IDataView
	:public QWidget
{
	Q_OBJECT
public:
	IDataView(QWidget *parent = nullptr)
		:QWidget(parent)
	{

	}

	enum ViewType
	{
		View_HeroItems = 0,
		View_PlayerMatchHistory,
		View_MatchDetail,
		View_FetchDAta,
		Count
	};

	virtual int getType() = 0;
	virtual const QString & getViewName() = 0;

	virtual void on_DataSource_Changed() = 0;
};