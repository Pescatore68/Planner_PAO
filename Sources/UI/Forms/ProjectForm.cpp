#include "Headers/UI/Forms/ProjectForm.h"
#include "Headers/project.h"
#include "Headers/task.h"
#include <QMessageBox>
#include <QLabel>

ProjectForm::ProjectForm(tagManager& tm, QWidget* parent)
    : ActivityForm(parent), tmPtr(&tm)
{
    buildCommonFields(tm);

    deadlineEdit = new QDateEdit(QDate::currentDate(), this);
    deadlineEdit->setCalendarPopup(false);
    deadlineEdit->setDisplayFormat("dd/MM/yyyy");

    oDeadlineEdit = new QTimeEdit(QTime(23, 59), this);
    oDeadlineEdit->setDisplayFormat("HH:mm");

    addTimeRow("Deadline", deadlineEdit, oDeadlineEdit);

    setupSubtaskPanel();

    mainLayout->addStretch();
}

void ProjectForm::setupSubtaskPanel()
{
    // bottone per mostrare/aggiungere subtask
    btnAddSubtask = new QPushButton("+ Aggiungi Subtask", this);
    mainLayout->addWidget(btnAddSubtask);

    // pannello scrollabile che contiene i TaskForm
    subtaskPanel = new QFrame(this);
    subtaskPanel->setFrameShape(QFrame::StyledPanel);
    subtaskPanel->setVisible(false);

    subtaskLayout = new QVBoxLayout(subtaskPanel);
    subtaskLayout->setAlignment(Qt::AlignTop);
    subtaskPanel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    subtaskLayout->setSpacing(6);
    subtaskLayout->setContentsMargins(8, 8, 8, 8);
    subtaskLayout->setAlignment(Qt::AlignTop | Qt::AlignHCenter);

    scroll = new QScrollArea(this);
    scroll->setWidget(subtaskPanel);
    scroll->setWidgetResizable(true);
    mainLayout->setStretchFactor(scroll, 1);
    scroll->setVisible(false);
    scroll->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scroll->setStyleSheet(R"(
        QScrollArea {
            border: none;
            background: transparent;
        }
        QScrollArea > QWidget > QWidget {
            background: transparent;
        }
        QScrollBar:vertical {
            width: 3px;
            background: transparent;
            margin: 0px;
        }
        QScrollBar::handle:vertical {
            background: #aaaaaa;
            border-radius: 1px;
            min-height: 20px;
        }
        QScrollBar::add-line:vertical,
        QScrollBar::sub-line:vertical,
        QScrollBar::add-page:vertical,
        QScrollBar::sub-page:vertical {
            background: transparent;
            height: 0px;
        }
    )");
    scroll->setFrameShape(QFrame::NoFrame);

    mainLayout->addWidget(scroll, 1);

    connect(btnAddSubtask, &QPushButton::clicked, this, [this]{
        if (!subtaskPanel->isVisible()) {
            subtaskPanel->setVisible(true);
            scroll->setVisible(true);
        }
        addSubtaskForm();
    });
}

void ProjectForm::addSubtaskForm()
{
    auto* container = new QFrame(subtaskPanel);
    container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    auto* vl = new QVBoxLayout(container);
    vl->setContentsMargins(4, 4, 4, 4);
    vl->setSpacing(2);
    vl->setAlignment(Qt::AlignTop | Qt::AlignHCenter);

    // header
    auto* header = new QHBoxLayout();
    auto* lbl    = new QLabel(QString("Subtask %1").arg(subtaskForms.size() + 1), container);
    auto* btnRem = new QPushButton("✕", container);
    btnRem->setFixedSize(24, 24);
    header->addWidget(lbl);
    header->addStretch();
    header->addWidget(btnRem);
    vl->addLayout(header);

    auto* form = new TaskForm(*tmPtr, container);
    form->setMaximumWidth(225);
    form->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    form->adjustSize();
    form->setFixedHeight(form->sizeHint().height());
    form->getMainLayout()->removeItem(form->getMainLayout()->itemAt(form->getMainLayout()->count() - 1));
    form->getMainLayout()->setContentsMargins(0, 0, 0, 15);
    form->getMainLayout()->setSpacing(2);
    subtaskForms.append(form);
    vl->addWidget(form);

    subtaskLayout->addWidget(container);
    container->adjustSize();
    container->setFixedHeight(container->sizeHint().height());


    connect(btnRem, &QPushButton::clicked, this, [this, container, form]{
        subtaskForms.removeOne(form);
        container->deleteLater();
    });
}

bool ProjectForm::validate()
{
    if (nameEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Campo obbligatorio", "Inserisci un nome per il progetto.");
        nameEdit->setFocus();
        return false;
    }

    const QDate projectDeadline = deadlineEdit->date();

    for (QList<TaskForm*>::const_iterator it = subtaskForms.cbegin(); it != subtaskForms.cend(); ++it) {
        if (!(*it)->validate()) return false;

        if ((*it)->getDeadlineDate() > projectDeadline) {
            QMessageBox::warning(this, "Data non valida",
                                 "La scadenza di una subtask non può superare quella del progetto.");
            return false;
        }
    }
    return true;
}

AbstractActivity* ProjectForm::createActivity()
{
    const std::string name = nameEdit->text().trimmed().toStdString();
    const std::string desc = descEdit->text().trimmed().toStdString();
    const tag* t           = tagCombo->getSelectedTag();
    const QDate qd         = deadlineEdit->date();
    const QTime qt         = oDeadlineEdit->time();

    auto* p = new project(
        name, desc, t,
        date(qd.day(), qd.month(), qd.year()),
        HourMinute(qt.hour(), qt.minute())
        );

    for (QList<TaskForm*>::const_iterator it = subtaskForms.cbegin(); it != subtaskForms.cend(); ++it) {
        if (!(*it)->validate()) continue;
        auto* sub = static_cast<task*>((*it)->createActivity());
        p->add(sub->getName(), sub->getDescription(),
               sub->getDeadline(), sub->getODeadline());
        delete sub;
    }

    return p;
}

void ProjectForm::reset() {
    ActivityForm::reset();
    deadlineEdit->setDate(QDate::currentDate());
    oDeadlineEdit->setTime(QTime(23, 59));

    // rimuovi tutti i container delle subtask
    for (QList<TaskForm*>::const_iterator it = subtaskForms.cbegin(); it != subtaskForms.cend(); ++it)
        (*it)->parentWidget()->deleteLater(); // elimina il container

    subtaskForms.clear();
    subtaskPanel->setVisible(false);
    scroll->setVisible(false);
}