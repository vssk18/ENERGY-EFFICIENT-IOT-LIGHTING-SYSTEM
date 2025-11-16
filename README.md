# 💡 Energy-Efficient IoT Lighting System

Low-cost, sensor-guided street / corridor lighting that dims or turns off in low-traffic periods and ramps up on motion, with a reproducible energy model and firmware.

- **Hardware:** 12 V LED + Arduino-style microcontroller + PIR + light sensor  
- **Control:** simple rule-based state machine (ambient light + presence)  
- **Results (default profile):** ~55% simulated night-hour energy savings vs an always-ON baseline  
  – see `data/energy_summary.csv` and `docs/EXPERIMENTS.md`

---

## 🔍 1. What this project solves

Many campus and public lights stay at full brightness all night, even when there is no one around.

This project explores a realistic prototype that:

- Keeps **instant brightness** when people are present  
- Avoids **visible flicker** when dimming up/down  
- Reduces **wasted on-time** in low-traffic hours  
- Ships a **reproducible simulator** so anyone can re-run the energy model

The goal is not city-scale deployment, but a clean, documented prototype and simulator that a student team can rebuild and extend.

---

## 🗂️ 2. Repository layout

```text
energy-efficient-iot-lighting-system/
├─ firmware/
│   └─ src/
│       ├─ config.h              # Tunable thresholds, PWM levels, timing
│       ├─ lighting_state.h      # State machine types + telemetry
│       ├─ lighting_state.cpp    # Core control logic (OFF / DIM / ON_ACTIVE)
│       └─ main_arduino.ino      # Arduino entry point + serial logging
├─ simulator/
│   ├─ config.py                 # Motion + light profile, power levels
│   ├─ energy_model.py           # Baseline vs prototype Wh computation
│   └─ simulate_week.py          # 7-day simulation, writes CSVs + plots
├─ tools/
│   └─ cpp_log_summary/
│       ├─ log_summary.cpp       # C++ tool to summarise prototype_log.csv
│       └─ Makefile              # Small helper to build/run the summary
├─ data/
│   ├─ prototype_log.csv         # Generated: per-minute telemetry + power
│   └─ energy_summary.csv        # Generated: baseline vs prototype Wh
├─ results/
│   ├─ duty_cycle_plot.png       # Generated: duty cycle / state vs time
│   └─ savings_bar.png           # Generated: baseline vs prototype bar chart
├─ docs/
│   ├─ EXPERIMENTS.md            # Experiments, model, and how to reproduce
│   └─ HARDWARE_NOTES.md         # Safe prototype notes and mains disclaimer
├─ Makefile                      # `make quick-demo` → run simulator end-to-end
├─ LICENSE                       # MIT License
└─ README.md
```

---

## ⚙️ 3. Environment and setup

### 3.1 Python environment (simulator)
```bash
cd ~/Projects/energy-efficient-iot-lighting-system

python3 -m venv .venv
source .venv/bin/activate

pip install --upgrade pip
pip install matplotlib
```
This installs only what the simulator needs (matplotlib + its dependencies).

### 3.2 Toolchain for firmware and C++
- Firmware (Arduino): any Arduino-compatible board + IDE / CLI
- C++ summary tool: g++ (comes with Xcode Command Line Tools on macOS)

The repo does not contain upload scripts for real hardware; it focuses on firmware logic and simulation.

---

## ▶️ 4. Quick demo (one command)

Once the virtual environment is ready:
```bash
cd ~/Projects/energy-efficient-iot-lighting-system
make quick-demo
```
This will:
1. Run a 7-day simulation of an always-ON baseline vs the smart light.
2. Write:
   - data/prototype_log.csv
   - data/energy_summary.csv
3. Generate plots:
   - results/duty_cycle_plot.png
   - results/savings_bar.png
4. Print a short summary, for example:
```
Baseline Wh : 756.0
Prototype Wh: 337.8
Savings %   : 55.3
```
You can mention this as your one-command reproducible demo.

### 4.1 Example result plot

After `make quick-demo`, you will find:

- `results/duty_cycle_plot.png`
- `results/savings_bar.png`

For example:

```
Baseline Wh : 756.0
Prototype Wh: 337.8
Savings %   : 55.3
```
You can embed the savings bar chart in reports or slides to visually show impact.

---

## 🧠 5. Firmware overview (firmware/src)

The firmware is written in simple C++ for an Arduino-style board.

Key idea: a tiny state machine with ambient light + motion inputs.
- config.h
  - Lux thresholds for day/night detection
  - PWM levels for DIM vs ON
  - Hold times after motion
  - Idle times before dimming down
  - Ramp duration for smooth transitions
- lighting_state.h / .cpp
  - Mode { OFF, DIM, ON_ACTIVE }
  - Tracks:
    - current mode
    - last motion time
    - current PWM
  - Steps per loop:
    - Read lux + motion
    - Apply hysteresis for day/night
    - Update mode based on motion + timers
    - Ramp PWM smoothly to the new level
    - Return a Telemetry struct with ms, lux, motion, mode, pwm
- main_arduino.ino
  - Calls sys.begin() in setup()
  - In loop(), calls sys.step() and logs one line every second to the serial port

The same telemetry schema is mirrored in the simulator so that real logs and synthetic logs can be compared.

---

## 📈 6. Simulator and energy model (simulator/)

The simulator produces minute-by-minute data for 7 days.
- config.py
  - Motion profile by hour of day (weekday vs weekend)
  - Simple ambient light curve for sunrise / sunset
  - LED power in watts for each mode
- energy_model.py
  - Computes:
    - Baseline energy: always-ON at full power during night hours
    - Prototype energy: integral of per-minute power under OFF / DIM / ON
- simulate_week.py
  - Generates 7 × 24 × 60 samples
  - Saves:
    - data/prototype_log.csv (full time series)
    - data/energy_summary.csv (Wh + savings %)
    - results/duty_cycle_plot.png
    - results/savings_bar.png

Details of each experiment and configuration are documented in docs/EXPERIMENTS.md.

---

## 🧪 7. Experiments and interpretation

See docs/EXPERIMENTS.md for:
- Baseline assumptions (hours, power, traffic profile)
- How the simulator maps mode → power → Wh
- 7-day time series examples
- How to tweak:
  - HOLD / IDLE times
  - DIM level
  - Motion intensity
- How those changes affect:
  - Energy savings
  - Comfort (time spent at full brightness)

That file is written so that a reviewer can understand your experiment pipeline without reading the source code.

---

## 🔌 8. Hardware scope and safety

See docs/HARDWARE_NOTES.md for:
- A safe 12 V lab prototype (LED + MOSFET + sensors)
- Recommended sensor parts (PIR, LDR/BH1750)
- Notes on enclosures and thermal considerations
- Clear statement that mains deployments require certified hardware and qualified electricians

This project focuses on embedded logic + simulation, not full mains-grade electrical design.

---

## 📚 Documentation

- **Experiments & energy model:** [`docs/EXPERIMENTS.md`](docs/EXPERIMENTS.md)  
  How the simulator works, how energy is computed, and how to reproduce the numbers.

- **Hardware scope & safety:** [`docs/HARDWARE_NOTES.md`](docs/HARDWARE_NOTES.md)  
  Recommended 12 V lab prototype and clear boundaries for mains deployments.

---

## 🧾 License

This project is released under the MIT License.
See LICENSE for details.

---

## 👤 Author

Varanasi Sai Srinivasa Karthik  
B.Tech CSE (Cybersecurity), GITAM School of Technology, Hyderabad  
Contact: svaranas3@gitam.in, varanasikarthik44@gmail.com  
- GitHub: @vssk18
