@ECHO OFF
REM ************************************************************
REM
REM Script to upgrade all VC++ Project files to the specifed
REM Visual C++ version. Due to the Express versions not shipping
REM with a full version of DEVENV the Solution files have to be
REM upgraded using the IDE.
REM
REM NB: You must set the environment variables first by running
REM the SetVars script.
REM
REM ************************************************************

REM 
REM Check command line..
REM

:check_args
IF /I "%1" == "" GOTO :invalid_args
IF /I "%2" == "" GOTO :invalid_args

REM 
REM Check environment variables are set.
REM

:check_vars
IF /I "%VCVERSION%" == "" GOTO :invalid_vars

REM 
REM Determine compiler.
REM

:set_compiler
IF /I "%1" == "vc80" GOTO :do_vc80
IF /I "%1" == "vc90" GOTO :do_vc90
GOTO :invalid_args

:do_vc80
SET VCVERSION=VC80
GOTO :set_tool

:do_vc90
SET VCVERSION=VC80
GOTO :set_tool

REM
REM Determine product type.
REM

:set_tool
IF /I "%2" == "full"    GOTO :do_upgrade
IF /I "%2" == "express" GOTO :do_upgrade
GOTO :invalid_args

REM
REM Make files writable.
REM

:do_upgrade
ATTRIB /s -r *.sln
ATTRIB /s -r *.vcproj

REM
REM Upgrade files.
REM

:do_projects
FOR /R %%I IN (*.vcproj) DO ECHO %%I & vcbuild /upgrade %%I

:do_solutions
IF /I "%2" == "express" GOTO :do_vcexpress
FOR /R %%I IN (*.sln) DO ECHO %%I && devenv /upgrade %%I
GOTO :done

:do_vcexpress
FOR /R %%I IN (*.sln) DO ECHO %%I & vcexpress %%I
GOTO :done

REM 
REM Report errors.
REM

:invalid_args
ECHO Usage: UpgradeAll [vc80 or vc90] [full or express]
GOTO :done

:invalid_vars
ECHO ERROR: Compiler environment variables not set. Run 'SetVars' first.
GOTO :done

:done
