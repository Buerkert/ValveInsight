import scipy.stats
import numpy as np

def switching_detection_tail_slope(time:np.array, current:np.array):
    """
    Calculate tail mean from last 10 ms in data starting from a duration of 20 ms. 
    Check if tail is constant enough. If not increase duration by 1 ms and move window. 
    """

    t_mean_slope_ms = 10
    t_duration_ms = 20
    
    tail_const = False
    while tail_const == False:
        I = current[time < t_duration_ms]
        t = time[time < t_duration_ms]
        
        res = scipy.stats.linregress(t[t >  t.max()-t_mean_slope_ms], I[t >  t.max()-t_mean_slope_ms])
        tail_slope = res[0]

        if tail_slope < 0.002:
            tail_const = True
        elif t_duration_ms+1 > time.max():
            break
        else:
            t_duration_ms += 1

    tail_mean = I[t >  t.max()-t_mean_slope_ms].mean()

    current = current[time < t_duration_ms]
    time = time[time < t_duration_ms]

    current_inv = tail_mean - current
    current_inv_norm = current_inv / tail_mean

    time_step_ms = time[1] - time[0]

    integral = current_inv_norm.sum()*time_step_ms
    integral = np.round(integral, 4)
    return integral, tail_mean
