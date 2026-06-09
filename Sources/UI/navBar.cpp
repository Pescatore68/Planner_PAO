#include "Headers/UI/navBar.h"

navBar::navBar(QWidget* parent) : QWidget(parent) {
    setup();
}

void navBar::setup() {
    setFixedWidth(200);
    layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignTop);
    layout->setSpacing(10);
    layout->setContentsMargins(10, 20, 10, 20);

    btnCalendar = new QPushButton("Calendar", this);
    btnTaskProject = new QPushButton("Task/Project", this);
    btnSearch = new QPushButton("Search", this);
    btnTags = new QPushButton("Tag", this);
    btnAdd = new QPushButton("+ add", this);

    layout->addWidget(btnCalendar);
    layout->addWidget(btnTaskProject);
    layout->addWidget(btnSearch);
    layout->addWidget(btnTags);
    layout->addStretch();
    layout->addWidget(btnAdd);

    connect(btnCalendar,    &QPushButton::clicked, this, &navBar::calendarClicked);
    connect(btnTaskProject, &QPushButton::clicked, this, &navBar::taskProjectClicked);
    connect(btnSearch,      &QPushButton::clicked, this, &navBar::searchClicked);
    connect(btnTags,        &QPushButton::clicked, this, &navBar::tagsClicked);
    connect(btnAdd,         &QPushButton::clicked, this, &navBar::addClicked);
}