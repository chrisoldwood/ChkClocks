@ECHO OFF
REM ************************************************************
REM
REM Script to execute all Unit Tests.
REM
REM ************************************************************

ECHO ========================================
ECHO Running 'Core' library tests...
ECHO ========================================
Lib\Core\Test\Debug\Test.exe
IF errorlevel 1 goto :failed

ECHO.

ECHO ========================================
ECHO Running 'WCL' library tests...
ECHO ========================================
Lib\WCL\Test\Debug\Test.exe
IF errorlevel 1 goto :failed

ECHO.
ECHO The suite of tests PASSED

goto :done

:failed
ECHO.
ECHO ERROR: The suite of tests FAILED.
goto :done

:done
