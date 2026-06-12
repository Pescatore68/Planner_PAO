#include "Headers/UI/ActivityDelete.h"
#include <QMessageBox>



ActivityDelete::ActivityDelete(ActivityManager& activityMgr, QObject* parent) : QObject(parent), am(activityMgr) {}
bool ActivityDelete::execute(AbstractActivity* act, QWidget* parent) {
    if (!act) return false;

    // 1. Mostra il pop-up di conferma
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(parent,
                                  "Conferma eliminazione",
                                  QString::fromStdString("Sei sicuro di voler eliminare definitivamente '" + act->getName() + "'?"),
                                  QMessageBox::Yes | QMessageBox::No);

    // 2. Se l'utente conferma, cerchiamo l'indice
    if (reply == QMessageBox::Yes) {
        int foundIdx = -1;

        // Cicliamo usando i metodi pubblici size() e get() del tuo manager
        for (unsigned int i = 0; i < am.size(); ++i) {
            if (am.get(i) == act) {
                foundIdx = static_cast<int>(i);
                break; // Trovato! Usciamo dal ciclo
            }
        }

        // 3. Se l'indice è valido, chiamiamo il tuo am.remove(unsigned int)
        if (foundIdx != -1) {
            am.remove(static_cast<unsigned int>(foundIdx));

            emit activityDeleted();

            return true; // Ritorna true alla View per avvisare di rinfrescare lo schermo
        }
    }

    return false; // Annullato o non trovato
}