#!/usr/bin/env bash
# =============================================================================
# restructure.sh
# Ristruttura il progetto Qt secondo la nuova organizzazione:
#
#  Headers/Model/            ← gerarchia attività, date, ore, tag, tagManager
#  Headers/Model/Persistence ← JSONformat, xmlFormat
#  Headers/UI/Forms/         ← invariato
#  Headers/UI/Widgets/       ← tutte le QWidget (DayWidget, TaskWidget, ecc.)
#                               AddDialog → AddWidget (file rinominato)
#  Sources/ speculare
#
# Uso: cd <root-progetto> && bash restructure.sh
# Il progetto .pro viene aggiornato automaticamente.
# =============================================================================
set -euo pipefail

ROOT="$(pwd)"
PRO_FILE="$ROOT/Planner_PAO.pro"

# ─── 1. CREA DIRECTORY ───────────────────────────────────────────────────────
echo "[1] Creazione directory..."

mkdir -p Headers/Model/Persistence
mkdir -p Headers/UI/Widgets
mkdir -p Sources/Model/Persistence
mkdir -p Sources/UI/Widgets

# ─── 2. MAPPA SPOSTAMENTI ────────────────────────────────────────────────────
# Formato: "SORGENTE|DESTINAZIONE"
# (percorsi relativi dalla root del progetto)

