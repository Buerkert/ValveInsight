<img src="images/logo.png" alt="Bürkert Fluid Control Systems" width="240">


# ValveInsight - Switching analysis

This repository presents selected algorithms from the **ValveInsight** portfolio, focused on the evaluation and analysis of valve switching behavior based on inrush current signals.  

## Overview

- **Switching Fault Detection**  
  Calculate a value that indicates if a valve has successfully completed its inrush based switching operation or not.
  
- **Switching Duration Detection**  
  Calculate the duration of valve switching operation.

The sample data included in this repository was recorded using the ValveInsight Evaluation Kit. For comparison, we also provide example curves captured using high-end laboratory instrumentation.

If you have specific requirements or are interested in a tailored solution, please don’t hesitate to [contact us](https://www.buerkert.de/de/landingpage/zuverlaessige-analysegeraete-durch-diagnose-mit-valveinsight).


## Getting Started

The algorithms are available in both Python and C, allowing flexible integration into various environments.  

### Python
The [python implementation](algorithms_python) is delivered as a standard python package with unit tests. 
A configuration file for [Visual Studio Code](https://code.visualstudio.com/) is included, allowing you to run the Python unit tests easily within the IDE. 

### C
The [C implementation](algorithms_c) includes unit tests and comes with a project structure compatible with [PlatformIO](https://platformio.org) for convenience. 
The provided platformio.ini file is configured to run on a local PC so that the unit tests can execute.
If you want to test on embedded hardware, you need to adjust the [configuration](https://docs.platformio.org/en/latest/projectconf/index.html).  
While PlatformIO is provided as an example, you can easily adapt the code to your preferred build system or project structure as each algorithm is self contained in a dedicated header/source file pair. 


### Jupyter
Interactive [Jupyter Notebooks](https://jupyter.org) are provided for demonstration and visualization. View them in github or Visual Studio Code. If necessary, run them in a Jupyter environment. A Dockerfile is included to support environment setup if required. 

- **[Getting started notebook](01%20-%20Switching%20analysis%20-%20Getting%20started.ipynb):** A guided introduction to the core algorithms. 

- **[Advanced notebook](02%20-%20Switching%20analysis%20-%20Advanced.ipynb):** Explores more sophisticated use cases and edge-case handling.


## Additional Resources

More information is available on our [ValveInsight landing page](https://www.buerkert.de/de/landingpage/zuverlaessige-analysegeraete-durch-diagnose-mit-valveinsight). Register to download a whitepaper with technical details. 


## License

Please refer to the [LICENSE](LICENSE.md) file for licensing details.
