System dependencies to run the docker image
```````````````````````````````````````````

In order to run the docker image, you only need docker installed on your machine.
The docker image is known to work on MacOS (Intel and Silicon), any Linux, and Windows in the WSL Linux Environment

Please follow this tutorial to install docker depending on your OS : https://docs.docker.com/engine/install/

Get the orfeotoolbox docker image
`````````````````````````````````

    .. code-block:: bash

        docker pull orfeotoolbox/otb:latest

Get an example image
````````````````````

    .. code-block:: bash

        curl https://www.orfeo-toolbox.org/download/data_example.zip
        unzip data_example.zip /Path/to/Unzip/date_example

Apply your first BandMath processing
````````````````````````````````````

    .. code-block:: bash

        docker run orfeotoolbox/otb:latest otbcli_BandMath –il /Path/to/Unzip/data_example/Toulouse.tif –out ndvi.tif –exp “(im1b4-im1b1)/ (im1b4+im1b1)”

**Important Note**:
    If you are on MacOS, there is a trick to use the docker image due to the ARM architecture, it is described in the `README <https://hub.docker.com/r/orfeotoolbox/otb>`_ of the official docker hub image page