declare -a MOVES=(
  # ── Model: gerarchia attività ────────────────────────────────────────────
  "Headers/AbstractActivity.h|Headers/Model/AbstractActivity.h"
  "Headers/ActivityManager.h|Headers/Model/ActivityManager.h"
  "Headers/HourMinute.h|Headers/Model/HourMinute.h"
  "Headers/date.h|Headers/Model/date.h"
  "Headers/event.h|Headers/Model/event.h"
  "Headers/project.h|Headers/Model/project.h"
  "Headers/reminder.h|Headers/Model/reminder.h"
  "Headers/routine.h|Headers/Model/routine.h"
  "Headers/tag.h|Headers/Model/tag.h"
  "Headers/tagManager.h|Headers/Model/tagManager.h"
  "Headers/task.h|Headers/Model/task.h"
  "Headers/config.h|Headers/Model/config.h"
  "Headers/rec_ev.h|Headers/Model/rec_ev.h"

  # ── Model/Persistence ────────────────────────────────────────────────────
  "Headers/JSONformat.h|Headers/Model/Persistence/JSONformat.h"
  "Headers/xmlFormat.h|Headers/Model/Persistence/xmlFormat.h"
  "Headers/xmlFormat.cpp|Headers/Model/Persistence/xmlFormat_stub.cpp"  # file vuoto, tenuto per traccia

  # ── Visitor: rimane in Headers/Visitor (invariato) ───────────────────────
  # (nessuno spostamento per i Visitor)

  # ── UI/Widgets: tutte le QWidget che non sono Forms ──────────────────────
  "Headers/UI/DayWidget.h|Headers/UI/Widgets/DayWidget.h"
  "Headers/UI/TaskWidget.h|Headers/UI/Widgets/TaskWidget.h"
  "Headers/UI/TagWidget.h|Headers/UI/Widgets/TagWidget.h"
  "Headers/UI/TagComboBox.h|Headers/UI/Widgets/TagComboBox.h"
  "Headers/UI/calendar.h|Headers/UI/Widgets/calendar.h"
  "Headers/UI/month.h|Headers/UI/Widgets/month.h"
  "Headers/UI/navBar.h|Headers/UI/Widgets/navBar.h"
  "Headers/UI/ActivityDelete.h|Headers/UI/Widgets/ActivityDelete.h"
  "Headers/UI/ActivityModel.h|Headers/UI/Widgets/ActivityModel.h"
  "Headers/UI/ActivityModify.h|Headers/UI/Widgets/ActivityModify.h"
  "Headers/UI/ActivitySearch.h|Headers/UI/Widgets/ActivitySearch.h"

  # ── AddDialog → AddWidget (rinomina + sposta in Widgets) ─────────────────
  "Headers/Dialog/AddDialog.h|Headers/UI/Widgets/AddWidget.h"
  # Dialog/ProjectDialog e Dialog/TaskDialog: rimangono o si spostano?
  # Per coerenza li spostiamo in Widgets (sono dialog ausiliari della UI)
  "Headers/Dialog/ProjectDialog.h|Headers/UI/Widgets/ProjectDialog.h"
  "Headers/Dialog/TaskDialog.h|Headers/UI/Widgets/TaskDialog.h"

  # ── Sources: Model ───────────────────────────────────────────────────────
  "Sources/AbstractActivity.cpp|Sources/Model/AbstractActivity.cpp"
  "Sources/ActivityManager.cpp|Sources/Model/ActivityManager.cpp"
  "Sources/HourMinute.cpp|Sources/Model/HourMinute.cpp"
  "Sources/date.cpp|Sources/Model/date.cpp"
  "Sources/event.cpp|Sources/Model/event.cpp"
  "Sources/project.cpp|Sources/Model/project.cpp"
  "Sources/reminder.cpp|Sources/Model/reminder.cpp"
  "Sources/Routine.cpp|Sources/Model/Routine.cpp"
  "Sources/tag.cpp|Sources/Model/tag.cpp"
  "Sources/tagManager.cpp|Sources/Model/tagManager.cpp"
  "Sources/task.cpp|Sources/Model/task.cpp"

  # ── Sources: Model/Persistence ───────────────────────────────────────────
  "Sources/JSONformat.cpp|Sources/Model/Persistence/JSONformat.cpp"
  "Sources/xmlFormat.cpp|Sources/Model/Persistence/xmlFormat.cpp"

  # ── Sources: UI/Widgets ──────────────────────────────────────────────────
  "Sources/UI/DayWidget.cpp|Sources/UI/Widgets/DayWidget.cpp"
  "Sources/UI/TaskWidget.cpp|Sources/UI/Widgets/TaskWidget.cpp"
  "Sources/UI/TagWidget.cpp|Sources/UI/Widgets/TagWidget.cpp"
  "Sources/UI/TagComboBox.cpp|Sources/UI/Widgets/TagComboBox.cpp"
  "Sources/UI/calendar.cpp|Sources/UI/Widgets/calendar.cpp"
  "Sources/UI/month.cpp|Sources/UI/Widgets/month.cpp"
  "Sources/UI/navBar.cpp|Sources/UI/Widgets/navBar.cpp"
  "Sources/UI/ActivityDelete.cpp|Sources/UI/Widgets/ActivityDelete.cpp"
  "Sources/UI/ActivityModify.cpp|Sources/UI/Widgets/ActivityModify.cpp"
  "Sources/UI/ActivitySearch.cpp|Sources/UI/Widgets/ActivitySearch.cpp"

  # ── AddDialog → AddWidget ────────────────────────────────────────────────
  "Sources/Dialog/AddDialog.cpp|Sources/UI/Widgets/AddWidget.cpp"
  "Sources/Dialog/TaskDialog.cpp|Sources/UI/Widgets/TaskDialog.cpp"
)

# ─── 3. ESEGUI SPOSTAMENTI (cp, non mv: sicuro se lo script gira due volte) ─
echo "[2] Copia file nelle nuove posizioni..."

for entry in "${MOVES[@]}"; do
  SRC="${entry%%|*}"
  DST="${entry##*|}"
  if [ -f "$ROOT/$SRC" ]; then
    cp "$ROOT/$SRC" "$ROOT/$DST"
    echo "  OK  $SRC → $DST"
  else
    echo "  --  $SRC (non trovato, skip)"
  fi
done

# ─── 4. AGGIORNA GLI #include INTERNI ───────────────────────────────────────
# Usa sed per sostituire i vecchi percorsi con i nuovi in TUTTI i file .h e .cpp
# della struttura nuova (Headers/ e Sources/).
echo "[3] Aggiornamento #include nei file nuovi..."

