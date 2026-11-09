#!/usr/bin/env bash
# ==============================================================================
# Script Name: build-flasher.sh
# Description: Clean and parallel build script for Crusan & Co. AxisOS Flasher
# Author:      Marc F. Crusan (MFC) / Crusan & Co. w/ PP & AGY (AXEL)
# Date:        2026110601
# Version:     v1.2.0
# ==============================================================================
# 2026110601 (MFC/PP/AGY) Upgraded to canonical 5-Field SOP Header.

make clean
make -j4
