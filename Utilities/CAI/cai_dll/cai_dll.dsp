# Microsoft Developer Studio Project File - Name="cai_dll" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=cai_dll - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "cai_dll.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "cai_dll.mak" CFG="cai_dll - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "cai_dll - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "cai_dll - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "cai_dll - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "CAI_DLL_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "CAI_DLL_EXPORTS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40c /d "NDEBUG"
# ADD RSC /l 0x40c /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386

!ELSEIF  "$(CFG)" == "cai_dll - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "CAI_DLL_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "inc" /I "inc/inc_hdf" /I "inc/inc_unix2win" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "CAI_DLL_EXPORTS" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40c /d "_DEBUG"
# ADD RSC /l 0x40c /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 lib/hd421md.lib lib/hm421md.lib lib/libjpeg.lib lib/tiff.lib lib/unix2win.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /nodefaultlib:"libcmtd.lib" /out:"Debug/cai.dll" /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "cai_dll - Win32 Release"
# Name "cai_dll - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\src\cai_arcinfo.c
# End Source File
# Begin Source File

SOURCE=.\src\cai_atg2.c
# End Source File
# Begin Source File

SOURCE=.\src\cai_bandeau.c
# End Source File
# Begin Source File

SOURCE=.\src\cai_bil.c
# End Source File
# Begin Source File

SOURCE=.\src\cai_bip.c
# End Source File
# Begin Source File

SOURCE=.\src\cai_bsq.c
# End Source File
# Begin Source File

SOURCE=.\src\cai_DonAux.c
# End Source File
# Begin Source File

SOURCE=.\src\cai_fonctions.c
# End Source File
# Begin Source File

SOURCE=.\src\cai_gessimu.c
# End Source File
# Begin Source File

SOURCE=.\src\cai_grilles.c
# End Source File
# Begin Source File

SOURCE=.\src\cai_hdf.c
# End Source File
# Begin Source File

SOURCE=.\src\cai_inria.c
# End Source File
# Begin Source File

SOURCE=.\src\cai_jpeg.c
# End Source File
# Begin Source File

SOURCE=.\src\cai_lum.c
# End Source File
# Begin Source File

SOURCE=.\src\cai_mapsat.c
# End Source File
# Begin Source File

SOURCE=.\src\cai_multis.c
# End Source File
# Begin Source File

SOURCE=.\src\cai_onera.c
# End Source File
# Begin Source File

SOURCE=.\src\cai_radar.c
# End Source File
# Begin Source File

SOURCE=.\src\cai_rasterfile.c
# End Source File
# Begin Source File

SOURCE=.\src\cai_spot.c
# End Source File
# Begin Source File

SOURCE=.\src\cai_tiff.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\inc\cai_image.h
# End Source File
# Begin Source File

SOURCE=.\inc\cderror.h
# End Source File
# Begin Source File

SOURCE=.\inc\cdjpeg.h
# End Source File
# Begin Source File

SOURCE=.\inc\g3states.h
# End Source File
# Begin Source File

SOURCE=.\inc\jchuff.h
# End Source File
# Begin Source File

SOURCE=.\inc\jconfig.h
# End Source File
# Begin Source File

SOURCE=.\inc\jdct.h
# End Source File
# Begin Source File

SOURCE=.\inc\jdhuff.h
# End Source File
# Begin Source File

SOURCE=.\inc\jerror.h
# End Source File
# Begin Source File

SOURCE=.\inc\jinclude.h
# End Source File
# Begin Source File

SOURCE=.\inc\jmemsys.h
# End Source File
# Begin Source File

SOURCE=.\inc\jmorecfg.h
# End Source File
# Begin Source File

SOURCE=.\inc\jpegint.h
# End Source File
# Begin Source File

SOURCE=.\inc\jpeglib.h
# End Source File
# Begin Source File

SOURCE=.\inc\jversion.h
# End Source File
# Begin Source File

SOURCE=.\inc\machdep.h
# End Source File
# Begin Source File

SOURCE=.\inc\port.h
# End Source File
# Begin Source File

SOURCE=.\inc\t4.h
# End Source File
# Begin Source File

SOURCE=.\inc\tif_dir.h
# End Source File
# Begin Source File

SOURCE=.\inc\tif_fax3.h
# End Source File
# Begin Source File

SOURCE=.\inc\tif_predict.h
# End Source File
# Begin Source File

SOURCE=.\inc\tiff.h
# End Source File
# Begin Source File

SOURCE=.\inc\tiffcomp.h
# End Source File
# Begin Source File

SOURCE=.\inc\tiffconf.h
# End Source File
# Begin Source File

SOURCE=.\inc\tiffio.h
# End Source File
# Begin Source File

SOURCE=.\inc\tiffiop.h
# End Source File
# Begin Source File

SOURCE=.\inc\uvcode.h
# End Source File
# Begin Source File

SOURCE=.\inc\version.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
