#!/bin/bash

qemu-system-x86_64 -drive format=raw,file=tinyos.img -rtc base=localtime -m 1G
