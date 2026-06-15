#include "Headers/UI/Widgets/ActivityDelete.h"
#include <QMessageBox>



ActivityDelete::ActivityDelete(ActivityManager& activityMgr, QObject* parent) : QObject(parent), am(activityMgr) {}
bool ActivityDelete::execute(AbstractActivity* act, QWidget* parent) {
    if (!act) return false;


    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(parent, "Confirm Delete",QString::fromStdString("are you sure you want to delete" + act->getName() + "'?"), QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        int foundIdx = -1;

        unsigned int i = 0;
        bool found = false;
        while (i < am.size() && !found) {
            if (am.get(i) == act) {
                foundIdx = static_cast<int>(i);
                found = true;
            } else {
                ++i;
            }
        }

        if (foundIdx != -1) {
            am.remove(static_cast<unsigned int>(foundIdx));

            emit activityDeleted();

            return true;
        }
    }

    return false;
}