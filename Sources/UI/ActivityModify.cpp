#include "Headers/UI/ActivityModify.h"
#include "Headers/Visitor/FormSelectVisitor.h" // Il visitor che fa il "new" del form corretto

#include <QHBoxLayout>

ActivityModify::ActivityModify(AbstractActivity* act, tagManager& tm, QWidget* parent)
    : QWidget(parent), activeForm(nullptr), activity(act) // Inizializza sempre a nullptr!
{
    if (!activity) {
        qDebug() << "CRASH PREVENTED: activity è NULL!";
        return; // Non fare NIENTE
    }
    mainLayout = new QVBoxLayout(this);

    // Il FormSelectVisitor crea il form corretto (questo lo teniamo!)
    FormSelectVisitor selector(tm, nullptr); // Passa nullptr invece di 'this'
    activity->accept(selector);
    activeForm = selector.chosenForm;

    if (activeForm) {
        activeForm->setParent(this); // Lo colleghi DOPO che è stato creato
        activeForm->loadFromActivity(activity);
        mainLayout->addWidget(activeForm);
    }


    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnCancel = new QPushButton("Cancel", this);
    btnSave = new QPushButton("Save", this);
    btnSave->setStyleSheet("font-weight: bold; background-color: #0275d8; color: white; padding: 5px; border-radius: 4px;");

    btnLayout->addStretch();
    btnLayout->addWidget(btnCancel);
    btnLayout->addWidget(btnSave);
    mainLayout->addLayout(btnLayout);

    connect(btnSave, &QPushButton::clicked, this, [this]() {
        if (activeForm && activeForm->validate()) {
            // Il salvataggio ora è diretto e sicuro
            activeForm->saveToActivity(activity);
            emit modificationFinished();
        }
    });

    connect(btnCancel, &QPushButton::clicked, this, &ActivityModify::modificationFinished);
}

ActivityModify::~ActivityModify() {
    // Se activeForm è stato creato (new), lo eliminiamo correttamente
    if (activeForm) {
        delete activeForm;
        activeForm = nullptr;
    }
}