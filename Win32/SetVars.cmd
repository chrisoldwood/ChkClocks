@ECHO OFF
REM ************************************************************
REM
REM Script to run the vsvars32.bat for the specified compiler.
REM
REM VC++ 7.1: "%VS71COMNTOOLS%\vsvars32.bat"
REM VC++ 8.0: "%VS80COMNTOOLS%\vsvars32.bat"
REM
REM ************************************************************

:check_compiler
IF /I "%1" == "" GOTO :invalid_args

REM 
REM Determine compiler.
REM
:set_compiler
IF /I "%1" == "vc71" GOTO :do_vc71
IF /I "%1" == "vc80" GOTO :do_vc80
goto :invalid_args

:do_vc71
"%VS71COMNTOOLS%\vsvars32.bat"
goto :done
:do_vc80
"%VS80COMNTOOLS%\vsvars32.bat"
goto :done

:invalid_args
ECHO Usage: SetVars [vc71 or vc80]

:done
