#ifndef TAGWIDGET_H
#define TAGWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QListWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "Headers/Model/tagManager.h"
#include "Headers/Model/ActivityManager.h"
#include <QColorDialog>

class TagWidget : public QWidget {
    Q_OBJECT
private:
    tagManager& tm;
    ActivityManager& am;

    QLineEdit* tagInput;
    QPushButton* btnAdd;
    QListWidget* tagList;
    QPushButton* btnBack;

    QPushButton* btnColorPick;
    QWidget* paletteContainer;
    QColor currentSelectedColor;

    const QList<QColor> defaultColors = {
        QColor::fromRgb(255,140,140),
        QColor::fromRgb(209,221,147),
        QColor::fromRgb(240,237,170),
        QColor::fromRgb(194,213,244),
        QColor::fromRgb(189,238,237),

        QColor::fromRgb(249,150,174),
        QColor::fromRgb(159,244,223),
        QColor::fromRgb(255,195,120),
        QColor::fromRgb(107,206,238),
        QColor::fromRgb(217,182,253),

        QColor::fromRgb(243,168,188),
        QColor::fromRgb(245,173,148),
        QColor::fromRgb(255,103,103),
        QColor::fromRgb(180,249,165),
        QColor::fromRgb(158,231,245)
    };

    void setupUI();
    void populateList();
    void createPaletteGrid();

private slots:
    void onAddClicked();
    void onDeleteTagClicked(const std::string& tagName);
    void openColorDialog();

signals:
    void tagsChanged();
    void tagViewClosed();

public:
    explicit TagWidget(tagManager& tm, ActivityManager& am, QWidget* parent = nullptr);
    void refresh();
};

#endif // TAGWIDGET_H