#ifndef MQTTSUBSCRIBEDELEGATE_H
#define MQTTSUBSCRIBEDELEGATE_H

#include <QStyledItemDelegate>
#include <QPainter>

typedef struct {
  QString titleText;
  QString state;
  QWidget *widget;
  bool isSelect;
} itemProperty;
Q_DECLARE_METATYPE(itemProperty)

class mqttSubscribeDelegate : public QStyledItemDelegate
{
public:
    explicit mqttSubscribeDelegate(QObject *parent = nullptr);
    void paint(QPainter *painter,
                   const QStyleOptionViewItem &option,
                   const QModelIndex &index)
                   const override;
        QSize sizeHint(const QStyleOptionViewItem &option,
                       const QModelIndex &index)
                       const override;
};

#endif // MQTTSUBSCRIBEDELEGATE_H
