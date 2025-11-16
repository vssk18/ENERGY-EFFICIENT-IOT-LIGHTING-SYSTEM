# Experiments – Energy-Efficient IoT Lighting System

This document explains how the current energy results in this repository were produced and how to reproduce them.

The goal is to compare a **naive always-ON light** with a **presence + ambient-light controlled light** using a simple, explainable model.

---

## 1. Objectives

1. Model night-time energy use for:
   - a baseline light that is ON at full power all night, and  
   - a prototype light that switches between `OFF`, `DIM`, and `ON_ACTIVE` based on motion and ambient light.
2. Use the **same power rating** for both systems so differences come only from duty cycle.
3. Provide a **reproducible simulation**: one command regenerates CSV logs and plots.

The current configuration gives a result close to:

- **Baseline energy**: 756 Wh over the simulated week  
- **Prototype energy**: ~338 Wh  
- **Savings**: ~55.3 %  

These numbers come directly from the simulator in `simulator/` and the configuration in `simulator/config.py`.

---

## 2. Firmware behaviour (short summary)

The embedded logic in `firmware/src/` is a small C++ state machine with three modes:

- `OFF` – LED off  
- `DIM` – reduced PWM duty cycle (low brightness)  
- `ON_ACTIVE` – full PWM duty cycle (full brightness)

Inputs:

- Ambient light (LDR or digital light sensor)
- Motion (PIR sensor)
- Time since last detected motion

High-level rules:

- During the **day**, when ambient light is high, the light is forced to `OFF`.
- At **night**, when motion is detected, the light goes to `ON_ACTIVE` and stays there for a “hold” time.
- After the hold time with no motion, the light falls back to `DIM` instead of fully off.
- Basic hysteresis is applied on ambient light thresholds to avoid mode flapping at dawn/dusk.
- PWM transitions are ramped, not instant, to keep visible flicker low.

All tunable parameters (PWM levels, thresholds, timings) live in:

- `firmware/src/config.h`

The Python simulator mimics this behaviour at a higher level, it does not simulate PWM at the hardware-timer level.

---

## 3. Simulation setup

The simulator is in the `simulator/` folder:

- `config.py` – describes the week profile and power model  
- `energy_model.py` – baseline vs prototype energy integration  
- `simulate_week.py` – runs the 7-day simulation and writes CSV + plots  

### 3.1 Time horizon and step

- Time step: **1 minute**  
- Horizon: **7 days** (mixed weekdays and weekend)  
- For each minute we generate:
  - approximate ambient light level  
  - whether motion occurred  
  - the resulting mode (`OFF`, `DIM`, `ON_ACTIVE`)  
  - estimated power draw for that minute

### 3.2 Ambient light profile

The simulator uses a simple day/night profile:

- Bright during daytime hours  
- Low during night hours  
- Transition periods around sunrise and sunset

Exact thresholds and curve shape are defined in `simulator/config.py`.  
At a high level, the logic is only:

- “Day” → force mode to `OFF`  
- “Night” → control based on motion history + hold/idle timers

### 3.3 Motion model

Motion events are generated synthetically per minute:

- Higher motion rates in the **evening** and **early night**
- Lower motion rates in the **late night** and **early morning**

The motion model is intentionally simple (random events with different probabilities by hour).  
The purpose is to approximate a corridor or path where traffic is bursty and non-uniform over the night.

### 3.4 Power model

For the energy calculation we only need **relative power** for each mode:

- `OFF`        → 0 W  
- `DIM`        → some fraction of full power (for example, around 30–40 %)  
- `ON_ACTIVE`  → full rated power (for example, around 9 W)

Exact power values and fractions are in `simulator/config.py`.  
The important point is that **baseline and prototype share the same full-power rating**.

---

## 4. Baseline vs prototype definitions

### 4.1 Baseline

The baseline model corresponds to a light that is ON for all night-time minutes.

In the simulator this is:

- For each night minute: power = `P_full`  
- For each day minute: power = 0  

Total baseline energy (Wh) over the week is the sum of all night-minute energy contributions.

### 4.2 Prototype

The prototype uses the same full-power rating, but:

- Applies the state machine logic described in Section 2
- Chooses `OFF`, `DIM`, or `ON_ACTIVE` per minute based on simulated light and motion
- Converts the chosen mode to an estimated power draw using the power model

Total prototype energy (Wh) over the week is the sum of per-minute energy with the adaptive modes.

---

## 5. Outputs produced

When you run:

```bash
cd ~/Projects/energy-efficient-iot-lighting-system
source .venv/bin/activate
python simulator/simulate_week.py
