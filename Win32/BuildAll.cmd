@ECHO OFF
REM ************************************************************
REM
REM Script to build all solutions for Debug and/or Release
REM targets.
REM
REM NB: See Build.cmd for notes about setiing the environment
REM variables up first.
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
REM Do builds
REM

:do_builds
FOR /R %%I IN (*.sln) DO ECHO %%I & CALL Build %1 %2 %%I
GOTO :done

REM 
REM Report errors.
REM

:invalid_args
ECHO Usage: BuildAll [vc71 or vc80 or vc90] [debug or release or both]
GOTO :done

:invalid_vars
ECHO ERROR: Compiler environment variables not set. Run 'SetVars' first.
GOTO :done

:done
