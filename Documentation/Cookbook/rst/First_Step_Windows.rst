Windows binary packages are available for Windows 10 64bits or higher.
In order to run properly, you need to make sure you installed the required system dependencies

Installation of required dependencies
+++++++++++++++++++++++++++++++++++++

- You must have "Visual C++ Redistributable for Visual Studio 2019" installed for using this package.
  It can be downloaded freely from `microsoft <https://aka.ms/vs/16/release/vc_redist.x64.exe>`_

- If you plan to run the graphical interface, monteverdi could crash if you run Windows on a VM because of the lack of OpenGL support.
  to cope with that problem you can `download here <https://downloads.fdossena.com/geth.php?r=mesa64-latest>`__ the MESA project opensource OpenGL dll , and put it in the 'bin' folder of the OTB installation, before launching any OTB graphical app
