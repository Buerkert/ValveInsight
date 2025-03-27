#!/bin/bash
nohup .local/bin/jupyter-lab --ip=0.0.0.0 --port 8080 --notebook-dir="~/jupyter-notebooks" --no-browser --NotebookApp.allow_origin='*' 1> jupyter.log 2> jupyter.err &