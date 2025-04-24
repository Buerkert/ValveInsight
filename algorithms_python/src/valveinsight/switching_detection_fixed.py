import numpy as np

def switching_detection_fixed(time_ms:np.array, current_mA:np.array):
    """
    Calculate tail mean from last 10 milliseconds in data. 
    """
    t_mean_slope_ms = 10
    
    idx = np.where((time_ms >  time_ms.max()-t_mean_slope_ms))
    tail_mean = current_mA[idx].mean()

    current_inv = tail_mean - current_mA
    current_inv_norm = current_inv / tail_mean

    time_step_ms = time_ms[1] - time_ms[0]

    integral = current_inv_norm.sum()*time_step_ms
    integral = np.round(integral, 2)
    return integral, tail_mean