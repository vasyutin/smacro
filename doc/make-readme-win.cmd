set DEST=..\build\smacro-readme-ru
smacro -s .\src -d %DEST% -v .\config-ru -@
del ..\README_ru.md
move %DEST%\README.md ..\README_ru.md

set DEST=..\build\smacro-readme-en
smacro -s .\src -d %DEST% -v .\config-en -@
del ..\README.md
move %DEST%\README.md ..\README.md