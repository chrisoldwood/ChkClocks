@ECHO OFF
REM ************************************************************
REM
REM Script to execute all Unit Tests.
REM
REM ************************************************************

:check_args
IF /I "%1" == "" GOTO :invalid_args

:set_debug_build
IF /I NOT "%1" == "debug" GOTO :set_release_build
SET BUILD=Debug
GOTO :run_tests

:set_release_build
IF /I NOT "%1" == "release" GOTO :invalid_args
SET BUILD=Release
GOTO :run_tests

:run_tests

ECHO ========================================
ECHO Running 'Core' library tests...
ECHO ========================================
Lib\Core\Test\%BUILD%\Test.exe
IF errorlevel 1 goto :failed

ECHO.

ECHO ========================================
ECHO Running 'WCL' library tests...
ECHO ========================================
Lib\WCL\Test\%BUILD%\Test.exe
IF errorlevel 1 goto :failed

ECHO.

ECHO ========================================
ECHO Running 'NCL' library tests...
ECHO ========================================
Lib\NCL\Test\%BUILD%\Test.exe
IF errorlevel 1 goto :failed

ECHO.

ECHO ========================================
ECHO Running 'MDBL' library tests...
ECHO ========================================
Lib\MDBL\Test\%BUILD%\Test.exe
IF errorlevel 1 goto :failed

ECHO.

ECHO ========================================
ECHO Running 'COM' library tests...
ECHO ========================================
Lib\COM\Test\%BUILD%\Test.exe
IF errorlevel 1 goto :failed

ECHO.

ECHO ========================================
ECHO Running 'XML' library tests...
ECHO ========================================
Lib\XML\Test\%BUILD%\Test.exe
IF errorlevel 1 goto :failed

ECHO.

ECHO ========================================
ECHO Running 'COMTrace' library tests...
ECHO ========================================
COMTrace\Test\%BUILD%\Test.exe
IF errorlevel 1 goto :failed

ECHO.

ECHO ========================================
ECHO Running 'DDECOMClient' library tests...
ECHO ========================================
DDECOMClient\Test\%BUILD%\Test.exe
IF errorlevel 1 goto :failed

ECHO.
ECHO The suite of tests PASSED

GOTO :done

:failed
ECHO.
ECHO ERROR: The suite of tests FAILED.
GOTO :done

:invalid_args
ECHO Usage: RunTests [debug or release]
GOTO :done

:done
