#!/bin/bash
make mrproper
cp sata210_config .config
make -j8

