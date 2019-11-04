call ./WinStop.bat
timeout 1

start ServerGame.exe 10001
timeout 1
::start ServerGame.exe 10002
::timeout 1
start ServerMaster.exe


