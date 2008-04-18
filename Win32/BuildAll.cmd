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

:check_args
IF /I "%1" == "" GOTO :invalid_args
IF /I "%2" == "" GOTO :invalid_args

REM
REM Do all builds
REM
:do_builds
FOR /R %%I IN (*.sln) DO CALL Build %1 %2 %%I
goto :done

:invalid_args
ECHO Usage: BuildAll [vc71 or vc80 or vc90] [debug or release or both]

:done
