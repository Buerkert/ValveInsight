import numpy as np

def switching_duration_inside_band(time_ms:np.array, current_mA:np.array, band=0.02):
    """
    Calculate tail mean from last 10 milliseconds in data. 
    Switching is finished, if all values remain inside a band (in percent) around the tail mean. 
    """
    t_final_steady_state_ms = 10
    
    idx = np.where((time_ms >  time_ms.max()-t_final_steady_state_ms))
    tail_mean = current_mA[idx].mean()

    I_lower_band = tail_mean - tail_mean * band
    I_upper_band = tail_mean + tail_mean * band

    first_idx_inside_band = len(current_mA) - 1
    last_good_i = len(current_mA) - 1
    for i in np.arange(len(current_mA)-1, -1, -1):
        if (current_mA[i] < I_lower_band) or (current_mA[i] > I_upper_band):
            first_idx_inside_band = last_good_i
            break
        last_good_i = i
    
    duration = time_ms[first_idx_inside_band] - time_ms[0]
    return duration


if __name__ == "__main__":
    a = np.array([0, 5, 10, 15, 20])
    b = np.array([1, 2, 4, 4, 4])
    print(switching_duration_inside_band(a, b))
    
    a = np.array([15, 20])
    b = np.array([4, 4])
    print(switching_duration_inside_band(a, b))
    
    a = np.array([5])
    b = np.array([4])
    print(switching_duration_inside_band(a, b))