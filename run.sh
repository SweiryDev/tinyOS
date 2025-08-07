#!/bin/bash

# Kernel running with 1MB of RAM (-m 1G)
# Serial port COM1 redirected to the running terminal
qemu-system-x86_64 -drive format=raw,file=tinyos.img -rtc base=localtime -m 1G -serial stdio
