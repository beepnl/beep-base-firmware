echo off
cls
echo Date format = %date%
echo dd = %date:~3,2%
echo mm = %date:~6,2%
echo yyyy = %date:~9,4%
echo.
echo Time format = %time%
SET hh=%time:~0,2%
IF "%hh:~0,1%" == " " SET hh=0%hh:~1,1%
echo mm = %time:~3,2%
echo ss = %time:~6,2%
echo.

REM Create a build folder with the following name version_major.version_minor.sub_version_YYYY_MM_DD_HHMM
SET build_folder=BEEP_v1.5.10_%date:~9,4%_%date:~6,2%_%date:~3,2%_%hh%%time:~3,2%
ECHO build folder %build_folder%

if exist %build_folder%				RMDIR /S /Q %build_folder%
MD %build_folder%

start /B /wait Compile_Beep_build.bat Release %build_folder%
if %ERRORLEVEL% NEQ 0 (
	echo Build compilation failed with error: %ERRORLEVEL%
	pause
	exit
)


echo Completed all builds
pause
exit