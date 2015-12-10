#pragma once

#include <QStyledItemDelegate>

struct ItemList
{
	ItemList()
	{
		items[0] = 0;
		items[1] = 0;
		items[2] = 0;
		items[3] = 0;
		items[4] = 0;
		items[5] = 0;
	}
	ItemList(int item0, int item1, int item2, int item3, int item4, int item5)
	{
		items[0] = item0;
		items[1] = item1;
		items[2] = item2;
		items[3] = item3;
		items[4] = item4;
		items[5] = item5;
	}
	int items[6];
};

Q_DECLARE_METATYPE(ItemList);


class ItemImageDelegate
	:public QStyledItemDelegate
{
	Q_OBJECT
public:
	ItemImageDelegate(QWidget *parent = nullptr);

	virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

	virtual QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

};