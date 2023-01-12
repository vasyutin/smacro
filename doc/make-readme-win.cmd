@echo off
set DEST=..\build\smacro-readme-ru
smacro -s .\src -d %DEST% -v .\config-ru -@
powershell -Command "(gc -encoding utf8 %DEST%\README.md) -replace 'DOLLAR_SIGN', '$' | Out-File -encoding utf8 %DEST%\README.md"
del ..\README_ru.md 2> nul > nul
move %DEST%\README.md ..\README_ru.md 2> nul > nul

set DEST=..\build\smacro-readme-en
smacro -s .\src -d %DEST% -v .\config-en -@
powershell -Command "(gc -encoding utf8 %DEST%\README.md) -replace 'DOLLAR_SIGN', '$' | Out-File -encoding utf8 %DEST%\README.md"
del ..\README.md 2> nul > nul
move %DEST%\README.md ..\README.md 2> nul > nul