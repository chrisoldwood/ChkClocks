@ECHO OFF
REM ************************************************************
REM
REM Setup the environment variables for STLport. We have to
REM ensure that the STLport include path precedes the Visual C++
REM one.
REM
REM ************************************************************

:check_args
IF /I "%1" == "" GOTO :invalid_args

SET INCLUDE=%1\stlport;%INCLUDE%
SET LIB=%1\lib;%LIB%
SET PATH=%PATH%;%1\bin;

GOTO :done

:invalid_args
ECHO Usage: SetVars [STLport root folder]
GOTO :done

:done
