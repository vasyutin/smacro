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
```
smacro  -s <папка исходных файлов>
        -d <папка в которую записывается результат обработки> 
		  -v <файл с переменными>
		  [-e <шаблоны файлов, которые не обрабатываются> ...] 
		  [-i <шаблоны файлов, которые игнорируются> ...] 
		  [-o <файл, в котором перечислены файлы в том порядке, в котором они должны обрабатываться>] 
		  [-@h] [--version] 
```
где:
@else
## Usage
```
smacro  [-@h] [--version] [-e <exclude masks> ...] [-i <ignore masks>
        ...] [-o <order file>] -d <destination folder> -s <source
        folder> -v <variables file>

@endif



@if LANGUAGE=="Ru"
где:
@else
where:
@endif


Where: 

   -s <source folder>,  --source <source folder>
     (required) The folder, containing documentation files to process

   -d <destination folder>,  --destination <destination folder>
     (required) The destination folder for the processed files

   -v <variables file>,  --variables <variables file>
     (required) The file, containing values of the variables for the
     current run (the text in the file is assumed to be in UTF-8).

   -e <exclude masks>,  --exclude <exclude masks>  (accepted multiple
      times)
     The mask of filename to exclude from processing. This files are only
     copied to the output folder.

   -i <ignore masks>,  --ignore <ignore masks>  (accepted multiple times)
     The mask of filename to ignore. This files are not copied to the
     output folder.

   -o <order file>,  --order <order file>
     The file containing the list of the files to parse to look to the
     $number directive in required order

   -@,  --at-prefixed
     Use symbol '@' instead of '#' as the prefix for the control operators
     (@//, @if, @elif, @else, @endif and @include instead of #//, #if,
     #elif, #else, #endif and #include) in text with plenty of '#' (ex. in
     Markdown)

   --,  --ignore_rest
     Ignores the rest of the labeled arguments following this flag.

   --version
     Displays version information and exits.

   -h,  --help
     Displays usage information and exits.

   All the files being processed (except the files excluded from the
   processing with the -e switch) are assumed to be in UTF-8 encoding.
   Example:
   	smacro -s ..\..\example\source -d ..\..\build\doc_res -v
   ..\..\example\config -e *.txt,*.png -e *.jpg
   
   SMACRO (Simple MACRO processor) is written by Sergey Vasyutin (see
   https://github.com/vasyutin/smacro).
```


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
