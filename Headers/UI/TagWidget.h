#ifndef TAGWIDGET_H
#define TAGWIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QListWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "Headers/tagManager.h"
#include "Headers/ActivityManager.h"
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

    QPushButton* btnColorPick;   // Il pulsante-anteprima di fianco al testo
    QWidget* paletteContainer;     // Il contenitore della griglia (da mostrare/nascondere)
    QColor currentSelectedColor;

    const QList<Qt::GlobalColor> defaultColors = {
        Qt::red,     Qt::green,   Qt::blue,
        Qt::cyan,    Qt::magenta, Qt::yellow,
        Qt::darkRed, Qt::darkGreen, Qt::darkBlue,
        Qt::darkCyan, Qt::darkMagenta, Qt::darkYellow,
        Qt::gray,    Qt::darkGray,  Qt::black
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