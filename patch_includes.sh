#!/usr/bin/env bash
# patch_includes.sh
# Aggiorna tutti gli #include ai nuovi percorsi.
# Compatibile con macOS (BSD sed) e Linux (GNU sed).
# Uso: cd <root-progetto> && bash patch_includes.sh
set -euo pipefail

ROOT="$(pwd)"

# Rileva se siamo su macOS (BSD sed) o Linux (GNU sed)
if sed --version 2>/dev/null | grep -q GNU; then
    SED_INPLACE=(-i)
else
    # BSD sed (macOS) richiede un argomento dopo -i
    SED_INPLACE=(-i '')
fi

echo "[patch] Utilizzo sed: ${SED_INPLACE[*]}"

# Raccogli tutti i file .h e .cpp nelle nuove posizioni
FILES=()
while IFS= read -r -d '' f; do
    FILES+=("$f")
done < <(find \
    "$ROOT/Headers/Model" \
    "$ROOT/Headers/UI" \
    "$ROOT/Headers/Visitor" \
    "$ROOT/Sources/Model" \
    "$ROOT/Sources/UI" \
    "$ROOT/Sources/Visitor" \
    "$ROOT/Headers/mainwindow.h" \
    "$ROOT/Sources/main.cpp" \
    "$ROOT/Sources/mainwindow.cpp" \
    \( -name "*.h" -o -name "*.cpp" \) \
    -print0 2>/dev/null)

echo "[patch] File trovati: ${#FILES[@]}"
echo ""

patch_file() {
    local f="$1"
    sed "${SED_INPLACE[@]}" \
        -e 's|"Headers/AbstractActivity\.h"|"Headers/Model/AbstractActivity.h"|g' \
        -e 's|"Headers/ActivityManager\.h"|"Headers/Model/ActivityManager.h"|g' \
        -e 's|"Headers/HourMinute\.h"|"Headers/Model/HourMinute.h"|g' \
        -e 's|"Headers/date\.h"|"Headers/Model/date.h"|g' \
        -e 's|"Headers/event\.h"|"Headers/Model/event.h"|g' \
        -e 's|"Headers/project\.h"|"Headers/Model/project.h"|g' \
        -e 's|"Headers/reminder\.h"|"Headers/Model/reminder.h"|g' \
        -e 's|"Headers/routine\.h"|"Headers/Model/routine.h"|g' \
        -e 's|"Headers/tag\.h"|"Headers/Model/tag.h"|g' \
        -e 's|"Headers/tagManager\.h"|"Headers/Model/tagManager.h"|g' \
        -e 's|"Headers/task\.h"|"Headers/Model/task.h"|g' \
        -e 's|"Headers/config\.h"|"Headers/Model/config.h"|g' \
        -e 's|"Headers/rec_ev\.h"|"Headers/Model/rec_ev.h"|g' \
        -e 's|"Headers/JSONformat\.h"|"Headers/Model/Persistence/JSONformat.h"|g' \
        -e 's|"Headers/xmlFormat\.h"|"Headers/Model/Persistence/xmlFormat.h"|g' \
        -e 's|"Headers/UI/DayWidget\.h"|"Headers/UI/Widgets/DayWidget.h"|g' \
        -e 's|"Headers/UI/TaskWidget\.h"|"Headers/UI/Widgets/TaskWidget.h"|g' \
        -e 's|"Headers/UI/TagWidget\.h"|"Headers/UI/Widgets/TagWidget.h"|g' \
        -e 's|"Headers/UI/TagComboBox\.h"|"Headers/UI/Widgets/TagComboBox.h"|g' \
        -e 's|"Headers/UI/calendar\.h"|"Headers/UI/Widgets/calendar.h"|g' \
        -e 's|"Headers/UI/month\.h"|"Headers/UI/Widgets/month.h"|g' \
        -e 's|"Headers/UI/navBar\.h"|"Headers/UI/Widgets/navBar.h"|g' \
        -e 's|"Headers/UI/ActivityDelete\.h"|"Headers/UI/Widgets/ActivityDelete.h"|g' \
        -e 's|"Headers/UI/ActivityModel\.h"|"Headers/UI/Widgets/ActivityModel.h"|g' \
        -e 's|"Headers/UI/ActivityModify\.h"|"Headers/UI/Widgets/ActivityModify.h"|g' \
        -e 's|"Headers/UI/ActivitySearch\.h"|"Headers/UI/Widgets/ActivitySearch.h"|g' \
        -e 's|"Headers/Dialog/AddDialog\.h"|"Headers/UI/Widgets/AddWidget.h"|g' \
        -e 's|"Headers/Dialog/ProjectDialog\.h"|"Headers/UI/Widgets/ProjectDialog.h"|g' \
        -e 's|"Headers/Dialog/TaskDialog\.h"|"Headers/UI/Widgets/TaskDialog.h"|g' \
        -e 's/\bAddDialog\b/AddWidget/g' \
        -e 's/ADDDIALOG_H/ADDWIDGET_H/g' \
        "$f"
}

for f in "${FILES[@]}"; do
    patch_file "$f"
    echo "  patched: $f"
done

echo ""
echo "============================================================"
echo " Patch #include completata."
echo " Ora fai Rebuild All in Qt Creator."
echo "============================================================"
