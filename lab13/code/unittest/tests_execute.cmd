@echo off

echo TEMPORARY COPYING TARGET SOURCE FILES TO DESTINATION
echo ====================================================

if not exist ".\apptest" mkdir .\apptest>NUL
echo main.c > .\apptest\exclude.txt
xcopy /r /i /s /y /exclude:.\apptest\exclude.txt .\app\*.* .\apptest\ > NUL


echo COMPILING HOST UNITTESTS WITH TARGET CODE
echo ========================================= 

rem  -Xlinker --section-start -Xlinker .peripherals=0x428000^
rem  -Xlinker --print-map^

g++^
 -O0^
 -s^
 -x c++ -std=gnu++11 -Wno-write-strings -fno-exceptions^
 -I C:/MinGW/include/^
 -I C:/Keil_v5/ARM/PACK/InES/CTBoard14_DFP/4.0.0/HAL/Include/^
 -I ./unittest/include -L ./unittest/lib^
 -I ./app/^
 -I ./unittest/^
 -D CPPUTEST^
 ./apptest/*.c^
 ./RTE/HAL/CT_Board_HS14_M0/*.c^
 ./unittest/*.c^
 -lCppUTest^
 -lpthread^
 -o ./unittest/cunit_tests

rem  
IF %ERRORLEVEL% EQU 0 (

echo SUCCESSFUL COMPILATION! CHECK UNITTEST OUTPUT NOW:
echo ==================================================

.\unittest\cunit_tests.exe -v
echo.
echo ======================================================================

echo REMOVING TEMPORARY FILES / FOLDERS
echo ==================================

del .\unittest\cunit_tests.exe>NUL

) ELSE (

echo ======================================================================================
echo ERROR: COMPILATION FAILED! PLEASE CHECK ABOVE MESSAGES OUT OF GCC TO SEE WHAT'S WRONG!
echo ======================================================================================

)

rmdir .\apptest /S /Q>NUL