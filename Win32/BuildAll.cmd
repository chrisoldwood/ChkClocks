@ECHO OFF
REM ************************************************************
REM
REM Script to build all solutions for both Debug and Release
REM targets.
REM
REM NB: You must set the environment variables first by running
REM the appropriate script:-
REM
REM VC++ 7.1: "%VS71COMNTOOLS%\vsvars32.bat"
REM VC++ 8.0: "%VS80COMNTOOLS%\vsvars32.bat"
REM VC++ 9.0: "%VS90COMNTOOLS%\vsvars32.bat"
REM
REM If building with STLport you also need to set the STLport
REM paths as well by running:-
REM
REM STLport\SetVars.cmd
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
IF /I "%1" == "vc90" GOTO :do_vc90
goto :invalid_args

:do_vc71
SET VSBUILD=VC71
goto :do_debug
:do_vc80
SET VSBUILD=VC80
goto :do_debug
:do_vc90
SET VSBUILD=VC80
goto :do_debug

REM
REM Do a Debug build?
REM
:do_debug
IF /I "%2" == ""      GOTO :invalid_args
IF /I "%2" == "both"  GOTO :debug_build
IF /I "%2" == "debug" GOTO :debug_build
goto :do_release

:debug_build
IF "%VSBUILD%" == "VC80" goto :vc80_debug_build
IF "%VSBUILD%" == "VC90" goto :vc90_debug_build
FOR /R %%I IN (*.sln) DO devenv /nologo /useenv %%I /build Debug
goto :do_release

:vc80_debug_build
:vc90_debug_build
FOR /R %%I IN (*.sln) DO vcbuild /nologo /useenv %%I "Debug|Win32"
goto :do_release

REM
REM Do a Release build?
REM
:do_release
IF /I "%2" == ""        GOTO :invalid_args
IF /I "%2" == "both"    GOTO :release_build
IF /I "%2" == "release" GOTO :release_build
goto :done

:release_build
IF "%VSBUILD%" == "VC80" goto :vc80_release_build
IF "%VSBUILD%" == "VC90" goto :vc90_release_build
FOR /R %%I IN (*.sln) DO devenv /nologo /useenv %%I /build Release
goto :done

:vc80_release_build
:vc90_release_build
FOR /R %%I IN (*.sln) DO vcbuild /nologo /useenv %%I "Release|Win32"
goto :done

:invalid_args
ECHO Usage: BuildAll [vc71 or vc80 or vc90] [debug or release or both]

:done
