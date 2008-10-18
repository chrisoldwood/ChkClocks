@ECHO OFF
REM ************************************************************
REM
REM Script to run the vsvars32.bat for the specified compiler.
REM
REM VC++ 7.1: "%VS71COMNTOOLS%\vsvars32.bat"
REM VC++ 8.0: "%VS80COMNTOOLS%\vsvars32.bat"
REM VC++ 9.0: "%VS90COMNTOOLS%\vsvars32.bat"
REM
REM ************************************************************

REM 
REM Check command line..
REM

:check_args
IF /I "%1" == "" GOTO :invalid_args

REM 
REM Remove existing variables.
REM

:check_vars
IF /I "%VCVERSION%" == "" GOTO :clean
ECHO.
ECHO WARNING: Overwriting variables for '%VCVERSION%'
ECHO.

:clean
SET INCLUDE=
SET LIB=
SET SOURCE=

REM 
REM Determine compiler.
REM

:set_compiler
IF /I "%1" == "vc71" GOTO :do_vc71
IF /I "%1" == "vc80" GOTO :do_vc80
IF /I "%1" == "vc90" GOTO :do_vc90
GOTO :invalid_args

REM 
REM Setup variables.
REM

:do_vc71
CALL "%VS71COMNTOOLS%\vsvars32.bat"
SET VCVERSION=vc71
GOTO :done

:do_vc80
CALL "%VS80COMNTOOLS%\vsvars32.bat"
SET VCVERSION=vc80
GOTO :done

:do_vc90
CALL "%VS90COMNTOOLS%\vsvars32.bat"
SET VCVERSION=vc90
GOTO :done

REM 
REM Report errors.
REM

:invalid_args
ECHO Usage: SetVars [vc71 or vc80 or vc90]
GOTO :done

:done
