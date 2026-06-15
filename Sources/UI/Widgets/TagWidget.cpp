#include "Headers/UI/Widgets/TagWidget.h"
#include "Headers/Model/tag.h"
#include <QLabel>
#include <QMessageBox>
#include <QColorDialog>

TagWidget::TagWidget(tagManager& tm, ActivityManager& am, QWidget* parent)
    : QWidget(parent), tm(tm), am(am), currentSelectedColor(Qt::gray)
{
    setupUI();
    populateList();
}

void TagWidget::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(15, 15, 15, 15);
    mainLayout->setSpacing(10);

    QLabel* titleLabel = new QLabel("Tag Manager", this);
    mainLayout->addWidget(titleLabel);

    QHBoxLayout* inputRowLayout = new QHBoxLayout();

    tagInput = new QLineEdit(this);
    tagInput->setPlaceholderText("New tag");

    btnColorPick = new QPushButton(this);
    btnColorPick->setFixedSize(28, 28);
    btnColorPick->setCursor(Qt::PointingHandCursor);
    btnColorPick->setStyleSheet(QString("QPushButton { background-color: %1; border: 2px solid #ccc; border-radius: 14px; }").arg(currentSelectedColor.name()));


    btnAdd = new QPushButton("add", this);

    inputRowLayout->addWidget(tagInput);
    inputRowLayout->addWidget(btnAdd);
    inputRowLayout->addWidget(btnColorPick);
    mainLayout->addLayout(inputRowLayout);

    //color palette
    paletteContainer = new QWidget(this);
    createPaletteGrid();
    paletteContainer->setVisible(false);
    mainLayout->addWidget(paletteContainer);

    tagList = new QListWidget(this);
    mainLayout->addWidget(tagList);

    btnBack = new QPushButton("back", this);
    mainLayout->addWidget(btnBack);

    connect(btnBack, &QPushButton::clicked, this, &TagWidget::tagViewClosed);

    connect(btnAdd, &QPushButton::clicked, this, &TagWidget::onAddClicked);
    connect(tagInput, &QLineEdit::returnPressed, this, &TagWidget::onAddClicked);

    connect(btnColorPick, &QPushButton::clicked, this, &TagWidget::openColorDialog);
}

void TagWidget::createPaletteGrid() {
    QGridLayout* grid = new QGridLayout(paletteContainer);
    grid->setContentsMargins(0, 5, 0, 5);
    grid->setSpacing(6);

    int row = 0;
    int col = 0;

    for (const QColor& color : defaultColors) {

        QPushButton* colorBtn = new QPushButton(paletteContainer);
        colorBtn->setFixedSize(24, 24);
        colorBtn->setCursor(Qt::PointingHandCursor);
        colorBtn->setStyleSheet(QString("QPushButton { background-color: %1; border: 1px solid #aaa; border-radius: 4px; }"
                                        "QPushButton:hover { border: 2px solid #333; }").arg(color.name()));

        connect(colorBtn, &QPushButton::clicked, this, [this, color]() {
            currentSelectedColor = color;

            btnColorPick->setStyleSheet(QString("QPushButton { background-color: %1; border: 2px solid #ccc; border-radius: 14px; }").arg(color.name()));

            paletteContainer->setVisible(false);
        });

        grid->addWidget(colorBtn, row, col);

        col++;
        if (col > 4) {
            col = 0;
            row++;
        }
    }
}

void TagWidget::openColorDialog() {
    paletteContainer->setVisible(!paletteContainer->isVisible());
}

void TagWidget::populateList() {
    tagList->clear();

    const std::vector<tag*>& allTags = tm.getTags();

    for (tag* t : allTags) {
        if (!t) continue;

        std::string tagName = t->getName();

        if (t == tm.getDefaultTag()) {
            QListWidgetItem* item = new QListWidgetItem(tagList);
            QWidget* rowWidget = new QWidget(this);
            QHBoxLayout* rowLayout = new QHBoxLayout(rowWidget);
            rowLayout->setContentsMargins(5, 10, 5, 12);

            QLabel* label = new QLabel(QString::fromStdString(tagName), this);
            QLabel* defaultLabel = new QLabel("(Default)", this);

            rowLayout->addWidget(label, 0, Qt::AlignVCenter);
            rowLayout->addStretch();
            rowLayout->addWidget(defaultLabel, 0, Qt::AlignVCenter);

            tagList->addItem(item);
            tagList->setItemWidget(item, rowWidget);
            item->setSizeHint(rowWidget->sizeHint());
        } else {
            QListWidgetItem* item = new QListWidgetItem(tagList);
            QWidget* rowWidget = new QWidget(this);
            QHBoxLayout* rowLayout = new QHBoxLayout(rowWidget);
            rowLayout->setContentsMargins(5, 10, 5, 12);

            QLabel* label = new QLabel(QString::fromStdString(tagName), this);
            label->setStyleSheet(QString("QLabel { color: %1; font-weight: bold; }").arg(t->getColor().name()));

            QPushButton* btnDelete = new QPushButton("delete", this);
            btnDelete->setFixedSize(60, 28);
            btnDelete->setCursor(Qt::PointingHandCursor);

            rowLayout->addWidget(label, 0, Qt::AlignVCenter);
            rowLayout->addStretch();
            rowLayout->addWidget(btnDelete, 0, Qt::AlignVCenter);

            tagList->addItem(item);
            tagList->setItemWidget(item, rowWidget);
            item->setSizeHint(rowWidget->sizeHint());

            connect(btnDelete, &QPushButton::clicked, this, [this, tagName]() {
                onDeleteTagClicked(tagName);
            });
        }
    }
}

void TagWidget::onAddClicked() {
    QString text = tagInput->text().trimmed();
    if (text.isEmpty()) return;

    std::string newTagName = text.toStdString();

    tm.newTag(newTagName, currentSelectedColor);

    tagInput->clear();

    populateList();
    emit tagsChanged();
}

void TagWidget::onDeleteTagClicked(const std::string& tagName) {
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Delete Tag",
                                  QString::fromStdString("delete " + tagName + "?"),
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        tm.removeTag(tagName, am);

        populateList();
        emit tagsChanged();
    }
}

void TagWidget::refresh() {
    if (tagInput) {
        tagInput->clear();
    }
    currentSelectedColor = Qt::gray;
    if (btnColorPick) {
        btnColorPick->setStyleSheet(QString("QPushButton { background-color: %1; border: 2px solid #ccc; border-radius: 14px; }").arg(currentSelectedColor.name()));
    }
    if (paletteContainer) {
        paletteContainer->setVisible(false);
    }
    populateList();
}