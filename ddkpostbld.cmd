@echo off
:: $Id: ddkpostbld.cmd 11 2009-02-03 01:50:55Z oliver $
setlocal
:: Perform post-build steps
if not EXIST ".\bin" md ".\bin"
xcopy /y ".\obj%BUILD_ALT_DIR%\i386\*.exe" ".\bin"
xcopy /y ".\obj%BUILD_ALT_DIR%\i386\*.pdb" ".\bin"
endlocal