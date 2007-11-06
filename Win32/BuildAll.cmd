@ECHO OFF
REM ************************************************************
REM
REM Script to build all solutions for both Debug and Release
REM targets.
REM
REM NB: You must run %VS??COMNTOOLS%\vsvars32.bat first to setup
REM the environment variables first.
REM
REM ************************************************************

FOR /R %%I IN (*.sln) DO devenv %%I /build Debug
FOR /R %%I IN (*.sln) DO devenv %%I /build Release