# Costruiamo una lista di sostituzioni sed
declare -a SED_RULES=(
  # Model
  's|"Headers/AbstractActivity\.h"|"Headers/Model/AbstractActivity.h"|g'
  's|"Headers/ActivityManager\.h"|"Headers/Model/ActivityManager.h"|g'
  's|"Headers/HourMinute\.h"|"Headers/Model/HourMinute.h"|g'
  's|"Headers/date\.h"|"Headers/Model/date.h"|g'
  's|"Headers/event\.h"|"Headers/Model/event.h"|g'
  's|"Headers/project\.h"|"Headers/Model/project.h"|g'
  's|"Headers/reminder\.h"|"Headers/Model/reminder.h"|g'
  's|"Headers/routine\.h"|"Headers/Model/routine.h"|g'
  's|"Headers/tag\.h"|"Headers/Model/tag.h"|g'
  's|"Headers/tagManager\.h"|"Headers/Model/tagManager.h"|g'
  's|"Headers/task\.h"|"Headers/Model/task.h"|g'
  's|"Headers/config\.h"|"Headers/Model/config.h"|g'
  's|"Headers/rec_ev\.h"|"Headers/Model/rec_ev.h"|g'

  # Model/Persistence
  's|"Headers/JSONformat\.h"|"Headers/Model/Persistence/JSONformat.h"|g'
  's|"Headers/xmlFormat\.h"|"Headers/Model/Persistence/xmlFormat.h"|g'

  # UI/Widgets (da Headers/UI/ flat)
  's|"Headers/UI/DayWidget\.h"|"Headers/UI/Widgets/DayWidget.h"|g'
  's|"Headers/UI/TaskWidget\.h"|"Headers/UI/Widgets/TaskWidget.h"|g'
  's|"Headers/UI/TagWidget\.h"|"Headers/UI/Widgets/TagWidget.h"|g'
  's|"Headers/UI/TagComboBox\.h"|"Headers/UI/Widgets/TagComboBox.h"|g'
  's|"Headers/UI/calendar\.h"|"Headers/UI/Widgets/calendar.h"|g'
  's|"Headers/UI/month\.h"|"Headers/UI/Widgets/month.h"|g'
  's|"Headers/UI/navBar\.h"|"Headers/UI/Widgets/navBar.h"|g'
  's|"Headers/UI/ActivityDelete\.h"|"Headers/UI/Widgets/ActivityDelete.h"|g'
  's|"Headers/UI/ActivityModel\.h"|"Headers/UI/Widgets/ActivityModel.h"|g'
  's|"Headers/UI/ActivityModify\.h"|"Headers/UI/Widgets/ActivityModify.h"|g'
  's|"Headers/UI/ActivitySearch\.h"|"Headers/UI/Widgets/ActivitySearch.h"|g'

  # AddDialog → AddWidget
  's|"Headers/Dialog/AddDialog\.h"|"Headers/UI/Widgets/AddWidget.h"|g'
  's|"Headers/Dialog/ProjectDialog\.h"|"Headers/UI/Widgets/ProjectDialog.h"|g'
  's|"Headers/Dialog/TaskDialog\.h"|"Headers/UI/Widgets/TaskDialog.h"|g'

  # class name: AddDialog → AddWidget (nei file spostati)
  's/\bAddDialog\b/AddWidget/g'
)

# Costruisci un unico comando sed con tutti i -e
SED_CMD="sed -i"
for rule in "${SED_RULES[@]}"; do
  SED_CMD="$SED_CMD -e '$rule'"
done

# Applica a tutti i .h e .cpp nelle nuove cartelle
find "$ROOT/Headers/Model" \
     "$ROOT/Headers/UI/Widgets" \
     "$ROOT/Sources/Model" \
     "$ROOT/Sources/UI/Widgets" \
     "$ROOT/Headers/UI/Forms" \
     "$ROOT/Sources/UI/Forms" \
     "$ROOT/Headers/Visitor" \
     "$ROOT/Sources/Visitor" \
     "$ROOT/Sources/main.cpp" \
     "$ROOT/Sources/mainwindow.cpp" \
     "$ROOT/Headers/mainwindow.h" \
     -name "*.h" -o -name "*.cpp" 2>/dev/null | sort -u | while read -r f; do
  eval "$SED_CMD \"$f\""
  echo "  patched: $f"
