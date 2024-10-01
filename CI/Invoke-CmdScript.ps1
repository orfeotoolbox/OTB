#requires -version 5.0

# File takes from https://github.com/Wintellect/WintellectPowerShell/blob/master/Code/Invoke-CmdScript.ps1
# it invoke a CMD script and transfer env variables from cmd to pwsh
# Currently used to setup M$ compilation toolset as M$ does not provide
# powershell script...
###############################################################################
# WintellectPowerShell Module
# Copyright (c) 2010-2017 - John Robbins/Wintellect
# 
# Do whatever you want with this module, but please do give credit.
###############################################################################

# Always make sure all variables are defined and all best practices are 
# followed.
Set-StrictMode -version Latest

function Invoke-CmdScript 
{
<#
.SYNOPSIS
Executes the specified command script and imports the environment into current
PowerShell instance.

.DESCRIPTION
Running development tools at the command line in PowerShell can be a hassle since 
they rely on environment variables and those are set through batch files. This 
cmdlet runs those batch files and imports any set environment variables into
the running PowerShell instance. 

.PARAMETER script
The required batch file to run.

.PARAMETER parameters
The optional parameters to pass to the batch file.

.NOTES
The original script is by Lee Holmes. I updated the script to make removing environment variables
work.

.LINK
http://www.leeholmes.com/blog/2006/05/11/nothing-solves-everything-%e2%80%93-powershell-and-other-technologies/
https://github.com/Wintellect/WintellectPowerShell
#>
    param
    (
        [Parameter(Mandatory=$true,
                   Position=0,
                   HelpMessage="Please specify the command script to execute.")]
        [string] $script, 
        [Parameter(Position=1)]
        [string] $parameters=""
    )  

    # Save off the current environment variables in case there's an issue
    $oldVars = $(Get-ChildItem -Path env:\)
    $tempFile = [IO.Path]::GetTempFileName()  
    
    try
    {
        ## Store the output of cmd.exe.  We also ask cmd.exe to output   
        ## the environment table after the batch file completes  
        cmd /c " `"$script`" $parameters && set > `"$tempFile`" "

        if ($LASTEXITCODE -ne 0)
        {
            throw "Error executing CMD.EXE: $LASTEXITCODE"
        }
        
        # Before we delete the environment variables get the output into a string
        # array.
        $vars = Get-Content -Path $tempFile
    
        # Clear out all current environment variables in PowerShell.
        Get-ChildItem -Path env:\ | Foreach-Object { 
                        set-item -force -path "ENV:\$($_.Name)" -value "" 
                    }
 

        ## Go through the environment variables in the temp file.  
        ## For each of them, set the variable in our local environment.  
        $vars | Foreach-Object {   
                            if($_ -match "^(.*?)=(.*)$")  
                            { 
                                Set-Content -Path "env:\$($matches[1])" -Value $matches[2]
                            } 
                        }
    }
    catch
    {
        "ERROR: $_"

        # Any problems, restore the old environment variables.
        $oldVars | ForEach-Object { Set-Item -Force -Path "ENV:\$($_.Name)" -value $_.Value }
    }
    finally
    {
        Remove-Item -Path $tempFile -Force -ErrorAction SilentlyContinue
    }
}
