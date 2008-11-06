@ECHO OFF
@ECHO Removing Java intermediate files...

del /s /f *.class
del /s /f *.jar
del /s /f *.cab

@ECHO Removing other Java temporary files...

del /s /f codebase.dat
del /s /f *.suo
