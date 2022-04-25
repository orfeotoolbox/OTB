FROM ubuntu:20.04 
ARG OTB_RELEASE=8.0.0

# Install system dependencies
COPY system-dependencies.txt .
RUN apt-get update -y \
 && apt-get upgrade -y \
 && cat system-dependencies.txt | DEBIAN_FRONTEND=noninteractive xargs apt-get install -y --no-install-recommends \
 && apt-get clean \
 && rm -rf /var/lib/apt/lists/* system-dependencies.txt

# Install pre-compiled OTB binaries
RUN curl https://www.orfeo-toolbox.org/packages/OTB-$OTB_RELEASE-Linux64.run -o OTB-$OTB_RELEASE-Linux64.run \
 && chmod +x OTB-$OTB_RELEASE-Linux64.run \
 && ./OTB-$OTB_RELEASE-Linux64.run --target /opt/otb \
 && rm OTB-$OTB_RELEASE-Linux64.run

# Symlink Python library to a known path for OTB
RUN ln -s /usr/lib/x86_64-linux-gnu/libpython3.8.so /opt/otb/lib/libpython3.8.so.rh-python38-1.0

# Persistant env
ENV CMAKE_PREFIX_PATH=/opt/otb
ENV PYTHONPATH=/opt/otb/lib/python
ENV GDAL_DRIVER_PATH=disable
ENV GDAL_DATA=/opt/otb/share/gdal
ENV PATH=/opt/otb/bin:$PATH
ENV OTB_APPLICATION_PATH=/opt/otb/lib/otb/applications
ENV LC_NUMERIC=C

# Default command fo docker run or docker create
CMD /bin/bash
