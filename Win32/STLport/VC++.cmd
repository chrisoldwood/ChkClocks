@ECHO OFF
REM ************************************************************
REM
REM Launch Visual C++ with the environment variables configured
REM for use with STLport.
REM
REM ************************************************************

REM 
REM Check command line..
REM

:check_args
IF /I "%1" == "" GOTO :invalid_args
IF /I "%2" == "" GOTO :invalid_args

REM
REM Setup environment variables for VC++.
REM

SETLOCAL

CALL SetVars.cmd %1 %2
IF errorlevel 1 GOTO :set_vars_failed

REM
REM Start VC++.
REM

start devenv.exe /useenv
IF errorlevel 1 GOTO :devenv_failed

GOTO :done

REM
REM Report errors.
REM

:invalid_args
ECHO Usage: VC++ [vc71 or vc80 or vc90] [STLport root folder]
GOTO :done

:set_vars_failed
ECHO ERROR: Failed to set the environment variables
GOTO :done

:devenv_failed
ECHO ERROR: Failed to start Visual C++
GOTO :done

:done
