@echo off
if "%~1" == "noinc" set NO_INCREASE_BUILDNO=1
call ddkbuild.cmd -WNET2K fre . -cZ
set NO_INCREASE_BUILDNO=1
call ddkbuild.cmd -WNETAMD64 fre . -cZ
if exist bin\*.idb del /f bin\*.idb
call ollisign.cmd "%~dp0\bin\*.exe" "http://assarbad.net" "Looklink, application to inspect reparse points of various kinds ..."
:: Find 7-Zip
set SEVENZIP=%ProgramFiles%\7-Zip\7z.exe
if not exist "%SEVENZIP%" set SEVENZIP=%ProgramFiles(x86)%\7-Zip\7z.exe
if not exist "%SEVENZIP%" ( echo ERROR: Could not find 7z.exe & goto :EOF )
set ARCHIVE=looklink.zip
if exist %ARCHIVE% del /f %ARCHIVE%
if exist %ARCHIVE%.asc del /f %ARCHIVE%.asc
"%SEVENZIP%" a -tzip %ARCHIVE% bin BUILD makefile sources *.cmd *.cpp *.h *.rc *.rst *.sln *.vcproj *.vcxproj *.vsprops
sha1sum %ARCHIVE%
md5sum %ARCHIVE%
gpg -a --detach-sign %ARCHIVE%
