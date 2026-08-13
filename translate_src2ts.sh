#!/bin/bash
cd `dirname $0`

# Prefer Qt6 lupdate, fallback to the generic one.
if command -v lupdate-qt6 >/dev/null 2>&1; then
    LUPDATE=lupdate-qt6
elif [ -x /usr/lib/qt6/bin/lupdate ]; then
    LUPDATE=/usr/lib/qt6/bin/lupdate
else
    LUPDATE=lupdate
fi

"${LUPDATE}" -recursive src/ -ts src/translations/sysbro*.ts
"${LUPDATE}" -recursive sysbro-network-test/ -ts sysbro-network-test/translations/sysbro-network-test*.ts
"${LUPDATE}" -recursive sysbro-cpufreq/ -ts sysbro-cpufreq/translations/sysbro-cpufreq*.ts
