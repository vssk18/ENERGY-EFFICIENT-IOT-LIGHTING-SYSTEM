"""
Simple energy model for the Energy-Efficient IoT Lighting System.

Assumptions:
- One log row per minute.
- Column 'power_w' = instantaneous power draw in watts at that minute.
Total energy in Wh = sum(power_w) * (delta_t_hours).
"""

from __future__ import annotations
from dataclasses import dataclass
from typing import Iterable


@dataclass
class EnergyStats:
    baseline_wh: float
    prototype_wh: float

    @property
    def savings_wh(self) -> float:
        return self.baseline_wh - self.prototype_wh

    @property
    def savings_pct(self) -> float:
        if self.baseline_wh <= 0:
            return 0.0
        return 100.0 * (self.baseline_wh - self.prototype_wh) / self.baseline_wh


def energy_from_power_series(power_w: Iterable[float], minutes_step: float = 1.0) -> float:
    """
    Integrate energy from a power series (watts).
    Assumes one sample every 'minutes_step' minutes.
    Returns watt-hours (Wh).
    """
    power_list = list(power_w)
    hours_step = minutes_step / 60.0
    return sum(power_list) * hours_step