done

# ─── 5. AGGIORNA AddWidget.h: classe e guard ─────────────────────────────────
echo "[4] Rinomina guardie include e classe in AddWidget.h..."

AW_H="$ROOT/Headers/UI/Widgets/AddWidget.h"
if [ -f "$AW_H" ]; then
  sed -i \
    -e 's/ADDDIALOG_H/ADDWIDGET_H/g' \
    -e 's/class AddDialog/class AddWidget/g' \
    -e 's/AddDialog(tagManager/AddWidget(tagManager/g' \
    "$AW_H"
fi

AW_CPP="$ROOT/Sources/UI/Widgets/AddWidget.cpp"
if [ -f "$AW_CPP" ]; then
  sed -i \
    -e 's/AddDialog::/AddWidget::/g' \
    -e 's/AddDialog(tagManager/AddWidget(tagManager/g' \
    -e 's/: QWidget(parent), /: QWidget(parent), /g' \
    "$AW_CPP"
fi

# ─── 6. AGGIORNA mainwindow (usa AddWidget, nuovi path) ─────────────────────
echo "[5] Patch mainwindow.h e mainwindow.cpp..."

MW_H="$ROOT/Headers/mainwindow.h"
MW_CPP="$ROOT/Sources/mainwindow.cpp"

for f in "$MW_H" "$MW_CPP"; do
  [ -f "$f" ] || continue
  sed -i \
    -e 's|#include "Headers/Dialog/AddDialog\.h"|#include "Headers/UI/Widgets/AddWidget.h"|g' \
    -e 's/AddDialog\*/AddWidget*/g' \
    -e 's/AddDialog \*/AddWidget */g' \
    -e 's/new AddDialog(/new AddWidget(/g' \
    -e 's/AddDialog\b/AddWidget/g' \
    "$f"
done

# ─── 7. GENERA NUOVO .pro ────────────────────────────────────────────────────
echo "[6] Generazione nuovo Planner_PAO.pro..."

cat > "$PRO_FILE" << 'PROEOF'
QT       += core gui xml
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += c++17

#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

SOURCES += \
    Sources/Model/AbstractActivity.cpp \
    Sources/Model/ActivityManager.cpp \
    Sources/Model/HourMinute.cpp \
    Sources/Model/date.cpp \
    Sources/Model/event.cpp \
    Sources/Model/project.cpp \
    Sources/Model/reminder.cpp \
    Sources/Model/Routine.cpp \
    Sources/Model/tag.cpp \
    Sources/Model/tagManager.cpp \
    Sources/Model/task.cpp \
    Sources/Model/Persistence/JSONformat.cpp \
    Sources/Model/Persistence/xmlFormat.cpp \
    Sources/UI/Forms/ActivityForm.cpp \
    Sources/UI/Forms/EventForm.cpp \
    Sources/UI/Forms/ProjectForm.cpp \
    Sources/UI/Forms/ReminderForm.cpp \
    Sources/UI/Forms/RoutineForm.cpp \
    Sources/UI/Forms/TaskForm.cpp \
    Sources/UI/Widgets/ActivityDelete.cpp \
    Sources/UI/Widgets/ActivityModify.cpp \
    Sources/UI/Widgets/ActivitySearch.cpp \
    Sources/UI/Widgets/AddWidget.cpp \
    Sources/UI/Widgets/DayWidget.cpp \
    Sources/UI/Widgets/TagComboBox.cpp \
    Sources/UI/Widgets/TagWidget.cpp \
    Sources/UI/Widgets/TaskWidget.cpp \
    Sources/UI/Widgets/calendar.cpp \
    Sources/UI/Widgets/month.cpp \
    Sources/UI/Widgets/navBar.cpp \
    Sources/Visitor/DisplayVisitor.cpp \
    Sources/Visitor/FormSelectVisitor.cpp \
    Sources/Visitor/TaskWidgetVisitor.cpp \
    Sources/Visitor/DayWidgetVisitor.cpp \
    Sources/main.cpp \
    Sources/mainwindow.cpp \
    Sources/UI/Widgets/TaskDialog.cpp

