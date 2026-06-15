#include "Headers/UI/TaskWidget.h"
#include "Headers/AbstractActivity.h"
#include "Headers/task.h"
#include "Headers/project.h"
#include "Headers/Visitor/TaskWidgetVisitor.h"

#include <QHeaderView>
#include <QMessageBox>

TaskWidget::TaskWidget(ActivityManager& am, QWidget* parent)
    : QWidget(parent), am(am), activityDelete(am)
{
    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(8, 8, 8, 8);
    mainLayout->setSpacing(6);

    toolbarLayout = new QHBoxLayout();
    btnDelete     = new QPushButton("Elimina", this);
    btnDelete->setEnabled(false);

    toolbarLayout->addStretch();
    toolbarLayout->addWidget(btnDelete);

    tree = new QTreeWidget(this);
    tree->setColumnCount(2);
    tree->setHeaderLabels({"Name", "Deadline"});
    tree->header()->setStretchLastSection(false);
    tree->header()->setSectionResizeMode(0, QHeaderView::Stretch);
    tree->header()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    tree->setSelectionMode(QAbstractItemView::SingleSelection);
    tree->setAnimated(true);

    tree->setStyleSheet(
        "QTreeView { "
        "  font-family: 'SF Pro Text', '-apple-system', 'BlinkMacSystemFont', 'Inter', sans-serif; "
        "  font-size: 14px; "
        "  color: #1c1c1e; "
        "}"
        "QHeaderView::section { "
        "  font-family: 'SF Pro Text', '-apple-system', 'BlinkMacSystemFont', 'Inter', sans-serif; "
        "  font-size: 12px; "
        "  font-weight: 600; "
        "  color: #8e8e93; "
        "  background-color: transparent; "
        "  border: none; "
        "}"
        );

    tree->setItemDelegateForColumn(0, new ProgressBarDelegate(tree));

    mainLayout->addLayout(toolbarLayout);
    mainLayout->addWidget(tree);

    connect(btnDelete, &QPushButton::clicked,
            this, &TaskWidget::onDeleteClicked);
    connect(tree, &QTreeWidget::itemClicked,
            this, &TaskWidget::onItemClicked);
    connect(tree, &QTreeWidget::itemChanged,
            this, &TaskWidget::onItemChanged);

    connect(tree, &QTreeWidget::itemDoubleClicked,
            this, &TaskWidget::onItemDoubleClicked);

    buildTree();
}

void TaskWidget::refresh()
{
    tree->blockSignals(true);
    buildTree();
    tree->blockSignals(false);

    current = nullptr;
    btnDelete->setEnabled(false);
}

void TaskWidget::buildTree()
{
    tree->clear();

    auto* rootTasks    = new QTreeWidgetItem(tree, {"Task",    ""});
    auto* rootProjects = new QTreeWidgetItem(tree, {"Project", ""});

    QFont boldFont = rootTasks->font(0);
    boldFont.setBold(true);
    rootTasks->setFont(0, boldFont);
    rootProjects->setFont(0, boldFont);

    //root not selectable
    rootTasks->setFlags(Qt::ItemIsEnabled);
    rootProjects->setFlags(Qt::ItemIsEnabled);

    TreeBuilderVisitor visitor(rootTasks, rootProjects);
    for (unsigned int i = 0; i < am.size(); ++i) {
        AbstractActivity* a = am.get(i);
        if (a) a->accept(visitor);
    }

    rootTasks->setExpanded(true);
    rootProjects->setExpanded(true);
}

AbstractActivity* TaskWidget::activityFromItem(QTreeWidgetItem* item) const
{
    if (!item) return nullptr;
    QVariant v = item->data(0, TaskWidgetRoles::ActivityPtrRole);
    if (!v.isValid()) return nullptr;
    return static_cast<AbstractActivity*>(v.value<void*>());
}

void TaskWidget::onItemClicked(QTreeWidgetItem* item, int)
{
    AbstractActivity* a = activityFromItem(item);
    current = a;
    btnDelete->setEnabled(a != nullptr);
    if (a) emit activitySelected(a);
}


void TaskWidget::onItemDoubleClicked(QTreeWidgetItem* item, int)
{
    AbstractActivity* a = activityFromItem(item);
    if (!a) return;

    QVariant vParent = item->data(0, TaskWidgetRoles::ParentProjectRole);
    if (vParent.isValid()) {
        AbstractActivity* parentProject = static_cast<AbstractActivity*>(vParent.value<void*>());
        if (parentProject) {
            emit activityDoubleClicked(parentProject);
            return;
        }
    }

    emit activityDoubleClicked(a);
}

void TaskWidget::onItemChanged(QTreeWidgetItem* item, int column)
{
    if (column != 0) return;
    AbstractActivity* a = activityFromItem(item);
    if (!a) return;

    ItemChangedVisitor visitor(item);
    a->accept(visitor);
}

void TaskWidget::onDeleteClicked()
{
    if (!current) return;

    AbstractActivity* toDelete = current;
    if (activityDelete.execute(toDelete, this)) {
        current = nullptr;
        btnDelete->setEnabled(false);
        emit deleteRequested(toDelete);
        this->refresh();
    }
}

void TaskWidget::handleExternalActivitySelected(AbstractActivity* a) {
    current = a;
    btnDelete->setEnabled(a != nullptr);

    if (a) {
        tree->clearSelection();
    }
}

void TaskWidget::setDeleteEnabled(bool enabled) {
    btnDelete->setEnabled(enabled && current != nullptr);
}