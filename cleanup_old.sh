#!/usr/bin/env bash
# =============================================================================
# cleanup_old.sh
# Rimuove i file nelle vecchie posizioni DOPO aver verificato che il build
# con la nuova struttura funziona correttamente.
#
# Uso: cd <root-progetto> && bash cleanup_old.sh
# ATTENZIONE: esegui SOLO dopo un rebuild completo senza errori!
# =============================================================================
set -euo pipefail

ROOT="$(pwd)"

echo "Questo script rimuove i file nelle vecchie posizioni."
echo "Eseguilo SOLO dopo aver verificato che il build compila senza errori."
echo ""
read -rp "Sei sicuro di voler procedere? [s/N] " answer
if [[ "$answer" != "s" && "$answer" != "S" ]]; then
  echo "Operazione annullata."
  exit 0
fi

echo ""
echo "[1] Rimozione vecchi header Model (ora in Headers/Model/)..."
OLD_MODEL_HEADERS=(
  "Headers/AbstractActivity.h"
  "Headers/ActivityManager.h"
  "Headers/HourMinute.h"
  "Headers/date.h"
  "Headers/event.h"
  "Headers/project.h"
  "Headers/reminder.h"
  "Headers/routine.h"
  "Headers/tag.h"
  "Headers/tagManager.h"
  "Headers/task.h"
  "Headers/config.h"
  "Headers/rec_ev.h"
  "Headers/JSONformat.h"
  "Headers/xmlFormat.h"
  "Headers/xmlFormat.cpp"
)
for f in "${OLD_MODEL_HEADERS[@]}"; do
  [ -f "$ROOT/$f" ] && rm "$ROOT/$f" && echo "  rm $f" || true
done

echo "[2] Rimozione vecchia cartella Headers/Dialog/..."
rm -rf "$ROOT/Headers/Dialog" && echo "  rm -rf Headers/Dialog" || true

echo "[3] Rimozione vecchi UI flat headers (ora in Headers/UI/Widgets/)..."
OLD_UI_HEADERS=(
  "Headers/UI/DayWidget.h"
  "Headers/UI/TaskWidget.h"
  "Headers/UI/TagWidget.h"
  "Headers/UI/TagComboBox.h"
  "Headers/UI/calendar.h"
  "Headers/UI/month.h"
  "Headers/UI/navBar.h"
  "Headers/UI/ActivityDelete.h"
  "Headers/UI/ActivityModel.h"
  "Headers/UI/ActivityModify.h"
  "Headers/UI/ActivitySearch.h"
)
for f in "${OLD_UI_HEADERS[@]}"; do
  [ -f "$ROOT/$f" ] && rm "$ROOT/$f" && echo "  rm $f" || true
done

echo "[4] Rimozione vecchi Sources..."
OLD_SOURCES=(
  "Sources/AbstractActivity.cpp"
  "Sources/ActivityManager.cpp"
  "Sources/HourMinute.cpp"
  "Sources/date.cpp"
  "Sources/event.cpp"
  "Sources/project.cpp"
  "Sources/reminder.cpp"
  "Sources/Routine.cpp"
  "Sources/tag.cpp"
  "Sources/tagManager.cpp"
  "Sources/task.cpp"
  "Sources/JSONformat.cpp"
  "Sources/xmlFormat.cpp"
  "Sources/UI/DayWidget.cpp"
  "Sources/UI/TaskWidget.cpp"
  "Sources/UI/TagWidget.cpp"
  "Sources/UI/TagComboBox.cpp"
  "Sources/UI/calendar.cpp"
  "Sources/UI/month.cpp"
  "Sources/UI/navBar.cpp"
  "Sources/UI/ActivityDelete.cpp"
  "Sources/UI/ActivityModify.cpp"
  "Sources/UI/ActivitySearch.cpp"
)
for f in "${OLD_SOURCES[@]}"; do
  [ -f "$ROOT/$f" ] && rm "$ROOT/$f" && echo "  rm $f" || true
done

echo "[5] Rimozione vecchia cartella Sources/Dialog/..."
rm -rf "$ROOT/Sources/Dialog" && echo "  rm -rf Sources/Dialog" || true

echo ""
echo "Cleanup completato."
