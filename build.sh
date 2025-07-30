#!/bin/bash

nasm bootloader/boot.asm -o bootloader/boot -i bootloader/real_mode -i bootloader/protected_mode -i bootloader/long_mode

