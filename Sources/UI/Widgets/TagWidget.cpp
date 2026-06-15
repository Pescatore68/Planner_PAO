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
    titleLabel->setStyleSheet("QLabel { font-size: 18px; font-weight: bold; color: #333; }");
    mainLayout->addWidget(titleLabel);

    QHBoxLayout* inputRowLayout = new QHBoxLayout();

    tagInput = new QLineEdit(this);
    tagInput->setPlaceholderText("New tag");
    tagInput->setStyleSheet("QLineEdit { padding: 6px; font-size: 13px; }");

    // Pulsante circolare (Anteprima del colore attuale)
    btnColorPick = new QPushButton(this);
    btnColorPick->setFixedSize(28, 28);
    btnColorPick->setCursor(Qt::PointingHandCursor);
    btnColorPick->setStyleSheet(QString("QPushButton { background-color: %1; border: 2px solid #ccc; border-radius: 14px; }").arg(currentSelectedColor.name()));


    btnAdd = new QPushButton("add", this);
    btnAdd->setStyleSheet("QPushButton { padding: 6px 12px; font-weight: bold; background-color: #5cb85c; color: white; border-radius: 4px; }");

    inputRowLayout->addWidget(tagInput);
    inputRowLayout->addWidget(btnAdd);
    inputRowLayout->addWidget(btnColorPick);
    mainLayout->addLayout(inputRowLayout);

    //tavolozza colori
    paletteContainer = new QWidget(this);
    createPaletteGrid();
    paletteContainer->setVisible(false); // Nascosta/Chiusa di default!
    mainLayout->addWidget(paletteContainer);

    tagList = new QListWidget(this);
    tagList->setStyleSheet("QListWidget { border: 1px solid #ccc; border-radius: 4px; }");
    mainLayout->addWidget(tagList);

    btnBack = new QPushButton("back", this);
    btnBack->setStyleSheet("QPushButton { padding: 6px; font-weight: bold; background-color: #777; color: white; border-radius: 4px; }");
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

    for (Qt::GlobalColor globalColor : defaultColors) {
        QColor color(globalColor);

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
            QLabel* label = new QLabel(QString::fromStdString(tagName) + " (Default)", this);
            label->setStyleSheet("QLabel { color: #888; font-style: italic; padding: 7px; }");
            tagList->addItem(item);
            tagList->setItemWidget(item, label);
            item->setSizeHint(label->sizeHint());
        }
        else {

            QListWidgetItem* item = new QListWidgetItem(tagList);
            QWidget* rowWidget = new QWidget(this);
            QHBoxLayout* rowLayout = new QHBoxLayout(rowWidget);
            rowLayout->setContentsMargins(5, 2, 5, 2);

            QLabel* label = new QLabel(QString::fromStdString(tagName), this);

            label->setStyleSheet(QString("QLabel { color: %1; font-weight: bold; }").arg(t->getColor().name()));

            QPushButton* btnDelete = new QPushButton("delete", this);
            btnDelete->setFixedSize(60, 28);
            btnDelete->setCursor(Qt::PointingHandCursor);
            btnDelete->setStyleSheet("QPushButton { color: #d9534f; background: transparent; font-size: 14px; }"
                                     "QPushButton:hover { background-color: #f2dede; border-radius: 4px; }");

            rowLayout->addWidget(label);
            rowLayout->addStretch();
            rowLayout->addWidget(btnDelete);

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

    // Inseriamo nel backend usando il colore associato al cerchietto
    tm.newTag(newTagName, currentSelectedColor);

    // Reset del campo di testo
    tagInput->clear();

    // Ripopoliamo la lista e aggiorniamo i menu a tendina dell'app
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