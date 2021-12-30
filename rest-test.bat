@echo off

curl http://192.168.2.20/mode/4/o
curl http://192.168.2.20/mode/6/o

timeout /t 1

:top

curl http://192.168.2.20/on?param=6
rem timeout /t 1

curl http://192.168.2.20/off?param=6
rem timeout /t 1

goto top

curl http://192.168.2.20/digital/4/1
timeout /t 5

rem curl http://192.168.2.20/digital/5/1
rem timeout /t 1

curl http://192.168.2.20/digital/4/0
timeout /t 5

rem curl http://192.168.2.20/digital/5/0
rem timeout /t 1

goto top