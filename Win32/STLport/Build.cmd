@ECHO OFF
REM ************************************************************
REM
REM Configure and build STLport for Visual C++.
REM
REM ************************************************************

:check_args
IF /I "%1" == "" GOTO :invalid_args

SETLOCAL
SET CWD=%CD%
CD %1

REM
REM Setup environment variables for VC++.
REM
:set_vars
CALL "%VS71COMNTOOLS%\vsvars32.bat"
IF errorlevel 1 GOTO :failed

CALL "%CWD%\SetVars.cmd"
IF errorlevel 1 GOTO :failed

REM
REM Configure STLport build.
REM
:configure
CD build\lib
IF errorlevel 1 GOTO :failed

CALL configure --compiler msvc71 --extra-cxxflag /Zc:wchar_t --extra-cxxflag /Zc:forScope
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

:invalid_args
ECHO Usage: Build [STLport root folder]
GOTO :done

:failed
ECHO Failed to build STLport.
GOTO :cleanup

:cleanup
CD %CWD%

:done
