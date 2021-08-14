@echo off

cmake -B build -S . -DBUILD_SKYRIMVR=OFF --check-stamp-file "build/CMakeFiles/generate.stamp"
if %ERRORLEVEL% NEQ 0 exit 1
cmake --build build --config Release
if %ERRORLEVEL% NEQ 0 exit 1

cmake -B buildVR -S . -DBUILD_SKYRIMVR=ON --check-stamp-file "buildVR/CMakeFiles/generate.stamp"
if %ERRORLEVEL% NEQ 0 exit 1
cmake --build buildVR --config Release
if %ERRORLEVEL% NEQ 0 exit 1

xcopy "build\Release\%1.dll" "dist\SkyrimSE\SKSE\Plugins\%1.dll*" /y
xcopy "buildVR\Release\%1.dll" "dist\SkyrimVR\SKSE\Plugins\%1.dll*" /y
