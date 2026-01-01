#!/bin/bash
cd `dirname $0`
lupdate -recursive src/ -ts src/translations/sysbro*.ts
lupdate -recursive sysbro-network-test/ -ts sysbro-network-test/translations/sysbro-network-test*.ts
lupdate -recursive sysbro-cpufreq/ -ts sysbro-cpufreq/translations/sysbro-cpufreq*.ts
