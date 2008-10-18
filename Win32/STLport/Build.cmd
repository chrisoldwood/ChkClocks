@ECHO OFF
REM ************************************************************
REM
REM Configure and build STLport for Visual C++.
REM
REM ************************************************************

REM 
REM Check command line..
REM

:check_args
IF /I "%1" == "" GOTO :invalid_args
IF /I "%2" == "" GOTO :invalid_args

REM
REM Setup environment variables for VC++/STLport.
REM

SETLOCAL

CALL "SetVars.cmd" %1 %2
IF errorlevel 1 GOTO :failed

SET CWD=%CD%
CD %2

REM 
REM Determine compiler.
REM

:set_compiler
IF /I "%1" == "vc71" GOTO :do_vc71
IF /I "%1" == "vc80" GOTO :do_vc80
IF /I "%1" == "vc90" GOTO :do_vc90
GOTO :invalid_args

:do_vc71
SET COMPILER=msvc71
GOTO :configure

:do_vc80
SET COMPILER=msvc8
GOTO :configure

:do_vc90
SET COMPILER=msvc9
GOTO :configure

REM
REM Configure STLport build.
REM

:configure
CD build\lib
IF errorlevel 1 GOTO :failed

CALL configure --compiler %COMPILER% --extra-cxxflag /Zc:wchar_t --extra-cxxflag /Zc:forScope
IF errorlevel 1 GOTO :failed

REM
REM Build it.
REM

:build
nmake /f msvc.mak
IF errorlevel 1 GOTO :failed

nmake /f msvc.mak install
IF errorlevel 1 GOTO :failed

GOTO :cleanup

REM
REM Report errors.
REM

:invalid_args
ECHO Usage: Build [vc71 or vc80 or vc90] [STLport root folder]
GOTO :done

:failed
ECHO Failed to build STLport.
GOTO :cleanup

:cleanup
CD %CWD%

:done
