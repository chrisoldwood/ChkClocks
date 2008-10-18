@ECHO OFF
REM ************************************************************
REM
REM Script to build a single solution for both Debug and/or
REM Release targets.
REM
REM NB: You must set the environment variables first by running
REM the SetVars script. If building with STLport you need to use
REM the STLport\SetVars script instead.
REM
REM ************************************************************

REM 
REM Check command line..
REM

:check_args
IF /I "%1" == "" GOTO :invalid_args
IF /I "%2" == "" GOTO :invalid_args
IF /I "%3" == "" GOTO :invalid_args

REM 
REM Check environment variables are set.
REM

:check_vars
IF /I "%VCVERSION%" == "" GOTO :invalid_vars

REM 
REM Determine compiler.
REM

:set_compiler
IF /I "%1" == "vc71" GOTO :do_vc71
IF /I "%1" == "vc80" GOTO :do_vc80
IF /I "%1" == "vc90" GOTO :do_vc90
GOTO :invalid_args

:do_vc71
SET VSBUILD=VC71
GOTO :do_debug

:do_vc80
SET VSBUILD=VC80
GOTO :do_debug

:do_vc90
SET VSBUILD=VC80
GOTO :do_debug

REM
REM Do a Debug build?
REM

:do_debug
IF /I "%2" == ""      GOTO :invalid_args
IF /I "%2" == "both"  GOTO :debug_build
IF /I "%2" == "debug" GOTO :debug_build
GOTO :do_release

:debug_build
IF "%VSBUILD%" == "VC80" GOTO :vc80_debug_build
IF "%VSBUILD%" == "VC90" GOTO :vc90_debug_build
devenv /nologo /useenv %3 /build Debug
GOTO :do_release

:vc80_debug_build
:vc90_debug_build
vcbuild /nologo /useenv %3 "Debug|Win32"
GOTO :do_release

REM
REM Do a Release build?
REM

:do_release
IF /I "%2" == ""        GOTO :invalid_args
IF /I "%2" == "both"    GOTO :release_build
IF /I "%2" == "release" GOTO :release_build
GOTO :done

:release_build
IF "%VSBUILD%" == "VC80" GOTO :vc80_release_build
IF "%VSBUILD%" == "VC90" GOTO :vc90_release_build
devenv /nologo /useenv %3 /build Release
GOTO :done

:vc80_release_build
:vc90_release_build
vcbuild /nologo /useenv %3 "Release|Win32"
GOTO :done

REM 
REM Report errors.
REM

:invalid_args
ECHO Usage: Build [vc71 or vc80 or vc90] [debug or release or both] [solution]
GOTO :done

:invalid_vars
ECHO ERROR: Compiler environment variables not set. Run 'SetVars' first.
GOTO :done

:done
