<div align="center">

<a href="#readme"><img src="https://img.shields.io/badge/-README-24292f?style=flat&logo=book&logoColor=white" alt="README" /></a>
&nbsp;&nbsp;
<a href="CODE_OF_CONDUCT.md"><img src="https://img.shields.io/badge/-Code%20of%20Conduct-24292f?style=flat&logo=heart&logoColor=white" alt="Code of Conduct" /></a>
&nbsp;&nbsp;
<a href="CONTRIBUTING.md"><img src="https://img.shields.io/badge/-Contributing-24292f?style=flat&logo=group&logoColor=white" alt="Contributing" /></a>
&nbsp;&nbsp;
<a href="LICENSE"><img src="https://img.shields.io/badge/-MIT%20License-24292f?style=flat&logo=scale&logoColor=white" alt="MIT License" /></a>
&nbsp;&nbsp;
<a href="SECURITY.md"><img src="https://img.shields.io/badge/-Security-24292f?style=flat&logo=shield&logoColor=white" alt="Security" /></a>

</div>

---

# 💡 Energy-Efficient IoT Lighting System

![C++](https://img.shields.io/badge/Language-C++-blue)
![Python](https://img.shields.io/badge/Language-Python-yellow)
![License: MIT](https://img.shields.io/badge/License-MIT-brightgreen)
![Last Commit](https://img.shields.io/github/last-commit/vssk18/ENERGY-EFFICIENT-IOT-LIGHTING-SYSTEM)

> **Low-voltage 12 V LED “smart” street/corridor lighting demo:**  
> Rule-based embedded firmware + Python simulator for energy savings. Reproducible, fully open source.

---

## 🚀 Core Features

- Sensor-guided ambient + motion detection
- State machine logic (OFF / DIM / ON_ACTIVE)
- Instant brightness on presence, smooth dimming
- Cross-language code: firmware (C++) + simulator (Python)
- Quantitative, reproducible week-long energy savings
- Modular codebase, easy to extend for new hardware

---

## 💻 Tech Stack

| Tech       | Usage                                      |
|------------|--------------------------------------------|
| C++ / Arduino | Firmware: Embedded control, PWM logic    |
| Python 3.8+  | Simulator: Energy model, CSV, plots       |
| Makefile     | Build/make quick-demo targets             |
| MIT License  | Open source, full code + experiment docs  |

---

## 🗂️ Repository Layout

```plaintext
ergy-efficient-iot-lighting-system/
├── firmware/
│   └── src/
│       ├─ config.h              # Tunable thresholds, PWM levels, timing
│       ├─ lighting_state.h      # State machine types + telemetry
│       ├─ lighting_state.cpp    # Core control logic (OFF / DIM / ON_ACTIVE)
│       └─ main_arduino.ino      # Arduino entry + serial logging
├── simulator/
│   ├─ config.py                 # Motion/light profile, power levels
│   ├─ energy_model.py           # Baseline/prototype Wh computation
│   └─ simulate_week.py          # 7-day simulation, CSVs, plots
├── tools/
│   └─ cpp_log_summary/
│       ├─ log_summary.cpp       # C++ summary tool for log CSV
│       └─ Makefile
├── data/
│   ├─ prototype_log.csv         # Per-minute telemetry + power (generated)
│   └─ energy_summary.csv        # Baseline vs prototype Wh (generated)
├── results/
│   ├─ duty_cycle_plot.png       # Duty cycle/time, generated
│   └─ savings_bar.png           # Baseline vs prototype bar chart, generated
├── docs/
│   ├─ EXPERIMENTS.md            # Experiment, model, reproducibility doc
│   └─ HARDWARE_NOTES.md         # Hardware scope, key safety notes
├── Makefile                     # One-command demo and helpers
├── LICENSE                      # MIT License
└── README.md
```

---

## ⚙️ Setup & Environment

```bash
git clone https://github.com/vssk18/ENERGY-EFFICIENT-IOT-LIGHTING-SYSTEM.git
cd ENERGY-EFFICIENT-IOT-LIGHTING-SYSTEM

# Python simulator setup
python3 -m venv .venv
source .venv/bin/activate
pip install --upgrade pip
pip install matplotlib

# Firmware (C++): Use any Arduino-style board + IDE/CLI
# C++ summary tool: Requires g++, tested with macOS Xcode CLI tools

# One-command demo:
make quick-demo
```

---

## ▶️ Quick Demo & Results

- **Run `make quick-demo`**
- Direct outputs (simulated week):
  - `data/prototype_log.csv`
  - `data/energy_summary.csv` (shows baseline/energy Wh and savings %)
  - `results/duty_cycle_plot.png`
  - `results/savings_bar.png`

Sample summary:
```
Baseline Wh:   756.0
Prototype Wh:  337.8
Savings %:     55.3
```
Embed the bar chart or time-series plot in reports/slides to show real impact.

---

## 🧠 Firmware Overview

- **config.h:** All thresholds, PWM levels, timing, I/O pin mapping.
- **lighting_state.*:** Rule-based state machine for live sensor inputs, smooth transitions.
- **main_arduino.ino:** Entry point. Calls Lighting::step() each loop, logs telemetry.

Typical Arduino Nano/Uno upload style—see docs/HARDWARE_NOTES.md for connection and safety.

---

## 📈 Simulator & Energy Model

- `simulator/`: All code for synthetic week simulation.
  - Models day/night, motion, power, and control modes.
- Produces CSVs and plots to quantify savings.
- Modifiable—change config.py for different usage patterns and rerun.

---

## 📚 Documentation

- **Experiments & reproducibility:** [`docs/EXPERIMENTS.md`](docs/EXPERIMENTS.md)
- **Hardware scope/safety:** [`docs/HARDWARE_NOTES.md`](docs/HARDWARE_NOTES.md)

---

## 🛡️ Security & Conduct

- Bug/feature reporting: see [issues/templates](./.github/ISSUE_TEMPLATE)
- MIT Licensed; no proprietary code or cloud integrations

---

## 🤝 Contributing & Acknowledgements

Open to community enhancement and reproducibility replication.  
See [CONTRIBUTING.md](./CONTRIBUTING.md) if present, or open issues/PRs directly.

Big thanks to all collaborators, testers, and reviewers!

---

## 📄 License

MIT License – see [LICENSE](./LICENSE).

---

## 👨‍💻 Author / Maintainer

**Varanasi Sai Srinivasa Karthik**  
B.Tech CSE (Cybersecurity), GITAM School of Technology, Hyderabad  
📧 svaranas3@gitam.in | 📧 varanasikarthik44@gmail.com  
[GitHub Profile](https://github.com/vssk18)