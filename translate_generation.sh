#!/bin/bash
# this file is used to auto-generate .qm file from .ts file.
# author: shibowen at linuxdeepin.com

# Prefer Qt6 lrelease, fallback to the generic one.
if command -v lrelease-qt6 >/dev/null 2>&1; then
    LRELEASE=lrelease-qt6
elif [ -x /usr/lib/qt6/bin/lrelease ]; then
    LRELEASE=/usr/lib/qt6/bin/lrelease
else
    LRELEASE=lrelease
fi

ts_list=(`ls src/translations/*.ts` `ls sysbro-startup-apps/translations/*.ts` `ls sysbro-file-shredder/translations/*.ts` `ls sysbro-network-test/translations/*.ts`)

for ts in "${ts_list[@]}"
do
    printf "\nprocess ${ts}\n"
    "${LRELEASE}" "${ts}"
done
