call ./WinStop.bat
timeout 2

start ServerData.exe
timeout 1
start ServerScene.exe
timeout 1
start ServerWorld.exe
timeout 1
start ServerLogin.exe
timeout 1
start ServerGate.exe

