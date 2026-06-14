#include "Headers/UI/Forms/ProjectForm.h"
#include "Headers/project.h"
#include "Headers/task.h"
#include <QMessageBox>
#include <QLabel>

ProjectForm::ProjectForm(tagManager& tm, QWidget* parent)
    : TaskForm(tm, parent)
{
    setupSubtaskPanel();
    mainLayout->addStretch();
}

void ProjectForm::setupSubtaskPanel()
{
    //btn add subtask
    btnAddSubtask = new QPushButton("+ Add Subtask", this);
    mainLayout->addWidget(btnAddSubtask);

    //scrollable panel
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

    auto* form = new TaskForm(tm, container);
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
        subtaskLayout->removeWidget(container); // Scollega subito il widget dal layout grafico
        container->deleteLater();

        // Se non ci sono più subtask, nascondi il pannello dello scroll
        if (subtaskForms.isEmpty()) {
            subtaskPanel->setVisible(false);
            scroll->setVisible(false);
        }
    });
}

bool ProjectForm::validate()
{

    if (nameEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Required field", "Please enter a name for the project.");
        nameEdit->setFocus();
        return false;
    }

    const QDate projectDeadline  = getDeadlineDate();
    const QTime projectODeadline = getODeadlineTime();

    for (QList<TaskForm*>::const_iterator it = subtaskForms.cbegin(); it != subtaskForms.cend(); ++it) {
        if (!(*it)->validate()) return false;

        if ((*it)->getDeadlineDate() > projectDeadline) {
            QMessageBox::warning(this,"Invalid Date",
                                 "Subtask deadline cannot exceed project deadline.");
            (*it)->getDeadlineEdit()->setFocus();
            return false;
        }
        if ((*it)->getDeadlineDate() == projectDeadline) {
            if ((*it)->getODeadlineTime() > projectODeadline) {
                QMessageBox::warning(this,"Invalid Time",
                                     "Subtask deadline cannot exceed project deadline.");
                (*it)->getODeadlineEdit()->setFocus();
                return false;
            }
        }

    }
    return true;
}

AbstractActivity* ProjectForm::createActivity()
{
    const std::string name = nameEdit->text().trimmed().toStdString();
    const std::string desc = descEdit->text().trimmed().toStdString();
    const tag* t           = tagCombo->getSelectedTag();
    const QDate qd         = getDeadlineDate();
    const QTime qt         = getODeadlineTime();

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
    TaskForm::reset();

    //rm all subtasks conteiner
    for (QList<TaskForm*>::const_iterator it = subtaskForms.cbegin(); it != subtaskForms.cend(); ++it)
        (*it)->parentWidget()->deleteLater();

    subtaskForms.clear();
    subtaskPanel->setVisible(false);
    scroll->setVisible(false);
}
void ProjectForm::triggerAddSubtaskForm() {
    if (subtaskPanel && !subtaskPanel->isVisible()) {
        subtaskPanel->setVisible(true);
        if (scroll) scroll->setVisible(true);
    }
    addSubtaskForm();
}

void ProjectForm::loadFromActivity(AbstractActivity* act) {
    auto* p = dynamic_cast<project*>(act);
    if (!p) return;
    fillCommonFields(p);
    getDeadlineEdit()->setDate(QDate(p->getDeadline().getYear(), p->getDeadline().getMonth(), p->getDeadline().getDay()));
    getODeadlineEdit()->setTime(QTime(p->getODeadline().getOre(), p->getODeadline().getMin()));

    // Rimuoviamo eventuali subtask esistenti e ricarichiamo
    // Assicurati che reset() pulisca la UI prima
    for(auto* sub : p->getSubtasks()) {
        triggerAddSubtaskForm();
        getSubtaskForms().last()->loadFromActivity(sub);
    }
}

void ProjectForm::saveToActivity(AbstractActivity* act) {
    auto* p = dynamic_cast<project*>(act);
    if (!p) return;
    p->setName(nameEdit->text().toStdString());
    p->setDesc(descEdit->text().toStdString());
    p->setTag(tagCombo->getSelectedTag());
    p->setDeadline(date(getDeadlineEdit()->date().day(), getDeadlineEdit()->date().month(), getDeadlineEdit()->date().year()));
    p->setODeadline(HourMinute(getODeadlineEdit()->time().hour(), getODeadlineEdit()->time().minute()));

    while (p->size() > 0) {
        p->remove(static_cast<unsigned int>(0));
    }

    // Aggiungiamo i nuovi dati aggiornati
    for (int i = 0; i < subtaskForms.size(); ++i) {
        // Creiamo un task temporaneo
        task* tempTask = new task("", "", nullptr, date(1,1,2000), HourMinute(0,0), false);

        // Deleghiamo il salvataggio al form specifico
        subtaskForms[i]->saveToActivity(tempTask);

        // Aggiungiamo al progetto
        p->add(tempTask->getName(),
               tempTask->getDescription(),
               tempTask->getDeadline(),
               tempTask->getODeadline(),
               tempTask->isCompleted());

        delete tempTask;
    }
}