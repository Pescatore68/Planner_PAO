#ifndef TASKWIDGET_H
#define TASKWIDGET_H

#include <QWidget>
#include <QTreeWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QStyledItemDelegate>
#include <QStyleOptionProgressBar>
#include <QApplication>
#include <QPainter>

#include "Headers/ActivityManager.h"
#include "Headers/AbstractActivity.h"
#include "Headers/UI/ActivityDelete.h"

class task;
class project;

class TaskWidget : public QWidget {
    Q_OBJECT

public:
    explicit TaskWidget(ActivityManager& am, QWidget* parent = nullptr);
    ActivityDelete& getActivityDelete() { return activityDelete; }
    void refresh();
public slots:
    void handleExternalActivitySelected(AbstractActivity* a);

signals:
    void activitySelected(AbstractActivity* a);
    void activityDoubleClicked(AbstractActivity* a);
    void deleteRequested(AbstractActivity* a);

private slots:
    void onItemClicked(QTreeWidgetItem* item, int column);
    void onItemChanged(QTreeWidgetItem* item, int column);
    void onItemDoubleClicked(QTreeWidgetItem* item, int column);
    void onDeleteClicked();

private:
    ActivityManager& am;
    ActivityDelete activityDelete;

    QVBoxLayout* mainLayout;
    QHBoxLayout* toolbarLayout;
    QTreeWidget* tree;

    QPushButton* btnDelete;

    AbstractActivity* current = nullptr;

    void buildTree();

    AbstractActivity* activityFromItem(QTreeWidgetItem* item) const;
signals:
    void activityUpdated();
};


//PROGRESS BAR!
class ProgressBarDelegate : public QStyledItemDelegate {
    Q_OBJECT
public:
    static constexpr int ProgressRole = Qt::UserRole + 10;
    static constexpr int TagColorRole = Qt::UserRole + 11;

    explicit ProgressBarDelegate(QObject* parent = nullptr)
        : QStyledItemDelegate(parent) {}

    void paint(QPainter* painter,
               const QStyleOptionViewItem& option,
               const QModelIndex& index) const override
    {
        QVariant v = index.data(ProgressRole);
        if (!v.isValid() || v.toInt() < 0) {
            QStyledItemDelegate::paint(painter, option, index);
            return;
        }

        QStyledItemDelegate::paint(painter, option, index);

        int pct = qBound(0, v.toInt(), 100);

        QVariant vColor = index.data(TagColorRole);
        QColor barColor = vColor.canConvert<QColor>() ? vColor.value<QColor>() : QColor(200, 200, 200);

        float barHeight = 3.0f;
        float borderRadius = barHeight / 2.0f;


        float left = option.rect.left() + 16.0f;
        float right = option.rect.right() - 10.0f;
        float bottom = option.rect.bottom() - 2.0f;
        float top = bottom - barHeight;

        QRectF totalBarRect(left, top, right - left, barHeight);

        float fillWidth = (totalBarRect.width() * pct) / 100.0f;
        QRectF fillRect = totalBarRect;
        fillRect.setWidth(fillWidth);

        painter->save();
        painter->setRenderHint(QPainter::Antialiasing, true);
        painter->setPen(Qt::NoPen);

        painter->setBrush(QColor(235, 235, 235));
        painter->drawRoundedRect(totalBarRect, borderRadius, borderRadius);

        if (fillWidth > 0.0f) {
            painter->setBrush(barColor);
            painter->drawRoundedRect(fillRect, borderRadius, borderRadius);
        }

        painter->restore();
    }

    QSize sizeHint(const QStyleOptionViewItem& option,
                   const QModelIndex& index) const override
    {
        QVariant v = index.data(ProgressRole);
        if (v.isValid() && v.toInt() >= 0) {
            QSize s = QStyledItemDelegate::sizeHint(option, index);
            s.setHeight(s.height() + 12);
            return s;
        }
        return QStyledItemDelegate::sizeHint(option, index);
    }
};

#endif // TASKWIDGET_H