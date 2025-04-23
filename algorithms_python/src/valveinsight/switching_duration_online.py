import scipy.stats
import numpy as np

class switching_duration_online:
    """
    Online estimation of the integral value for successful switching. 
    If the tail_window if large enough to cover the expected switching duration, 
    you can stop the data acquisition after the integral value is large enough. 
    """
    
    def __init__(self, tail_slope_max=0.02):
        self.n = 0
        self.tail_window_size = 200
        self.tail_window = []
        self.tail_slope_max = tail_slope_max

    def switching_duration_online(self, current_mA:float, timestep_ms:float):
        if len(self.tail_window) >= self.tail_window_size:
            self.tail_window.pop(0)
        self.tail_window.append(current_mA)
        self.n += 1

        switch_time = np.nan
        
        # Calculate tail slope, if a full window has been acquired
        if self.n >= self.tail_window_size:
            t = np.arange(self.n - len(self.tail_window), self.n) * timestep_ms
            res = scipy.stats.linregress(t, self.tail_window)
            tail_slope = np.abs(res.slope)

            if tail_slope < self.tail_slope_max:
                switch_time = (self.n - len(self.tail_window)) * timestep_ms

        return switch_time
