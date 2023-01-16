#
# Copyright (C) 2005-2022 Centre National d'Etudes Spatiales (CNES)
#
# This file is part of Orfeo Toolbox
#
#     https://www.orfeo-toolbox.org/
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
FROM ubuntu:20.04
ARG OTB_RELEASE=8.1.1

# Install system dependencies
COPY system-dependencies.txt .
RUN apt-get update -y \
 && apt-get upgrade -y \
 && cat system-dependencies.txt | DEBIAN_FRONTEND=noninteractive xargs apt-get install -y --no-install-recommends \
 && apt-get clean \
 && rm -rf /var/lib/apt/lists/* system-dependencies.txt

# Install pre-compiled OTB binaries
RUN curl https://www.orfeo-toolbox.org/packages/archives/OTB/OTB-$OTB_RELEASE-Linux64.run -o OTB-$OTB_RELEASE-Linux64.run \
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
ENV PROJ_LIB=/opt/otb/share/proj
ENV PATH=/opt/otb/bin:$PATH
ENV OTB_APPLICATION_PATH=/opt/otb/lib/otb/applications
ENV LC_NUMERIC=C

# Default command when using 'docker run' or 'docker create'
CMD /bin/bash
