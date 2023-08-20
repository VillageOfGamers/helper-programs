@echo off
D:
cd \Scripts\
for /F %%I in ( services.txt ) do (
	sc stop %%~I
	sc config %%~I start=disabled
)
sc stop "Apple Mobile Device Service"
sc config "Apple Mobile Device Service" start=disabled
sc stop "Bonjour Service"
sc config "Bonjour Service" start=disabled
echo Press any key to re-enable and restart Audiosrv...
pause > NUL
sc config Audiosrv start=auto
sc start Audiosrv