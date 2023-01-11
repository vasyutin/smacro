<!-- This is an auto generated file. Do not edit. -->

# SMACRO macroprocessor

## Purpose and functions

SMACRO is a simple macro processor that is designed to produce multiple versions of documentation based on the same source files. The scope of SMACRO is not limited to documentation development, the program works with any text files, including source files.

The main functions are:
* inclusion of blocks of source text files in the resulting text files depending on the values of the variables specified in the configuration files;
* substitution of the variables in the source text files with the values set in the configuration files;
* automatic numbering of sections, pictures, etc. in the text.

## Operating Principle

SMACRO copies files from the source files folder to the folder specified as the destination folder. When copying, SMACRO substitutes the variables in the source files with the values specified in the variable configuration file. In addition, depending on the logical value of the expressions composed of those variables, it may or may not copy text fragments from the source files to the resulting files. Also, when copying, SMACRO performs auto-numbering of text fragments marked with special tags. 

When you start SMACRO, you can specify templates of file names or file names that are not processed when copying (i.e. are copied without changes) and templates of file names or file names that are ignored by the program (i.e. are not copied to the results folder).

## Usage

```
smacro  -s <source folder>
        -d <destination folder> 
        -v <file with variables>
        [-e <templates or file names that are not processed> ...] 
        [-i <templates or file names patterns which are ignored> ...] 
        [-o <file which lists files in the order they are to be processed>] 
        [-@h] [--version] 
where:

  -s <source folder>,  --source <source folder>
     (required) The name of the folder containing the files to be processed.
	  
  -d <destination folder>,  --destination <destination folder>
     (required) The name of the folder where the result of the processing is written.

  -v <file with variables>,  --variables <file with variables>
     (required) The name of the file containing the values of the variables used 
     in processing.

  -e <exclude templates>,  --exclude <exclude templates>  (accepted multiple times)
     Templates or file names that are not processed when copied, i.e. they are copied 
     unchanged to the folder with the results of processing.

  -i <ignore templates>,  --ignore <ignore templates>  (accepted multiple times)
     Templates or file names that are ignored by the program, i.e. they are not 
     copied to the results folder.

  -o <order file>,  --order <order file>
     The name of file that lists the source files in the order in which they 
     will be processed. May be required for proper auto-numbering of text fragments.

   -@,  --at-prefixed
     Use the '@' symbol instead of '#' in control directives. Instead of using 
     #//, #if, #elif, #else, #endif and #include you'll be able to use 
     control directives more readable in files where the '#' symbol is used frequently 
     for example in Markdown files. Setting this switch affects all the source files.

   --,  --ignore_rest
     Ignores the rest of the labeled arguments following this flag.

   --version
     Displays version information and exits.

   -h,  --help
     Displays usage information and exits.
```
## File with variables

The values of the variables that are used in text processing are set in a file whose name is passed as a command-line parameter. Each variable value is placed on a separate line as follows
```
variable_name=value
```
where the variable name is a sequence of lowercase and uppercase Latin letters, numbers, and underscores. A variable name must begin with a letter or an underscore character. The value is a sequence of any characters up to the end of the string (spaces at the end of the string are ignored). For example
```
LANGUAGE=Italiano
ProgramDescription=SMACRO is a simple macro processor that is designed...
_div01=<br>
```
An empty value is specified by a string with the variable name, e.g.
```
VERSION_FOR_WEB
VERSION_FOR_PRINT
```
In the file with variables, blank lines and lines starting with # are ignored.

TODO
```
#if (defined(VAR1) && !defined(VAR2)) || Language == "Slovenščina"
	Besedilo na slovenskem
#elif Language == "Italiano"
   Un testo in italiano
#else
	Text in English
#endif
```

TODO

### Выражения в условных директивах

Выражения в директивах **#if** и **#elif** строятся по правилам, принятым для языков C и C++. Они могут включать в себя операторы и скобки, управляющие порядком вычисления выражений. Можно использовать следующие операторы:

* **defined(**имя_переменной**)** - оператор проверки того, определена ли переменная в файле переменных. Возвращает true, если переменная определена и false в противном случае. Пример использования:
```
#if defined(VERSION) && defined(FOR_PRINT)
	...
#endif
```

* операторы сравнения **==** (равно), **!=** (не равно), **>** (больше), **>=** (больше или равно), **<** (меньше) и **<=** (меньше или равно) выполняют лексикографическое сравнение значений переменных или строковых констант. Строковые константы заключаются в двойные кавычки. Операторы возвращают true при истинности условия, или false при его ложности. Примеры использования операторов сравнения:

```
#if (VERSION > "1" && Language == "English") || Language == "Italiano"
   #if TARGET == "HTML"
	   <hr>
	#else
	   ------------
	#endif
#endif
```
* оператор логического 'И' **&&** возвращает true при истинности обоих операндов. Пример использования:
```
#if (VERSION > "1" && Language == "English") && defined(PRINT_VERSION)
   ...
#endif
```

* оператор логического 'ИЛИ' **||** возвращает true при истинности хотя бы одного из операндов. Пример использования:
```
#if VERSION > "1" || Language == "English" || defined(PRINT_VERSION)
   ...
#endif
```
* оператор отрицания **!** инвертирует значение находящегося за ним выражения. Пример использования:
```
#if !(VERSION > "1" || Language == "English") || !defined(PRINT_VERSION)
   ...
#endif
```
Выражение, которое не умещается на одной строке, может быть перенесено на следующую строку если указать символ '\' последним в строке, например
```
#if !(VERSION > "1" || Language == "English") || \
   !defined(PRINT_VERSION)
   ...
#endif
```












## Build and installation

### Linux

To install SMACRO on Linux you have to build program's binary from sources. Just clone the repo or download the sources from the project's releases page (https://github.com/vasyutin/smacro/releases) and unpack them. Then chdir to */projects/gcc* and run

```sh
make
sudo make install
```


### Windows

The executable file for Windows can be downloaded at https://github.com/vasyutin/smacro/releases. The file is distributed as a ZIP archive. Extract the executable from the archive and put it in the directory that is listed in your PATH environment variable, to ensure that it can be called from a command prompt.

#### Build with MinGW

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



## License

GNU GPL Version 3

## Author

Sergey Vasyutin (sergey [at] vasyut.in)
