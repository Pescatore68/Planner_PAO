#ifndef ACTIVITYFORM_H
#define ACTIVITYFORM_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include "Headers/tagManager.h"
#include "Headers/AbstractActivity.h"
#include "Headers/UI/TagComboBox.h"

class ActivityForm : public QWidget {
    Q_OBJECT

protected:
    QVBoxLayout* mainLayout;

    QLineEdit*   nameEdit;
    QLineEdit*   descEdit;
    TagComboBox* tagCombo;

    // Aggiunge le righe nome/descrizione/tag al mainLayout.
    // Ogni riga è un QHBoxLayout con una QLabel fissa a sinistra (100px)
    // e il widget di input a destra — stesso pattern visivo di TaskWidget.
    void buildCommonFields(tagManager& tm);

    // Helper: crea una riga label + widget e la appende a mainLayout
    void addRow(const QString& labelText, QWidget* field);
    void addTimeRow(const QString& labelText, QWidget* dateField, QWidget* timeField);

public:
    explicit ActivityForm(QWidget* parent = nullptr);
    virtual ~ActivityForm() = default;
    QVBoxLayout* getMainLayout();

    virtual AbstractActivity* createActivity() = 0;
    virtual bool validate() = 0;
    virtual void reset();
    virtual void refreshTags();
};

#endif // ACTIVITYFORM_H
