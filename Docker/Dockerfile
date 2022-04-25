FROM ubuntu:20.04 

RUN echo "Europe/Paris" > /etc/timezone

RUN apt-get update -y \
 && apt-get upgrade -y \
 && apt-get install -y --no-install-recommends \
        ca-certificates \
        curl \
        libxcb1 \
        libxcb-composite0 \
        libxcb-glx0 \
        libxcb-icccm4 \
        libxcb-image0 \
        libxcb-keysyms1 \
        libxcb-randr0 \
        libxcb-render0 \
        libxcb-render-util0 \
        libxcb-util1 \
        libxcb-shm0 \
        libxcb-xfixes0 \
        libxcb-xinerama0 \
        libxcb-xinput0 \
        libxcb-xkb1 \
        libxcb-shape0 \
        libx11-xcb1 \
        libglu1-mesa \
        libxrender1 \
        libxi6 \
        libxkbcommon0 \
        libxkbcommon-x11-0 \
        libxinerama1 \
        python3 \ 
        python3-dev \
        python3-numpy \
        libtool \
 && rm -rf /var/lib/apt/lists/*
 
RUN curl https://www.orfeo-toolbox.org/packages/OTB-8.0.0-Linux64.run -o OTB-8.0.0-Linux64.run \
&& chmod +x OTB-8.0.0-Linux64.run \
&& ./OTB-8.0.0-Linux64.run --target /opt/otb

# Symlink Python library to a known path for OTB
RUN ln -s /usr/lib/x86_64-linux-gnu/libpython3.8.so /opt/otb/lib/libpython3.8.so.rh-python38-1.0

# Persistant env
ENV CMAKE_PREFIX_PATH=/opt/otb
ENV PYTHONPATH=/opt/otb/lib/python
ENV GDAL_DRIVER_PATH=disable
ENV GDAL_DATA=/opt/otb/share/gdal
ENV PATH=/opt/otb/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin
ENV OTB_APPLICATION_PATH=/opt/otb/lib/otb/applications
ENV LC_NUMERIC=C

# Default command fo docker run or docker create
CMD /bin/bash
