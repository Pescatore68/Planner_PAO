#include "Headers/UI/ActivityModify.h"
#include "Headers/Visitor/FormSelectVisitor.h"

#include <QHBoxLayout>

ActivityModify::ActivityModify(AbstractActivity* act, tagManager& tm, QWidget* parent)
    : QWidget(parent), activeForm(nullptr), activity(act)
{

    mainLayout = new QVBoxLayout(this);

    FormSelectVisitor selector(tm, nullptr);
    activity->accept(selector);
    activeForm = selector.chosenForm;

    if (activeForm) {
        activeForm->setParent(this);
        activeForm->loadFromActivity(activity);
        mainLayout->addWidget(activeForm);
    }


    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnCancel = new QPushButton("Cancel", this);
    btnSave = new QPushButton("Save", this);

    btnLayout->addStretch();
    btnLayout->addWidget(btnCancel);
    btnLayout->addWidget(btnSave);
    mainLayout->addLayout(btnLayout);

    connect(btnSave, &QPushButton::clicked, this, [this]() {
        if (activeForm && activeForm->validate()) {

            activeForm->saveToActivity(activity);
            emit modificationFinished();
        }
    });

    connect(btnCancel, &QPushButton::clicked, this, &ActivityModify::modificationFinished);
}

ActivityModify::~ActivityModify() {
        if (activeForm) {
        delete activeForm;
        activeForm = nullptr;
    }
}