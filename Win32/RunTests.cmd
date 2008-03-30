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

ECHO ========================================
ECHO Running 'NCL' library tests...
ECHO ========================================
Lib\NCL\Test\Debug\Test.exe
IF errorlevel 1 goto :failed

ECHO.

ECHO ========================================
ECHO Running 'MDBL' library tests...
ECHO ========================================
Lib\MDBL\Test\Debug\Test.exe
IF errorlevel 1 goto :failed

ECHO.

ECHO ========================================
ECHO Running 'COM' library tests...
ECHO ========================================
Lib\COM\Test\Debug\Test.exe
IF errorlevel 1 goto :failed

ECHO.

ECHO ========================================
ECHO Running 'XML' library tests...
ECHO ========================================
Lib\XML\Test\Debug\Test.exe
IF errorlevel 1 goto :failed

ECHO.
ECHO The suite of tests PASSED

goto :done

:failed
ECHO.
ECHO ERROR: The suite of tests FAILED.
goto :done

:done
