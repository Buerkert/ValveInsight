import numpy as np

class switching_detection_online:
    """
    Online estimation of the integral value for successful switching. 
    If the tail_window if large enough to cover the expected switching duration, 
    you can stop the data acquisition after the integral value is large enough. 
    """
    
    def __init__(self):
        self.integral = 0
        self.n = 0
        self.tail_window = np.zeros(200)

    def switching_detection_online(self, current_mA:float, timestep_ms:float):
        self.integral += current_mA * timestep_ms
        self.tail_window[self.n % len(self.tail_window)] = current_mA
        self.n += 1        

        # Calculate tail mean
        if self.n > len(self.tail_window):
            tail_mean = np.mean(self.tail_window)
        else:
            # Not a full tail window acquired yet
            return 0, 0

        max_integral = self.n * timestep_ms
        current_integral = max_integral - self.integral / tail_mean
        current_integral = np.round(current_integral, 2)
        return current_integral, tail_mean