# 💡 Energy-Efficient IoT Lighting System

![C++](https://img.shields.io/badge/Language-C%2B%2B-Firmware-blue)  
![Python](https://img.shields.io/badge/Language-Python-Simulator-yellow)  
![Arduino](https://img.shields.io/badge/Platform-Arduino-red)  
![License: MIT](https://img.shields.io/badge/License-MIT-brightgreen)

**Low-cost, sensor-guided lighting prototype that intelligently dims or turns off in low-traffic periods and instantly ramps up on motion — with a reproducible energy model & firmware.**

---

## ⚡ 1. The Problem  
Many street lights or corridor lights remain fully on all night, even when no one is around.  
This wastes energy, increases cost and reduces sustainability.

---

## 🔧 2. The Solution  
This project implements:  
- A 12 V LED + Arduino-style microcontroller + ambient light sensor + PIR motion sensor  
- A simple rule-based state machine: dim when no motion, off by day, full brightness on motion  
- A Python simulator + log analyzer to model weekly energy use & validate savings  
- A one-command demo for reproducibility  

---

## 🏗️ 3. Repository Layout

```text
energy-efficient-iot-lighting-system/
├─ firmware/
│   └─ src/
│       ├─ config.h
│       ├─ lighting_state.h
│       ├─ lighting_state.cpp
│       └─ main_arduino.ino
├─ simulator/
│   ├─ config.py
│   ├─ energy_model.py
│   └─ simulate_week.py
├─ tools/
│   └─ cpp_log_summary/
│       ├─ log_summary.cpp
│       ├─ Makefile
│       └─ README.md (usage)
├─ data/         # generated CSVs (git-ignored)
├─ results/      # simulation plots (git-ignored)
├─ Makefile
├─ README.md
└─ LICENSE
