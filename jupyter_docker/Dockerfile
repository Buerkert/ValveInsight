FROM python:3.12

# apt-get upgrade is run to patch known vulnerabilities
# in packages that the base image doesn't have yet
RUN apt-get update --yes && \
    apt-get upgrade --yes && \
    apt-get install --yes --no-install-recommends \
      git \
      vim \
      wget && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/*


# Build as user

RUN useradd --create-home --shell /bin/bash ganymede
USER ganymede

WORKDIR /home/ganymede
COPY --chown=ganymede:ganymede requirements.txt ./requirements.txt
COPY --chown=ganymede:ganymede entrypoint.sh ./entrypoint.sh
COPY --chown=ganymede:ganymede .jupyter ./.jupyter/

# Paths for local install of python binaries
RUN echo 'export PATH=$HOME/.local/bin:$PATH' >> ~/.profile && \
    echo 'export PATH=$HOME/.local/bin:$PATH' >> ~/.bashrc

# Install jupyterlab
RUN python -m pip install --upgrade pip &&\
    pip install -r requirements.txt &&\
    python -m ipykernel install --user --name=DS-2024


# Link notebook dir
WORKDIR /home/ganymede
RUN ln -s /mnt/jupyter-notebooks

EXPOSE 8080

#ENTRYPOINT ["/home/ganymede/entrypoint.sh"]
