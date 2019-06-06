<#
.SYNOPSIS
    .
.DESCRIPTION
    .
.PARAMETER ARCH
    Specify building 32bit or 64bit using x86 or x64. Based on this value we setup vs develop environment using vcvarsall.bat 
    Possible Values are: 'x86' or 'x64'. 
.PARAMETER PROJECT
    Choose to build either otb project (using existing xdk) or build actual xdk containing all dependencies of OTB.
    Possible Values are: 'xdk' or 'otb'
.PARAMETER SHORT_TARGET
    This is used to select a target winsdk version. It can be either 8.1 or 10. Default is 10
.EXAMPLE
    C:\PS> 
    dev_env.ps1 -project otb
    dev_env.ps1 -project otb -arch x64 -vc_version 14.16
    <Description of example>
.NOTES
    Author: OTB Team
    Date:   June 6, 2019
#>

<# 

:Help
setlocal
echo "Usage: dev_env.bat <compiler_arch>  <project>  [<target-os>]  [<vc_version>] [<build-type>]"
echo "  <compiler_arch> : 'x86' | 'x64'"
echo "  <project>       : 'xdk' | 'otb'"
echo "  <target-os>     : '8.1' | '10' (default)"
echo "  <vc_version>    :"
echo "    '14.20' (i.e. VS 2019)"
echo "    '14.16' (i.e. VS 2017)"
echo "    '14.0'  (i.e. VS 2015) (default)"
echo "  <build>         : 'Debug' | 'Release' (default)"
endlocal
 #>


param([string] $ARCH,
      [string] $PROJECT,
	  [int32]  $SHORT_TARGET = 10,
      [string] $BUILD_TYPE = "Release",
	  [string] $VCVER = "14.0"
)

$TARGET = $SHORT_TARGET
If($SHORT_TARGET -eq "10") { $TARGET = "10.0.17763.0"}

Write-Output "CLCACHE_DIR = $PROJECT-$BUILD_TYPE-$ARCH-$TARGET-$VCVER"

#:: Setup home dir (so that ssh configuration works fine)
if ( $Env:USERNAME -eq "otbbot" ) {
$Env:HOMEDRIVE=C:
$Env:HOMEPATH="\Users\otbbot"
}
Write-Output "Home dir: $Env:HOMEDRIVE$Env:HOMEPATH"

# Setup Python
$Env:PATH += ";C:\tools\Python35-$ARCH;C:\tools\Python35-$ARCH\Scripts"


# Setup GL dlls (TODO: recheck this)
$Env:PATH += ";C:\tools\GL\$ARCH\bin"

# Setup compiler
pushd "C:\Program Files (x86)\Microsoft Visual Studio\2019\BuildTools\VC\Auxiliary\Build"
cmd /c "vcvarsall.bat $ARCH $TARGET -vcvars_ver=$VCVER & set" | foreach {
  if ($_ -match "=") {
    $v = $_.split("="); set-item -force -path "ENV:\$($v[0])"  -value "$($v[1])"
  }
}
popd

Write-Host "`nVisual Studio 2017 Command Prompt variables set." -ForegroundColor Yellow

Write-Host "Configuring Clcache..."

CLCACHE_ROOT_DIR="C:\clcache"
$Env:CLCACHE_DIR="$CLCACHE_ROOT_DIR\$PROJECT-$BUILD_TYPE-$ARCH-$TARGET-$VCVER"
$Env:CLCACHE_HARDLINK=1

#### set CLCACHE_SERVER=1
#### for /F "delims=" %%a in ('where cl.exe') do @if defined CLCACHE_CL (break ) else (set CLCACHE_CL=%%a)

$Env:CLCACHE_CL=(get-command "cl.exe").Path
Write-Output " CL path: $Env:CLCACHE_CL"

Write-Output "Installing clcache.exe as cl.exe"
Copy-Item "C:\tools\Python35-$ARCH\Scripts\clcache.exe" -Destination "$CLCACHE_ROOT_DIR\cl.exe"
$Env:PATH = "$CLCACHE_ROOT_DIR;$Env:PATH"

Write-Output "We need to change cache max size: clcache -M <size-in-bytes>"
if ($PROJECT -eq "xdk") {
clcache.exe -M 3000000000
}
if ($PROJECT -eq "otb") {
#call "clcache.exe" -M 2000000000
clcache.exe -M 2000000000
}

clcache.exe -s

$IMAGE_NAME="windows-$SHORT_TARGET-$BUILD_TYPE-$ARCH-vc$VCVER"
Write-Output "Generated IMAGE_NAME: $IMAGE_NAME"

Write-Output "setup path to perl, but add it last ... (there is a libstdc++.dll in that folder...)"
$Env:PATH +="C:\tools\perl\perl\bin"

