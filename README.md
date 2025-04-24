<img src="images/logo.png" alt="Bürkert Fluid Control Systems" width="240">


# Valve insight - Switching analysis

This repository presents selected algorithms from the **Valve Insight** portfolio, focused on the evaluation and analysis of valve switching behavior based on inrush current signals.  

## Overview

- **Switching Fault Detection**  
  Calculate a value that indicates if a valve has successfully completed its switching operation or not.
  
- **Switching Duration Detection**  
  Calculate the duration of valve switching operation.

The sample data included in this repository was recorded using the Valve Insight Evaluation Kit. For comparison, we also provide example curves captured using high-end laboratory instrumentation.

If you have specific requirements or are interested in a tailored solution, please don’t hesitate to contact us.


## Getting Started

The algorithms are available in both Python and C, allowing flexible integration into various environments.  

**Python**
Interactive [Jupyter Notebooks](https://jupyter.org) are provided for demonstration and visualization. Run them by using the provided Dockerfile or in any local Jupyter environment. 

- **[Getting started notebook](01%20-%20Switching%20analysis%20-%20Getting%20started.ipynb):** A guided introduction to the core algorithms. 

- **[Advanced notebook](02%20-%20Switching%20analysis%20-%20Advanced.ipynb):** Explores more sophisticated use cases and edge-case handling.


**C**
The C implementations are delivered as a [PlatformIO](https://platformio.org) project, complete with unit tests for verification and validation.


### Additional Resources

More information is available on our [Valve Insight landing page](https://www.buerkert.de/de/landingpage/zuverlaessige-analysegeraete-durch-diagnose-mit-valveinsight).


### License

Please refer to the [LICENSE](LICENSE.md) file for licensing details.
