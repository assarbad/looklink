@echo off
call ddkbuild.cmd -WNET2K fre . -cZ
set NO_INCREASE_BUILDNO=1
call ddkbuild.cmd -WNETAMD64 fre . -cZ
call ollisign.cmd "%~dp0\bin\*.exe" "http://assarbad.net" "Looklink, application to inspect reparse points of various kinds ..."
