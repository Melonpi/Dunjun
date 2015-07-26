@echo off

:::::::::::::::::::::::::::
:: Build File for Dunjun ::
:::::::::::::::::::::::::::

:: hh:mm:ss,cc
set startTime=%TIME%

::
:: Settings
::

:: Set Platform
:: TODO(bill): x86 does not work yet, only x64
set platform=x64

:: Include Directories
set includes= ^
	-I"W:\Dunjun\external\SDL2-2.0.3\include"  ^
	-I"W:\Dunjun\external\glew-1.12.0\include" ^
	-I"W:\Dunjun\external\stb-master\include"  ^
	-I"W:\Dunjun\include"

:: Compiler Settings
:: Debug   = -Od
:: Release = -O2
set compilerFlags= -Od -MTd -nologo -fp:fast -fp:except- -Gm- -GF- -GR- -Zi -Zo -Oi -WX -W4 -MP -TP -EHsc- -FC
set compilerDefines= -DGLEW_STATIC -D_CRT_SECURE_NO_DEPRECATE -D_UNICODE -DUNICODE
set compilerWarnings= -wd4100 -wd4127 -wd4189 -wd4201 -wd4244 -wd4245 -wd4351 -wd4505 -wd4702 -wd4996

:: Linker Settings
set outExe="Dunjun.exe"

set libPaths= ^
	-libpath:"W:\Dunjun\external\glew-1.12.0\lib\Release\%platform%" ^
	-libpath:"W:\Dunjun\external\SDL2-2.0.3\lib\%platform%"
set libs= ^
	glew32s.lib opengl32.lib ^
	SDL2.lib ^
	user32.lib gdi32.lib winmm.lib

set linkerFlags= ^
	-incremental:no -opt:ref -OUT:%outExe% ^
	-PDB:"Dunjun-%random%.pdb" ^
	-subsystem:CONSOLE -NODEFAULTLIB:LIBCMT

:: Build Directory
set buildDir= "W:\Dunjun\bin\"

::
::
::

:: Create Build Directory if does not exist yet
IF NOT EXIST %buildDir% mkdir %buildDir%
:: Set current directory to build directory
pushd %buildDir%

:: Error output will be redirected to nul
:: > NUL
:: Normal output will be redirected to nul
:: 2> NUL

:: Clean everything
del *.pdb > NUL 2> NUL
del *.dll > NUL 2> NUL
del *.exe > NUL 2> NUL

:: Ignore normal output
copy "W:\Dunjun\external\SDL2-2.0.3\lib\%platform%\SDL2.dll" "SDL2.dll" > NUL

::
:: Compile and Link
::
call cl "W:\Dunjun\src\UnityBuild.cpp" ^
	%includes% %compilerFlags% %compilerDefines% %compilerWarnings% ^
	/link %linkerFlags% %libPaths% %libs%

:: Reset current directory
popd


::
:: Calculate Duration of Build
::

set endTime=%TIME%

for /F "tokens=1-4 delims=:.," %%a in ("%startTime%") do (
	set /A "startT=(((%%a*60)+1%%b %% 100)*60+1%%c %% 100)*100+1%%d %% 100"
)

for /F "tokens=1-4 delims=:.," %%a in ("%endTime%") do (
	set /A "endT=(((%%a*60)+1%%b %% 100)*60+1%%c %% 100)*100+1%%d %% 100"
)

set /A elapsed=endT-startT

set /A hh=elapsed/(60*60*100), rest=elapsed%%(60*60*100), mm=rest/(60*100), rest%%=60*100, ss=rest/100, cc=rest%%100

:: Add leading zero
if %hh% lss 10 set hh=0%hh%
if %mm% lss 10 set mm=0%mm%
if %ss% lss 10 set ss=0%ss%
if %cc% lss 10 set cc=0%cc%

:: Default Formatting
set duration=%ss%.%cc% s

if %mm% NEQ 00 set duration=%mm%:%ss%.%cc%
if %hh% NEQ 00 set duration=%hh%:%mm%:%ss%.%cc%

echo.
echo Elapsed: %duration%
