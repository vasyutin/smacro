${DO_NOT_EDIT_COMMENT}

@//----------------------------------------------------------------------------
@if LANGUAGE=="Ru"
# Макропроцессор SMACRO
@else
# SMACRO macroprocessor
@endif

@//----------------------------------------------------------------------------
@if LANGUAGE=="Ru"
## Назначение и функции

SMACRO - простой макропроцессор, который создан для подготовки нескольких вариантов документации на основе одних исходных файлов. Тем не менее область применения SMACRO не ограничивается разработкой документации, программа работает с любыми текстовыми файлами, в том числе с текстами программ.

Основные функции:
* включение в итоговые текстовые файлы блоков исходных текстовых файлов в зависимости от значений переменных, заданных в конфигурационных файлах;
* подстановка вместо переменных в исходных текстовых файлах их значений, заданных в конфигурационных файлах;
* автоматическая нумерация разделов, рисунков и пр. в тексте.
@else
## Purpose and functions

SMACRO is a simple macro processor that is designed to produce multiple versions of documentation based on the same source files. Nevertheless, the scope of SMACRO is not limited to documentation development, the program works with any text files, including source files.

The main functions are:
* inclusion of blocks of source text files in the resulting text files depending on the values of the variables specified in the configuration files;
* substitution of the variables in the source text files with the values set in the configuration files;
* automatic numbering of sections, pictures, etc. in the text.
@endif

@//----------------------------------------------------------------------------
@if LANGUAGE=="Ru"
## Принцип работы

SMACRO копирует файлы из папки исходных файлов в папку, указанную как папка назначения. При этом при копировании SMACRO подставляет вместо переменных в исходных файлах их значения, указанные в конфигурационном файле с переменными. Кроме того в зависимости от истинности выражений, составленных из этих переменных, может копировать или не копировать фрагменты текста из исходных файлов в результирующие. Также при копировании SMACRO выполняет автонумерацию фрагментов текста, помеченных специальными тегами. 

При запуске SMACRO можно указать шаблоны или имена файлов, которые не обрабатываются при копировании (то есть копируются без изменений), и шаблоны или имена файлов, которые игнорируются программой (то есть не копируются в папку с результатами).
@else
## Operating Principle

SMACRO copies files from the source files folder to the folder specified as the destination folder. When copying, SMACRO substitutes the variables in the source files with the values specified in the variable configuration file. In addition, depending on the logical value of the expressions composed of those variables, it may or may not copy text fragments from the source files to the resulting files. Also, when copying, SMACRO performs auto-numbering of text fragments marked with special tags. 

When you start SMACRO, you can specify templates of file names or file names that are not processed when copying (i.e. are copied without changes) and templates of file names or file names that are ignored by the program (i.e. are not copied to the results folder).
@endif

@//----------------------------------------------------------------------------
@if LANGUAGE=="Ru"
## Использование
@else
## Usage
@endif

```
@if LANGUAGE=="Ru"
smacro  -s <папка исходных файлов>
        -d <папка в которую записывается результат обработки> 
        -v <файл с переменными>
        [-e <шаблоны файлов, которые не обрабатываются> ...] 
        [-i <шаблоны файлов, которые игнорируются> ...] 
        [-o <файл, в котором перечислены файлы в том порядке, в котором они обрабатываются>] 
        [-@h] [--version] 
где:
@else
smacro  -s <source folder>
        -d <destination folder> 
        -v <file with variables>
        [-e <templates or file names that are not processed> ...] 
        [-i <templates or file names patterns which are ignored> ...] 
        [-o <file which lists files in the order they are to be processed>] 
        [-@h] [--version] 
where:
@endif

@if LANGUAGE=="Ru"
  -s <папка исходных файлов>,  --source <папка исходных файлов>
     (обязательный параметр) Имя папки, содержащей файлы для обработки.
@else
  -s <source folder>,  --source <source folder>
     (required) The name of the folder containing the files to be processed.
@endif

@if LANGUAGE=="Ru"
  -d <папка с результатом обработки>,  --destination <папка с результатом обработки>
     (обязательный параметр) Имя папки, в которую записывается результат обработки.
@else
  -d <destination folder>,  --destination <destination folder>
     (required) The name of the folder where the result of the processing is written.
@endif

@if LANGUAGE=="Ru"
  -v <файл с переменными>,  --variables <файл с переменными>
     (обязательный параметр) Имя файла, содержащего значения переменных, которые 
     используются при обработке.
@else
  -v <file with variables>,  --variables <file with variables>
     (required) The name of the file containing the values of the variables used 
     in processing.
@endif

@if LANGUAGE=="Ru"
  -e <шаблоны файлов, которые не обрабатываются>,  
  --exclude <шаблоны файлов, которые не обрабатываются>
     (можно использовать несколько раз) Шаблоны или имена файлов, которые не обрабатываются 
     при копировании, то есть копируются без изменений в папку с результатами обработки.
@else
  -e <exclude templates>,  --exclude <exclude templates>  (accepted multiple times)
     Templates or file names that are not processed when copied, i.e. they are copied 
     unchanged to the folder with the results of processing.
@endif

@if LANGUAGE=="Ru"
  -i <шаблоны файлов, которые игнорируются>,  
  --ignore <шаблоны файлов, которые игнорируются>
     (можно использовать несколько раз) Шаблоны или имена файлов, которые которые 
     игнорируются программой, то есть не копируются в папку с результатами.
@else
  -e <ignore templates>,  --exclude <ignore templates>  (accepted multiple times)
     Templates or file names that are ignored by the program, i.e. they are not 
     copied to the results folder.
@endif

@if LANGUAGE=="Ru"
  -o <имя файла со списком файлов в порядке обработки>,
  --order <имя файла со списком файлов в порядке обработки>
     Имя файла, в котором перечислены исходные файлы в том порядке, в котором они 
     будут обрабатываться. Может потребоваться для правильной автонумерации 
     фрагментов текста.
@else
  -o <order file>,  --order <order file>
     The name of file that lists the source files in the order in which they 
     will be processed. May be required for proper auto-numbering of text fragments.
@endif

@if LANGUAGE=="Ru"
   -@,  --at-prefixed
     Использовать символ '@' вместо '#' в директивах. То есть вместо директив 
     #//, #if, #elif, #else, #endif and #include использовать директивы 
     @//, @if, @elif, @else, @endif and @include. Позволяет сделать более 
     читаемыми управляющие конструкции языка SMACRO в файлах, в которых часто
     используется символ '#', например, в файлах на языке Markdown. Задание
     этого ключа влияет на все исходные файлы.
@else
   -@,  --at-prefixed
     Use the '@' symbol instead of '#' in control directives. Instead of using 
     #//, #if, #elif, #else, #endif and #include you'll be able to use 
     @//, @if, @elif, @else, @endif and @include directives. Allows you to make 
     control directives more readable in files where the '#' symbol is used frequently 
     for example in Markdown files. Setting this switch affects all the source files.
@endif

@if LANGUAGE=="Ru"
   --,  --ignore_rest
     Игнорировать аргументы после этого флага.

   --version
     Показывает версию программы и завершает её работу.

   -h,  --help
     Показывает информацию об использовании программы и завершает её работу.
@else
   --,  --ignore_rest
     Ignores the rest of the labeled arguments following this flag.

   --version
     Displays version information and exits.

   -h,  --help
     Displays usage information and exits.
@endif
```
@//----------------------------------------------------------------------------
@if LANGUAGE=="Ru"
## Описание языка

