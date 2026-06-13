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
    tagManager&  tm;

    //add name, descripton, tag to mainlayout
    void buildCommonFields();

    void addRow(QWidget* field, const QString& labelText = QString());
    void addTimeRow(const QString& labelText, QWidget* dateField, QWidget* timeField);

public:
    explicit ActivityForm(tagManager& tm, QWidget* parent = nullptr);
    virtual ~ActivityForm() = default;
    QVBoxLayout* getMainLayout();

    virtual AbstractActivity* createActivity() = 0;
    virtual bool validate() = 0;
    virtual void reset();
};

#endif // ACTIVITYFORM_H
