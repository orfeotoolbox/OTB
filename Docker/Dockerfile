#
# Copyright (C) 2005-2024 Centre National d'Etudes Spatiales (CNES)
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
ARG OTB_RELEASE=9.0.0

# Install system dependencies
COPY Docker/system-dependencies.txt .
RUN apt-get update -y \
 && apt-get upgrade -y \
 && cat system-dependencies.txt | DEBIAN_FRONTEND=noninteractive xargs apt-get install -y --no-install-recommends \
 && apt-get clean \
 && rm -rf /var/lib/apt/lists/* system-dependencies.txt

# Install pre-compiled OTB binaries
RUN curl https://www.orfeo-toolbox.org/packages/archives/OTB/OTB-$OTB_RELEASE-Linux.tar.gz -o OTB-$OTB_RELEASE-Linux.tar.gz

RUN tar xvf OTB-$OTB_RELEASE-Linux.tar.gz --one-top-level="/app/otb" \
&& rm OTB-$OTB_RELEASE-Linux.tar.gz

# Persistent env
ENV CMAKE_PREFIX_PATH=/app/otb
ENV PYTHONPATH=/app/otb/lib/python3/dist-packages:/app/otb/lib/otb/python
ENV GDAL_DRIVER_PATH=disable
ENV GDAL_DATA=/app/otb/share/gdal
ENV PROJ_LIB=/app/otb/share/proj
ENV PATH=/app/otb/bin:$PATH
ENV LD_LIBRARY_PATH=/app/otb/lib:$LD_LIBRARY_PATH
ENV OTB_APPLICATION_PATH=/app/otb/lib/otb/applications
ENV LC_NUMERIC=C
ENV OTB_INSTALL_DIR=/app/otb

# run post install to apply sed commands on cmake files and to get python
# bindings if needed
RUN sh /app/otb/tools/post_install.sh

# Default command when using 'docker run' or 'docker create'
CMD /bin/bash
