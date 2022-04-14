#!/bin/bash
ln -s /usr/lib/x86_64-linux-gnu/libpython3.8.so /opt/otb/lib/libpython3.8.so.rh-python38-1.0
source /opt/otb/otbenv.profile
exec /bin/bash
