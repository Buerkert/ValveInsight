import scipy.stats
import numpy as np

def switching_duration_tail_slope(time:np.array, current:np.array, tail_slope_max=0.02):
    """
    Calculate slope of current for a time window. Check if the slope is close to zero. 
    If the slope is close to zero, the movement has finished. Duration was from inrush current start to beginning of time window. 
    If not, move forward a timestep and try again. 
    """
    tail_window_ms = 10.0  # milliseconds
    time_tail_stop_ms = tail_window_ms  # start scan from end of first 10ms
    switch_time = np.nan

    while time_tail_stop_ms < time[-1]:
        stop_idx = np.searchsorted(time, time_tail_stop_ms)
        start_time = time_tail_stop_ms - tail_window_ms
        start_idx = np.searchsorted(time, start_time)

        if stop_idx <= start_idx or stop_idx - start_idx < 2:
            # Malformed data. Stop here. 
            break

        res = scipy.stats.linregress(time[start_idx:stop_idx], current[start_idx:stop_idx])
        tail_slope = np.abs(res.slope)

        if tail_slope < tail_slope_max:
            switch_time = time[start_idx]
            break
        else:
            timestep = time[1] - time[0]
            time_tail_stop_ms += timestep

    return switch_time
