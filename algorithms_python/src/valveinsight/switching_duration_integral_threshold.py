import numpy as np

from . import switching_detection_fixed

def switching_duration_integral_threshold(time_ms:np.array, current_mA:np.array, integral_threshold=0.8):
    """
    """
    integral_switching_detection, tail_mean = switching_detection_fixed(time_ms, current_mA)

    integral = 0
    duration_idx = len(time_ms)-1
    time_step_ms = time_ms[1] - time_ms[0]
    for idx, I in enumerate(current_mA):
        current_inv = tail_mean - I
        current_inv_norm = current_inv / tail_mean
        integral += current_inv_norm * time_step_ms

#        print(integral, integral_switching_detection)

#        if integral_threshold - np.abs(integral) < 1e-6:
        if integral >= integral_threshold:
            duration_idx = idx
            break

    duration = time_ms[duration_idx] - time_ms[0]
    return duration
