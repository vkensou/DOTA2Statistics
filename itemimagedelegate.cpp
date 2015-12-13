#include "itemimagedelegate.h"
#include <QPainter>
#include <map>

ItemImageDelegate::ItemImageDelegate(QWidget *parent /*= nullptr*/)
	:QStyledItemDelegate(parent)
{

}

void ItemImageDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	if (index.data().canConvert<ItemList>())
	{
		ItemList items = qvariant_cast<ItemList>(index.data());

		if (option.state & QStyle::State_Selected)
			painter->fillRect(option.rect, option.palette.highlight());

		int left = option.rect.x(), top = option.rect.y();
		for (int i = 0; i < 6; ++i)
		{
			if (items.items[i] == 0)
				continue;

			static std::map<int, QImage> images;
			{
				if (images.find(items.items[i]) == images.end())
				{
					images.insert(std::pair<int, QImage>(items.items[i], QImage()));
					QImage &image = images[items.items[i]];
					static QString imagename(":/item/resources/item_%1.png");
					image.load(imagename.arg(items.items[i]));
					if (image.isNull())
						image.load(imagename.arg("error"));

					image = image.scaled(34, 26, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
				}
			}

			QImage &image = images[items.items[i]];

			QRect drawrect(left, top, 34, 26);
			if (drawrect.left() > option.rect.right())
				continue;
			if (drawrect.right() > option.rect.right())
				drawrect.setRight(option.rect.right());
			QRect srcrect(0, 0, drawrect.width(), 26);
			painter->drawImage(drawrect, image, srcrect);
			left += 34;
		}
	}
	else
		QStyledItemDelegate::paint(painter, option, index);
}

QSize ItemImageDelegate::sizeHint(const QStyleOptionViewItem &, const QModelIndex &index) const
{
	return QSize(34 * 6, 26);
}
