#!/bin/bash
# 处理 dpkg 被中断的情况
dpkg --configure -a
# 修复未安装完成的依赖
aptss install -f -y --allow-downgrades