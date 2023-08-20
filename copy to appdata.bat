@echo off
echo This script is about to copy all data from the flash drive to the AppData folder.
echo It will kill the Minecraft-related contents of the AppData folder before it does so.
echo If you wish to proceed, press any key. Otherwise press CTRL+C or click the X to abort.
pause > NUL
rmdir /S /Q %APPDATA%\.minecraft
rmdir /S /Q %APPDATA%\.tlauncher
robocopy D:\.minecraft %APPDATA%\.minecraft /E /COPYALL /MT:2 /DCOPY:DATE
robocopy D:\.tlauncher %APPDATA%\.tlauncher /E /COPYALL /MT:2 /DCOPY:DATE