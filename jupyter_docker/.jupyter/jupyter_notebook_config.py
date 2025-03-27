# Configuration file for jupyter-notebook.

## (bytes/sec) Maximum rate at which messages can be sent on iopub before they
#  are limited.
c.NotebookApp.iopub_data_rate_limit = 100000000
# trust all notebooks
c.NotebookApp.token = ''
c.NotebookApp.password = ''
# shutdown kernels after no activity
c.MappingKernelManager.cull_idle_timeout = 60 * 60 * 24  # Time in seconds
# check for idle kernels every two minutes
c.MappingKernelManager.cull_interval = 2 * 60  # Time in seconds
# disable quit button on dashboard
c.NotebookApp.quit_button = False
