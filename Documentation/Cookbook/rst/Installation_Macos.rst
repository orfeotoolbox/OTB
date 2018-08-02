We provide for MacOS X through a standalone package. This package is a
self-extractible archive, quite similar to the Linux one. You may
uncompress it with the command line:

.. parsed-literal::

    chmod +x OTB-|release|-Darwin64.run
    ./OTB-|release|-Darwin64.run

Once the archive is extracted, you can see OTB-|release|-Darwin64 directory in
the same direcrtory along with OTB-|release|-Darwin64.run

Contents of OTB-|release|-Darwin64 is briefly listed below:

-  ``Monteverdi.app``: A Mac OSX .app for Monteverdi

-  ``Mapla.app``: A Mac OSX .app for Mapla.

-  ``bin``: A folder containing application launchers (otbcli.sh,
   otbgui.sh), monteverdi and mapla binaries.

-  ``lib``: A folder containing all shared libraries and OTB
   applications.

-  ``include``: A folder containing all the necessary headers to compile OTB
   based projects.

-  ``share``: A folder containing common resources and copyright
   mentions.

-  ``tool``: A folder containing useful scripts to test the installation or
   to uninstall OTB libraries and headers while keeping all the dependencies.

The applications can be launched from the Mapla launcher. If you want to
use the otbcli and otbgui launchers, you can initialize your environment
with ``source otbenv.profile``.

The package can be used to compile other projects using OTB (binaries, libraries
and headers are included). If you want to build OTB from source using this
package, you should first uninstall the specific OTB files from the package to
leave only the dependencies (what we call an XDK). You can do it using the
supplied script ``tools/uninstall_otb.sh``.

Python bindings
~~~~~~~~~~~~~~~

Starting from OTB 5.8.0, OTB bindings for Python 2.7 are distributed as a binary
package. With OTB 6.4.0, additional bindings for Python 3.5 are also included.
Please note that using a different Python version may not be compatible with
OTB wrappings. If no compatible Python 2.x version is found a
notification is generated during the installation process.  If the installation completes
without issue, information relating to your Python bindings will be provided. 

You must have Python numpy bindings installed in your system. They can be installed locally
without admin rights as follows: "pip install --user numpy". This is to give users the option 
to select their own existing Python installation rather than the one dibstributed by the OTB package.

By default, bindings for Python 2.7 will be enabled with the ``otbenv`` script.
If you want to use bindings for Python 3.5, you can copy this script and modify:

- ``lib/python`` into ``lib/python3``, for variable PYTHONPATH

Notes:

-  If you want to use the otbcli and otbgui launchers, you must access
   them via a terminal prompt.

-  The OSX .app are provided for monteverdi (viewer) and mapla
   (application browser).

-  You must use monteverdi and mapla through their .app files only.

-  You are allowed to move these .app files and refrain from moving or
   deleting OTB-|release|-Darwin64 after extraction. In case you need to have OTB
   installed in some other directory. Extract the .run file there.

FAQ
~~~

Q: I am getting an error message...
+++++++++++++++++++++++++++++++++++

::

   xcrun: error: invalid active developer path
   (/Library/Developer/CommandLineTools), missing xcrun at:
   /Library/Developer/CommandLineTools/usr/bin/xcrun

A: You can get this error at startup running Monteverdi.app or Mapla.app. The
solution is to run in a terminal the following command:  

::

   xcode-select --install

And then try to restart Monteverdi or Mapla. 
