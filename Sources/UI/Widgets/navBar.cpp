#include "Headers/UI/Widgets/navBar.h"

navBar::navBar(tagManager &tm, QWidget* parent) : QWidget(parent), tm(tm) {
    setup();
}

void navBar::setup() {
    setFixedWidth(300);
    layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignTop);
    layout->setSpacing(10);
    layout->setContentsMargins(10, 20, 10, 20);


    QHBoxLayout* searchRow = new QHBoxLayout();
    btnCalendar = new QPushButton("Calendar", this);
    btnSearch = new QPushButton("Search", this);
    btnFilter = new QPushButton("Filter", this);
    filterCombo = new TagComboBox(tm, this); // Usa il tm passato
    filterCombo->hide();
    btnTags = new QPushButton("Tag", this);
    btnAdd = new QPushButton("+ add", this);

    searchRow->addWidget(btnSearch);
    searchRow->addWidget(btnFilter);

    layout->addWidget(btnCalendar);
    layout->addLayout(searchRow);
    searchEdit = new QLineEdit(this);
    layout->addWidget(searchEdit);
    layout->addWidget(btnFilter);
    layout->addWidget(filterCombo);
    searchEdit->setPlaceholderText("Search Activity");
    searchEdit->hide();

    layout->addWidget(filterCombo);
    layout->addWidget(btnTags);
    layout->addStretch();
    layout->addWidget(btnAdd);

    connect(btnSearch, &QPushButton::clicked, this, [this](){
        searchEdit->setVisible(searchEdit->isHidden());
    });
    connect(btnCalendar,    &QPushButton::clicked, this, &navBar::calendarClicked);
    connect(btnTags,        &QPushButton::clicked, this, &navBar::tagsClicked);
    connect(btnAdd,         &QPushButton::clicked, this, &navBar::addClicked);
    connect(searchEdit, &QLineEdit::textChanged, this, &navBar::searchTextChanged);

    connect(btnFilter, &QPushButton::clicked, this, [this](){
        filterCombo->setVisible(filterCombo->isHidden());
    });
}