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
		Count
	};

	virtual int getType() = 0;
	virtual const QString & getViewName() = 0;
};