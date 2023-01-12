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

## Processing directives

### Directive for variable value substitution

A construct of the form
```
${variable_name}
```
is replaced in the text by the value of the specified variable. If such a variable is not defined in the variable file, the program will terminate with an error message.

### Directives for controlling the processing

The SMACRO language supports several directives that control which fragments of a file are included in its resulting version. Directives must be at the beginning of the line possibly preceded by spaces and tabs. Lines with directives are not copied into the resulting version of the file.

**#//** - comment directive. A line starting with this operator is ignored.

**#if**, **#elif**, **#else**, **#endif** - conditional operator directives. They play the same role as the preprocessor directives in C/C++. Conditions may use expressions that check if variables are defined and also check their values. Blocks of text for which the conditions are met are overwritten in summary files. An example of setting a conditional statement:
```
#if (defined(VAR1) && !defined(VAR2)) || Language == "Slovenščina"
	Besedilo na slovenskem
#elif Language == "Italiano"
   Un testo in italiano
#else
	Text in English
#endif
```

**#include** - file inclusion directive. Instead of the string with this directive the contents of the file specified in it are substituted. The name of the included file is specified in square brackets, just like in the C/C++ macro processor. For example,
```
#include<./include/some-file.txt>
```
For convenience, you can use the built-in variable ${SMACRO_ROOT}, which contains the path to the folder with the source files, when specifying the file name.
```
#include<${SMACRO_ROOT}/include/file.inc>
```
If you specify the `-@` or `--at-prefixed` key when starting a program, you will have to use the directives @//, @if, @elif, @else, @endif and @include instead of the directives #//, #if, #elif, #else, #endif and #include. This allows you to make SMACRO language control constructions in files that often use the `#` symbol, such as Markdown files, more readable. Setting this key affects all source files during processing.

### Expressions in conditional directives

Expressions in **#if** and **#elif** directives are built according to the rules accepted for C and C++ languages. They may include operators and brackets controlling the order of expressions evaluation. The following operators can be used:

* **defined(variable_name)** is an operator to check if a variable is defined in a variable file. Returns true if the variable is defined and false otherwise. Example usage:
```
#if defined(VERSION) && defined(FOR_PRINT)
	...
#endif
```
* **==** (equal), **!=** (not equal), **>** (more), **>=** (more or equal), **<** (less) and **<=** (less or equal) comparison operators perform lexicographical comparison of variable or string constant values. String constants are enclosed in double quotes. Examples of using comparison operators:
```
#if (VERSION > "1" && Language == "English") || Language == "Italiano"
   #if TARGET == "HTML"
	   <hr>
	#else
	   ------------
	#endif
#endif
```
* the boolean AND operator **&&** returns true if both operands are true. Example usage:
```
#if (VERSION > "1" && Language == "English") && defined(PRINT_VERSION)
   ...
#endif
```
* the logical OR operator **||** returns true if at least one of the operands is true. Example usage:
```
#if VERSION > "1" || Language == "English" || defined(PRINT_VERSION)
   ...
#endif
```
* the negation operator **!** inverts the value of the expression behind it. Example usage:
```
#if !(VERSION > "1" || Language == "English") || !defined(PRINT_VERSION)
   ...
#endif
```
An expression that does not fit to one line can be carried to the next line by specifying the `\` character last in the line, for example
```
#if !(VERSION > "1" || Language == "English") || \
   !defined(PRINT_VERSION)
   ...
#endif
```

### Directives for automatic numbering

In texts where you want parts, sections, pictures, etc. to be numbered automatically, the **$number** and **$ref** directives are used. The **$number** directive is replaced by a count incremented by one. It is written in the text as follows
```
$number{ имя_номера | имя_счетчика }
```
where `number_name` is a string that allows you to refer to the counter value obtained in this place of text in the directive **$ref** in other parts of the text; `number_name` is a counter, the value of which increases when inserting the number instead of $number directive. Number and counter names are text strings without spaces and punctuation marks. For example, the following directives
```
$number{number_intro | chapters}
$number{number_mainpart | chapters}
$number{number_figure_structure | figures}
$number{number_figure_streams | figures}
$number{number_conclusion | chapters}
```
will be transformed into this text
```
1
2
1
2
3
```
The **$number** directive is used to refer in the text to the number inserted by the **$number** directive. It is written in the following way
```
$ref{ имя_номера }
``` 
where `number_name` is the name of the number the directive refers to. For example the text
```
See [$ref{Susanto19}, $ref{Amalia20}].

References
$number{Amalia20|References}. Amalia, C.F. Title...
$number{Susanto19|References}. Susanto, D.A. Title...
```
will be transformed into
```
See [2, 1].

References
1. Amalia, C.F. Title...
2. Susanto, D.A. Title...
```
When automatically generating numbers in texts consisting of several separate files, the order in which the files are processed is important. The order can be set on the command line when starting the program with the `-o` (`--order`) option, followed by the file name with a list of files in processing order. In this file, each file name is specified on a separate line. You can also use the method when all required files are included in one main file using **#include** directives. In this case the order of file processing is determined by the order of **#include** directives.

### Directives for naming text fragments

Sometimes you need to insert a line in the text that appears elsewhere in the text. This may be necessary to avoid duplicating the text, and also to make sure that if you change the text in one place, it will automatically change in another. 

The **$name** directive is used to indicate the text that will be copied elsewhere. The format of the directive is
```
$name{text_name | string}
```
where `text_name` is a unique name that allows you to insert a string into other parts of the text using the $named directive; `string` is a sequence of characters. To insert a string from the **$name** directive into the text, the **$named** directive is used, for which the name of the string to be inserted is specified. For example, the text
```
1. $name{Chapter1 | Introduction}
...
In chapter <b>$named{Chapter1}</b>...
```
will be transformed into
```
1. Introduction
...
In chapter <b>Introduction</b>...
```

### Examples

Examples of how to use the directives can be found in the examples, which are located in the `/example` directory of the repository.

## Build and installation

### Linux

To install SMACRO on Linux you have to build program's binary from sources. Just clone the repo or download the sources from the project's releases page (https://github.com/vasyutin/smacro/releases) and unpack them. Then chdir to `/projects/gcc` and run

```sh
make
sudo make install
```

### Windows

The executable file for Windows can be downloaded at https://github.com/vasyutin/smacro/releases. The file is distributed as a ZIP archive. Extract the executable from the archive and put it in the directory that is listed in your PATH environment variable, to ensure that it can be called from a command prompt.

#### Build with MinGW

Clone the repo or download the sources from the project's releases pages (https://github.com/vasyutin/smacro/releases) and unzip them. Chdir to `/projects/gcc` and run

```sh
mingw32-make
``` 
The compiled binary is placed in the folder `/build/release` and named `smacro.exe` To use it put it in the directory is listed in your `PATH` environment variable.

### Build with Visual Studio

Clone the repo or download the sources from the project's releases pages (https://github.com/vasyutin/smacro/releases) and unzip them. Open the solution `/projects/vs2022/smacro.sln` in Visual Studio. Select `Release` configuration and run build. The compiled binary is placed in the folder `/build/x64/Release` and named `smacro.exe` To use it put it in the directory is listed in your `PATH` environment variable.


## License

GNU GPL Version 3

## Author

Sergey Vasyutin (sergey [at] vasyut.in)
