from __future__ import annotations
import csv
from dataclasses import dataclass
from datetime import datetime, timedelta
from pathlib import Path
import math
import random

import matplotlib.pyplot as plt

from energy_model import energy_from_power_series, EnergyStats
from config import LUX_DAY_ON, LUX_NIGHT_ON, PWM_DIM, PWM_ON

MINUTE_STEP = 1  # minutes per simulation step
N_DAYS = 7

# Assume full power at ON state
P_ON_W = 9.0       # example: 9 W LED
ALPHA_DIM = 0.35   # dim mode uses ~35% of full power


@dataclass
class SimState:
    night: bool = False
    mode: str = "OFF"  # OFF, DIM, ON_ACTIVE
    last_motion_ts: datetime | None = None
    pwm: int = 0


def lux_profile(ts: datetime) -> int:
    """
    Very rough lux model:
    - Bright midday (~180)
    - Dark night (~10)
    - Smooth transitions at dawn & dusk
    """
    hour = ts.hour + ts.minute / 60.0
    # cosine based day curve
    day_factor = 0.5 * (1 + math.cos((hour - 12) / 12 * math.pi))
    # map [0,1] to [10, 180]
    return int(10 + (1 - day_factor) * 170)


def motion_probability(ts: datetime) -> float:
    """
    Simple motion model:
    - Higher in early night (18–22)
    - Lower late night (1–5)
    """
    hour = ts.hour
    if 18 <= hour < 23:
        return 0.4
    if 23 <= hour or hour < 5:
        return 0.08
    if 5 <= hour < 7:
        return 0.1
    return 0.2


def power_for_pwm(pwm: int) -> float:
    if pwm <= 0:
        return 0.0
    frac = pwm / 255.0
    return P_ON_W * frac


def update_state(ts: datetime, state: SimState) -> SimState:
    lux = lux_profile(ts)
    prob = motion_probability(ts)
    motion = random.random() < prob

    # Hysteresis on lux
    if not state.night and lux < LUX_NIGHT_ON:
        state.night = True
    elif state.night and lux > LUX_DAY_ON:
        state.night = False

    if motion:
        state.last_motion_ts = ts

    if not state.night:
        # Daytime
        state.mode = "OFF"
        state.pwm = 0
    else:
        if state.last_motion_ts is None:
            idle_seconds = 999999
        else:
            idle_seconds = (ts - state.last_motion_ts).total_seconds()

        if motion or idle_seconds < 90:  # HOLD_SEC
            state.mode = "ON_ACTIVE"
            state.pwm = PWM_ON
        elif idle_seconds > 120:  # IDLE_SEC
            state.mode = "DIM"
            state.pwm = PWM_DIM
        # else keep current mode

    return state


def main():
    random.seed(42)

    root = Path(__file__).resolve().parents[1]
    data_dir = root / "data"
    results_dir = root / "results"
    data_dir.mkdir(exist_ok=True, parents=True)
    results_dir.mkdir(exist_ok=True, parents=True)

    log_path = data_dir / "prototype_log.csv"

    start = datetime(2023, 9, 1, 18, 0, 0)
    ts = start
    end = start + timedelta(days=N_DAYS)

    state = SimState()
    power_samples = []

    with log_path.open("w", newline="") as f:
        writer = csv.writer(f)
        writer.writerow(["ts", "lux", "motion", "mode", "pwm", "power_w"])

        while ts < end:
            state = update_state(ts, state)
            lux = lux_profile(ts)
            prob = motion_probability(ts)
            motion = random.random() < prob  # sampled for logging
            power_w = power_for_pwm(state.pwm)

            power_samples.append(power_w)

            writer.writerow([
                ts.isoformat(),
                lux,
                int(motion),
                state.mode,
                state.pwm,
                f"{power_w:.3f}",
            ])

            ts += timedelta(minutes=MINUTE_STEP)

    # Baseline: always ON during night hours with P_ON_W
    baseline_wh_per_day = P_ON_W * 12  # 12h night
    baseline_wh = baseline_wh_per_day * N_DAYS

    prototype_wh = energy_from_power_series(power_samples, minutes_step=MINUTE_STEP)
    stats = EnergyStats(baseline_wh=baseline_wh, prototype_wh=prototype_wh)

    summary_path = data_dir / "energy_summary.csv"
    with summary_path.open("w", newline="") as f:
        writer = csv.writer(f)
        writer.writerow(["period", "baseline_wh", "prototype_wh", "savings_pct"])
        writer.writerow([
            "overall",
            f"{stats.baseline_wh:.2f}",
            f"{stats.prototype_wh:.2f}",
            f"{stats.savings_pct:.2f}",
        ])

    print("Baseline Wh :", stats.baseline_wh)
    print("Prototype Wh:", stats.prototype_wh)
    print("Savings %   :", stats.savings_pct)

    # Plot 1: power time series
    plt.figure()
    plt.plot(power_samples)
    plt.xlabel("Minute index")
    plt.ylabel("Power (W)")
    plt.title("Prototype power over simulated week")
    plt.tight_layout()
    plt.savefig(results_dir / "duty_cycle_plot.png")

    # Plot 2: bar chart baseline vs prototype
    plt.figure()
    plt.bar(["Baseline", "Prototype"], [stats.baseline_wh, stats.prototype_wh])
    plt.ylabel("Energy (Wh) over week")
    plt.title("Baseline vs prototype energy")
    plt.tight_layout()
    plt.savefig(results_dir / "savings_bar.png")

    print("Wrote:", log_path)
    print("Wrote:", summary_path)
    print("Wrote:", results_dir / "duty_cycle_plot.png")
    print("Wrote:", results_dir / "savings_bar.png")


if __name__ == "__main__":
    main()