### Управляющие директивы

Язык SMACRO поддерживает несколько директив, управляющих тем, какие фрагменты файла включаются в его результирующую версию. Директивы должны находиться в начале строки, перед ними могут быть пробелы и знаки табуляции, но не должно быть других символов. Строки с директивами не копируются в результирующую версию файла.

**#//** - директива начала комментария. Строка, начинающаяся с этого оператора, игнорируется.

**#if**, **#elif**, **#else**, **#endif** - директивы условного оператора. Выполняют ту же роль, что и эти же директивы препроцессора в языке C/C++. В условиях могут использоваться выражения, которые проверяют, определены ли переменные, а также проверяют их значения. Блоки текста, для которых условия выполняются, переписываются в итоговые файлы. Пример задания условного оператора:

```
#if (defined(VAR1) && !defined(VAR2)) || Language == "English"
   Text in English
#elif Language == "Italiano"
   Un testo in italiano
#else
   Besedilo na slovenskem
#endif
```

**#include** - директива включения файла. Вместо строки с этой директивой подставляется содержимое указанного в ней файла. Имя включаемого файла задается в квадратных скобках также, как в макропроцессоре C/C++. Например,

```
#include<./include/some-file.txt>
```
Для удобства при указании имени файла можно использовать встроенную переменную ${SMACRO_ROOT}, которая содержит путь к папке с исходными файлами.

```
#include<${SMACRO_ROOT}/include/file.inc>
```

Если при запуске указать ключ **-@** или **--at-prefixed**, то вместо директив #//, #if, #elif, #else, #endif и #include надо будет использовать директивы @//, @if, @elif, @else, @endif и @include. Это позволяет сделать более читаемыми управляющие конструкции языка SMACRO в файлах, в которых часто используется символ '#', например, в файлах на языке Markdown. Задание этого ключа влияет на все исходные файлы при обработке.

### Файл с переменными

## Пример запуска программы



@else
## Language
@endif



### How to build/install on Linux

To install SMACRO on Linux you have to build program's binary from sources. Just clone the repo or download the sources from the project's releases page (https://github.com/vasyutin/smacro/releases) and unzip them. Then chdir to *\<ProjectRoot\>/projects/linux* and run

```sh
make
sudo make install
```

### How to install on Windows

The executable file for Windows can be downloaded at https://github.com/vasyutin/smacro/releases. The file is distributed as a ZIP archive. Extract the executable from the archive and put it in the directory that is listed in your PATH environment variable, to ensure that it can be called from a command prompt.

### How to build on Windows

To build SMACRO from sources you have to clone the repo or or download the sources from the project's releases pages (https://github.com/vasyutin/smacro/releases) and unzip them. 

### Build with MinGW 
 
Chdir to *\<ProjectRoot\>\\projects\\mingw* and run

```sh
mingw32-make
``` 

The compiled binary is placed in the folder *\<ProjectRoot\>\\build\\release* and named *smacro.exe*. To use it put it in the directory is listed in your *PATH* environment variable.

### Build with Visual Studio

By now I supply solution (.sln) with the project files (.vcproj) files for Visual Studio 2015 Community Edition. To build the software open the solution file *\<ProjectRoot\>\\projects\\VS2015\\smacro.sln* and start the build process. The compiled binaries will be placed in the folder *\<ProjectRoot\>\build\Platform\Configuration*, where the *Platform* is 'Win32' or 'x64' and the *Configuration* is 'Debug' or 'Release'. To run the program outside Visual Studio, you have to copy the result binary file *smacro.exe* to the folder, which is added to your *PATH* environment variable.

The sources are nothing than pure C++ thus the project can be opened in compiled in the newer version of Visual Studio.

@//----------------------------------------------------------------------------
@if LANGUAGE=="Ru"
## Лицензия
@else
## License
@endif

GPL

@//----------------------------------------------------------------------------
@if LANGUAGE=="Ru"
## Автор
@else
## Author
@endif

Sergey Vasyutin (sergey [at] vasyut.in)
