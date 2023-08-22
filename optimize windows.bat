@echo off
D:
cd \Scripts\
for /F "delims=" %%I in ( services.txt ) do (
	sc stop "%%~I"
	sc config "%%~I" start=disabled
)