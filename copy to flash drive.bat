@echo off
echo This script is about to copy all data from the AppData folder to the flash drive.
echo It will kill the Minecraft-related contents of the flash drive before it does so.
echo If you wish to proceed, press any key. Otherwise press CTRL+C or click the X to abort.
pause > NUL
rmdir /S /Q D:\.minecraft
rmdir /S /Q D:\.tlauncher
robocopy %APPDATA%\.minecraft D:\.minecraft /E /COPYALL /MT:2 /DCOPY:DATE
robocopy %APPDATA%\.tlauncher D:\.tlauncher /E /COPYALL /MT:2 /DCOPY:DATE