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

Since OTB 6.7.0 OTB bindings for Python 3.7 are distributed as a binary
package.
Please note that using a different Python version may not be compatible with
OTB wrappings. If the installation completes
without issue, information relating to your Python bindings will be provided. 

You must have Python numpy bindings installed in your system. They can be installed locally
without admin rights as follows: "pip install --user numpy". This is to give users the option 
to select their own existing Python installation rather than the one dibstributed by the OTB package.

By default, bindings for Python 3.7 will be enabled with the ``otbenv`` script.

Recompiling Python bindings
+++++++++++++++++++++++++++

If you are using another version of Python 3 than 3.7, but still want to use OTB Python bindings, it is possible
to compile the python bindings again with your version of Python. CMake is required (it is available in Brew or at [https://cmake.org/]). At the root of the OTB installation run :

.. parsed-literal::

    source otbenv.profile 
    ctest -S share/otb/swig/build_wrapping.cmake -VV

You should now be able to import ``otbApplication`` through Python !

Notes:
~~~~~~

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
