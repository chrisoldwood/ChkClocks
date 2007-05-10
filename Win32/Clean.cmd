@ECHO OFF
@ECHO Removing VC++ intermediate files...

del /s /f *.pch
del /s /f *.sbr
del /s /f *.obj
del /s /f *.res
del /s /f *.tlh
del /s /f *.tli
del /s /f *.tlb
del /s /f BuildLog.htm

@ECHO Removing VC++ target files...

del /s /f *.pdb
del /s /f *.idb
del /s /f *.ilk
del /s /f *.exe
del /s /f *.dll
del /s /f *.lib
del /s /f *.plg
del /s /f *.map
del /s /f *.exp

@ECHO Removing VC++ workspace files...

del /s /f *.ncb
del /s /f *.opt
del /s /f *.aps
del /s /f *.bsc
del /s /f *.scc

@ECHO Removing VC++ profiling/instrumentation files...

del /s /f *.pbi
del /s /f *.pbo
del /s /f *.pbt
del /s /f *._xe
del /s /f *.pcc
del /s /f *.stt

@ECHO Removing log files...

del /s /f *.log
del /s /f TraceLog.txt
del /s /f *.sup

@ECHO Removing Doxygen documentation
del /s Docs\*.html
del /s Docs\*.png
del /s Docs\*.gif
del /s Docs\*.css
