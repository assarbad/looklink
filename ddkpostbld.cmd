@echo off
:: setlocal
:: Perform post-build steps
if not EXIST ".\bin" md ".\bin"
xcopy /y ".\obj%BUILD_ALT_DIR%\i386\*.exe" ".\bin"
xcopy /y ".\obj%BUILD_ALT_DIR%\i386\*.pdb" ".\bin"
endlocal