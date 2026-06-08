#include "Headers/TaskWidget.h"
#include "Headers/AbstractActivity.h"
#include "Headers/task.h"
#include "Headers/project.h"

TaskWidget::TaskWidget(QWidget* parent) : QWidget(parent) {
    mainLayout   = new QVBoxLayout(this);
    headerLayout = new QHBoxLayout();

    labelType        = new QLabel(this);
    labelTitle       = new QLabel(this);
    labelDescription = new QLabel(this);
    labelDeadline    = new QLabel(this);
    progressBar      = new QProgressBar(this);
    btnComplete      = new QPushButton("Completa",   this);
    btnDelete        = new QPushButton("Elimina",    this);
    btnBack          = new QPushButton("← Indietro", this);

    headerLayout->addWidget(labelType);
    headerLayout->addWidget(labelTitle);

    mainLayout->addWidget(btnBack);
    mainLayout->addLayout(headerLayout);
    mainLayout->addWidget(labelDescription);
    mainLayout->addWidget(labelDeadline);
    mainLayout->addWidget(progressBar);
    mainLayout->addWidget(btnComplete);
    mainLayout->addWidget(btnDelete);

    progressBar->setVisible(false);
    labelDeadline->setVisible(false);
    btnComplete->setVisible(false);

    setLayout(mainLayout);

    // BUG FIX 13: rimosso connect(btnBack) duplicato (era presente due volte)
    connect(btnBack,     &QPushButton::clicked, this, &TaskWidget::backRequested);
    connect(btnComplete, &QPushButton::clicked, this, &TaskWidget::onCompleteClicked);
    connect(btnDelete,   &QPushButton::clicked, this, &TaskWidget::onDeleteClicked);
}

void TaskWidget::setActivity(AbstractActivity* a) {
    current = a;
    if (!a) return;

    // BUG FIX 15: getName() restituisce il nome dell'attività (non il tipo).
    // labelType mostra il nome, labelTitle la descrizione breve (invertiti nell'originale).
    // Riassegnati in modo semanticamente corretto:
    //   labelType  → nome dell'attività
    //   labelTitle → (lasciato per uso futuro con getType(), ora vuoto)
    // BUG FIX 14: labelDescription ora viene effettivamente popolato
    labelType->setText(QString::fromStdString(a->getName()));
    labelTitle->setText("");  // placeholder: da riempire con getType() quando implementato
    labelDescription->setText(QString::fromStdString(a->getDescription()));

    if (task* t = dynamic_cast<task*>(a)) {
        labelDeadline->setVisible(true);
        labelDeadline->setText(
            QString::fromStdString(t->getDeadline().toString())
            );

        btnComplete->setVisible(true);
        btnComplete->setText(t->isCompleted() ? "✓ Completato" : "Completa");

        if (project* p = dynamic_cast<project*>(t)) {
            progressBar->setVisible(true);
            progressBar->setValue(static_cast<int>(p->completionPercentage()));
        } else {
            progressBar->setVisible(false);
        }
    } else {
        labelDeadline->setVisible(false);
        btnComplete->setVisible(false);
        progressBar->setVisible(false);
    }
}

void TaskWidget::onCompleteClicked() {
    if (!current) return;
    if (task* t = dynamic_cast<task*>(current)) {
        t->setCompleted(true);
        btnComplete->setText("✓ Completato");

        // Aggiorna progress bar se è un project
        if (project* p = dynamic_cast<project*>(t)) {
            progressBar->setValue(static_cast<int>(p->completionPercentage()));
        }
    }
}

void TaskWidget::onDeleteClicked() {
    if (!current) return;
    // BUG FIX 4 (TaskWidget): emette deleteRequested con il puntatore all'activity,
    // così MainWindow sa cosa eliminare prima di tornare alla lista.
    AbstractActivity* toDelete = current;
    current = nullptr;
    emit deleteRequested(toDelete);
    emit backRequested();
}