HEADERS += \
    Headers/Model/AbstractActivity.h \
    Headers/Model/ActivityManager.h \
    Headers/Model/HourMinute.h \
    Headers/Model/date.h \
    Headers/Model/event.h \
    Headers/Model/project.h \
    Headers/Model/reminder.h \
    Headers/Model/routine.h \
    Headers/Model/tag.h \
    Headers/Model/tagManager.h \
    Headers/Model/task.h \
    Headers/Model/config.h \
    Headers/Model/rec_ev.h \
    Headers/Model/Persistence/JSONformat.h \
    Headers/Model/Persistence/xmlFormat.h \
    Headers/UI/Forms/ActivityForm.h \
    Headers/UI/Forms/EventForm.h \
    Headers/UI/Forms/ProjectForm.h \
    Headers/UI/Forms/ReminderForm.h \
    Headers/UI/Forms/RoutineForm.h \
    Headers/UI/Forms/TaskForm.h \
    Headers/UI/Widgets/ActivityDelete.h \
    Headers/UI/Widgets/ActivityModel.h \
    Headers/UI/Widgets/ActivityModify.h \
    Headers/UI/Widgets/ActivitySearch.h \
    Headers/UI/Widgets/AddWidget.h \
    Headers/UI/Widgets/DayWidget.h \
    Headers/UI/Widgets/TagComboBox.h \
    Headers/UI/Widgets/TagWidget.h \
    Headers/UI/Widgets/TaskWidget.h \
    Headers/UI/Widgets/calendar.h \
    Headers/UI/Widgets/month.h \
    Headers/UI/Widgets/navBar.h \
    Headers/UI/Widgets/ProjectDialog.h \
    Headers/UI/Widgets/TaskDialog.h \
    Headers/Visitor/ActivityVisitor.h \
    Headers/Visitor/AddSubtaskVisitor.h \
    Headers/Visitor/DayWidgetVisitor.h \
    Headers/Visitor/DisplayVisitor.h \
    Headers/Visitor/FormSelectVisitor.h \
    Headers/Visitor/TaskWidgetVisitor.h \
    Headers/mainwindow.h

PROEOF

echo ""
echo "============================================================"
echo " Ristrutturazione completata."
echo ""
echo " STRUTTURA FINALE:"
echo "   Headers/Model/               ← gerarchia attività, date, tag"
echo "   Headers/Model/Persistence/   ← JSONformat, xmlFormat"
echo "   Headers/UI/Forms/            ← form (invariati)"
echo "   Headers/UI/Widgets/          ← QWidget, AddWidget (ex-AddDialog)"
echo "   Headers/Visitor/             ← visitor (invariati)"
echo "   Sources/ (specchia Headers/)"
echo ""
echo " PROSSIMI PASSI:"
echo "   1. Apri Qt Creator"
echo "   2. Chiudi e riapri il .pro (o usa 'qmake && make' da terminale)"
echo "   3. Fai un rebuild completo (Build → Rebuild All)"
echo "   4. Se ci sono errori residui di #include, controlla i file"
echo "      in Sources/Visitor/ e Headers/Visitor/ (non spostati,"
echo "      ma usano i vecchi path: già patchati al punto 3)."
echo "   5. Dopo che tutto compila, puoi eliminare le cartelle vecchie:"
echo "      rm -rf Headers/Dialog"
echo "      # I file vecchi in Headers/UI/*.h e Sources/UI/*.cpp"
echo "      # restano finché non verifichi che tutto compila."
echo "============================================================"
