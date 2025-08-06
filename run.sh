#!/bin/bash

# Kernel running with 1MB of RAM (-m 1G)
qemu-system-x86_64 -drive format=raw,file=tinyos.img -rtc base=localtime -m 1G
