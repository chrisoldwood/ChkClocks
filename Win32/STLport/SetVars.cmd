@ECHO OFF
REM ************************************************************
REM
REM Setup the environment variables for STLport. We have to
REM ensure that the STLport include path precedes the Visual C++
REM one.
REM
REM ************************************************************

REM 
REM Check command line..
REM

:check_args
IF /I "%1" == "" GOTO :invalid_args
IF /I "%2" == "" GOTO :invalid_args

REM
REM Setup the Visual Studio environment variables.
REM

CALL ..\SetVars.cmd %1
IF errorlevel 1 GOTO :set_vars_failed

REM
REM Prepend the STLport environment variables.
REM

SET INCLUDE=%2\stlport;%INCLUDE%
SET LIB=%2\lib;%LIB%
SET PATH=%PATH%;%2\bin;

GOTO :done

REM
REM Report errors.
REM

:set_vars_failed
ECHO ERROR: Failed to set the environment variables
GOTO :done

:invalid_args
ECHO Usage: SetVars [vc71 or vc80 or vc90] [STLport root folder]
GOTO :done

:done
