#!/bin/bash

qemu-system-x86_64 -drive format=raw,file=bootloader/boot -rtc base=localtime
