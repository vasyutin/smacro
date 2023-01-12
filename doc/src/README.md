${DO_NOT_EDIT_COMMENT}

@//============================================================================
@if LANGUAGE=="Ru"
# Макропроцессор SMACRO

## Назначение и функции

SMACRO - простой макропроцессор, который предназначен для подготовки нескольких вариантов документации на основе одних исходных файлов. Область применения SMACRO не ограничивается разработкой документации, программа работает с любыми текстовыми файлами, в том числе с текстами программ.

Основные функции:
* включение в итоговые текстовые файлы блоков исходных текстовых файлов в зависимости от значений переменных, заданных в конфигурационных файлах;
* подстановка вместо переменных в исходных текстовых файлах их значений, заданных в конфигурационных файлах;
* автоматическая нумерация разделов, рисунков и пр. в тексте.
@//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
@else
# SMACRO macroprocessor

## Purpose and functions

SMACRO is a simple macro processor that is designed to produce multiple versions of documentation based on the same source files. The scope of SMACRO is not limited to documentation development, the program works with any text files, including source files.

The main functions are:
* inclusion of blocks of source text files in the resulting text files depending on the values of the variables specified in the configuration files;
* substitution of the variables in the source text files with the values set in the configuration files;
* automatic numbering of sections, pictures, etc. in the text.
@endif
@//----------------------------------------------------------------------------

@//============================================================================
@if LANGUAGE=="Ru"
## Принцип работы

SMACRO копирует файлы из папки исходных файлов в папку, указанную как папка назначения. При этом при копировании SMACRO подставляет вместо переменных в исходных файлах их значения, указанные в конфигурационном файле с переменными. Кроме того в зависимости от истинности выражений, составленных из этих переменных, может копировать или не копировать фрагменты текста из исходных файлов в результирующие. Также при копировании SMACRO выполняет автоматическую нумерацию фрагментов текста, помеченных специальными тегами. 

При запуске SMACRO можно указать шаблоны или имена файлов, которые не обрабатываются при копировании (то есть копируются в папку назначения без изменений), и шаблоны или имена файлов, которые игнорируются программой (то есть не копируются в папку с результатами).
@//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
@else
## Operating Principle

SMACRO copies files from the source files folder to the folder specified as the destination folder. When copying, SMACRO substitutes the variables in the source files with the values specified in the variable configuration file. In addition, depending on the logical value of the expressions composed of those variables, it may or may not copy text fragments from the source files to the resulting files. Also, when copying, SMACRO performs auto-numbering of text fragments marked with special tags. 

When you start SMACRO, you can specify templates of file names or file names that are not processed when copying (i.e. are copied without changes) and templates of file names or file names that are ignored by the program (i.e. are not copied to the results folder).
@endif
@//----------------------------------------------------------------------------

@//============================================================================
@if LANGUAGE=="Ru"
## Использование
@//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
@else
## Usage
@endif
@//----------------------------------------------------------------------------

```
@//============================================================================
@if LANGUAGE=="Ru"
smacro  -s <папка исходных файлов>
        -d <папка в которую записывается результат обработки> 
        -v <файл с переменными>
        [-e <шаблоны файлов, которые не обрабатываются> ...] 
        [-i <шаблоны файлов, которые игнорируются> ...] 
        [-o <файл, в котором перечислены файлы в том порядке, в котором они обрабатываются>] 
        [-@h] [--version] 
где:
@//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
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
@//----------------------------------------------------------------------------

@//============================================================================
@if LANGUAGE=="Ru"
  -s <папка исходных файлов>,  --source <папка исходных файлов>
     (обязательный параметр) Имя папки, содержащей файлы для обработки.
	  
  -d <папка с результатом обработки>,  --destination <папка с результатом обработки>
     (обязательный параметр) Имя папки, в которую записывается результат обработки.
	  
  -v <файл с переменными>,  --variables <файл с переменными>
     (обязательный параметр) Имя файла, содержащего значения переменных, которые 
     используются при обработке.
@//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
@else
  -s <source folder>,  --source <source folder>
     (required) The name of the folder containing the files to be processed.
	  
  -d <destination folder>,  --destination <destination folder>
     (required) The name of the folder where the result of the processing is written.

  -v <file with variables>,  --variables <file with variables>
     (required) The name of the file containing the values of the variables used 
     in processing.
@endif
@//----------------------------------------------------------------------------

@//============================================================================
@if LANGUAGE=="Ru"
  -e <шаблоны файлов, которые не обрабатываются>,  
  --exclude <шаблоны файлов, которые не обрабатываются>
     (можно использовать несколько раз) Шаблоны или имена файлов, которые не обрабатываются 
     при копировании, то есть копируются без изменений в папку с результатами обработки.

  -i <шаблоны файлов, которые игнорируются>,  
  --ignore <шаблоны файлов, которые игнорируются>
     (можно использовать несколько раз) Шаблоны или имена файлов, которые которые 
     игнорируются программой, то есть не копируются в папку с результатами.
@//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
@else
  -e <exclude templates>,  --exclude <exclude templates>  (accepted multiple times)
     Templates or file names that are not processed when copied, i.e. they are copied 
     unchanged to the folder with the results of processing.

  -i <ignore templates>,  --ignore <ignore templates>  (accepted multiple times)
     Templates or file names that are ignored by the program, i.e. they are not 
     copied to the results folder.
@endif
@//----------------------------------------------------------------------------

@//============================================================================
@if LANGUAGE=="Ru"
  -o <имя файла со списком файлов в порядке обработки>,
  --order <имя файла со списком файлов в порядке обработки>
     Имя файла, в котором перечислены исходные файлы в том порядке, в котором они 
     будут обрабатываться. Может потребоваться для правильной автонумерации 
     фрагментов текста.

   -@,  --at-prefixed
     Использовать символ '@' вместо '#' в директивах. То есть вместо директив 
     #//, #if, #elif, #else, #endif and #include использовать директивы 
     @//, @if, @elif, @else, @endif and @include. Позволяет сделать более 
     читаемыми управляющие конструкции языка SMACRO в файлах, в которых часто
     используется символ '#', например, в файлах на языке Markdown. Задание
     этого ключа влияет на все исходные файлы.
@//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
@else
  -o <order file>,  --order <order file>
     The name of file that lists the source files in the order in which they 
     will be processed. May be required for proper auto-numbering of text fragments.

   -@,  --at-prefixed
     Use the '@' symbol instead of '#' in control directives. Instead of using 
     #//, #if, #elif, #else, #endif and #include you'll be able to use 
     @//, @if, @elif, @else, @endif and @include directives. Allows you to make 
     control directives more readable in files where the '#' symbol is used frequently 
     for example in Markdown files. Setting this switch affects all the source files.
@endif
@//----------------------------------------------------------------------------

@//============================================================================
@if LANGUAGE=="Ru"
   --,  --ignore_rest
     Игнорировать аргументы после этого флага.

   --version
     Показывает версию программы и завершает её работу.

   -h,  --help
     Показывает информацию об использовании программы и завершает её работу.
@//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
@else
   --,  --ignore_rest
     Ignores the rest of the labeled arguments following this flag.

   --version
     Displays version information and exits.

   -h,  --help
     Displays usage information and exits.
@endif
@//----------------------------------------------------------------------------
```
@//============================================================================
@if LANGUAGE=="Ru"
## Файл переменных

Значения переменных, которые используются в процессе обработки текста, задаются в файле, имя которого передается как параметр командной строки. Каждое значение переменной задается в отдельной строке с помощью конструкции
```
имя_переменной=значение
```
где имя переменной - это последовательность строчных и заглавных латинских букв, цифр и знаков подчеркивания. Имя переменной должно начинаться с буквы или знака подчеркивания. Значение - это последовательность любых символов до конца строки (пробелы в конце строки игнорируются). Например,
```
LANGUAGE=Italiano
ProgramDescription=SMACRO is a simple macro processor that is designed...
_div01=<br>
```
Пустое значение задается строкой с именем переменной, например
```
VERSION_FOR_WEB
VERSION_FOR_PRINT
```
В файле переменных пустые строки и строки, начинающиеся с символа #, игнорируются.
@//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
@else
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
@endif
@//----------------------------------------------------------------------------

@//============================================================================
@if LANGUAGE=="Ru"
## Директивы обработки

### Директива подстановки значения переменной

Конструкция вида
```
DOLLAR_SIGN{имя_переменной}
```
заменяется в тексте на значение указанной переменной. Если такая переменная не определена в файле переменных, то выполнение программы завершится с сообщением об ошибке.

### Директивы управления обработкой

Язык SMACRO поддерживает несколько директив, управляющих тем, какие фрагменты файла включаются в его результирующую версию. Директивы должны находиться в начале строки, перед ними могут быть пробелы и знаки табуляции, но не должно быть других символов. Строки с директивами не копируются в результирующую версию файла.

**#//** - директива комментария. Строка, начинающаяся с этого оператора, игнорируется.

**#if**, **#elif**, **#else**, **#endif** - директивы условного оператора. Выполняют ту же роль, что и эти же директивы препроцессора в языке C/C++. В условиях могут использоваться выражения, которые проверяют, определены ли переменные, а также проверяют их значения. Блоки текста, для которых условия выполняются, переписываются в итоговые файлы. Пример задания условного оператора:
@//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
@else
## Processing directives

### Directive for variable value substitution

A construct of the form
```
DOLLAR_SIGN{variable_name}
```
is replaced in the text by the value of the specified variable. If such a variable is not defined in the variable file, the program will terminate with an error message.

### Directives for controlling the processing

The SMACRO language supports several directives that control which fragments of a file are included in its resulting version. Directives must be at the beginning of the line possibly preceded by spaces and tabs. Lines with directives are not copied into the resulting version of the file.

**#//** - comment directive. A line starting with this operator is ignored.

**#if**, **#elif**, **#else**, **#endif** - conditional operator directives. They play the same role as the preprocessor directives in C/C++. Conditions may use expressions that check if variables are defined and also check their values. Blocks of text for which the conditions are met are overwritten in summary files. An example of setting a conditional statement:
@endif
@//----------------------------------------------------------------------------
```
#if (defined(VAR1) && !defined(VAR2)) || Language == "Slovenščina"
	Besedilo na slovenskem
#elif Language == "Italiano"
   Un testo in italiano
#else
	Text in English
#endif
```

@//============================================================================
@if LANGUAGE=="Ru"
**#include** - директива включения файла. Вместо строки с этой директивой подставляется содержимое указанного в ней файла. Имя включаемого файла задается в квадратных скобках также, как в макропроцессоре C/C++. Например,

```
#include<./include/some-file.txt>
```
Для удобства при указании имени файла можно использовать встроенную переменную DOLLAR_SIGN{SMACRO_ROOT}, которая содержит путь к папке с исходными файлами.

```
#include<DOLLAR_SIGN{SMACRO_ROOT}/include/file.inc>
```

Если при запуске программы указать ключ `-@` или `--at-prefixed`, то вместо директив #//, #if, #elif, #else, #endif и #include надо будет использовать директивы @//, @if, @elif, @else, @endif и @include. Это позволяет сделать более читаемыми управляющие конструкции языка SMACRO в файлах, в которых часто используется символ `#`, например, в файлах на языке Markdown. Задание этого ключа влияет на все исходные файлы при обработке.
@//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
@else
**#include** - file inclusion directive. Instead of the string with this directive the contents of the file specified in it are substituted. The name of the included file is specified in square brackets, just like in the C/C++ macro processor. For example,
```
#include<./include/some-file.txt>
```
For convenience, you can use the built-in variable DOLLAR_SIGN{SMACRO_ROOT}, which contains the path to the folder with the source files, when specifying the file name.
```
#include<DOLLAR_SIGN{SMACRO_ROOT}/include/file.inc>
```
If you specify the `-@` or `--at-prefixed` key when starting a program, you will have to use the directives @//, @if, @elif, @else, @endif and @include instead of the directives #//, #if, #elif, #else, #endif and #include. This allows you to make SMACRO language control constructions in files that often use the `#` symbol, such as Markdown files, more readable. Setting this key affects all source files during processing.
@endif
@//----------------------------------------------------------------------------

@//============================================================================
@if LANGUAGE=="Ru"
### Выражения в условных директивах

Выражения в директивах **#if** и **#elif** строятся по правилам, принятым для языков C и C++. Они могут включать в себя операторы и скобки, управляющие порядком вычисления выражений. Можно использовать следующие операторы:

* **defined(имя_переменной)** - оператор проверки того, определена ли переменная в файле переменных. Возвращает true, если переменная определена и false в противном случае. Пример использования:
@//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
@else
### Expressions in conditional directives

Expressions in **#if** and **#elif** directives are built according to the rules accepted for C and C++ languages. They may include operators and brackets controlling the order of expressions evaluation. The following operators can be used:

* **defined(variable_name)** is an operator to check if a variable is defined in a variable file. Returns true if the variable is defined and false otherwise. Example usage:
@endif
@//----------------------------------------------------------------------------
```
#if defined(VERSION) && defined(FOR_PRINT)
	...
#endif
```
@//============================================================================
@if LANGUAGE=="Ru"
* операторы сравнения **==** (равно), **!=** (не равно), **>** (больше), **>=** (больше или равно), **<** (меньше) и **<=** (меньше или равно) выполняют лексикографическое сравнение значений переменных или строковых констант. Строковые константы заключаются в двойные кавычки. Операторы возвращают true при истинности условия, или false при его ложности. Примеры использования операторов сравнения:
@//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
@else
* **==** (equal), **!=** (not equal), **>** (more), **>=** (more or equal), **<** (less) and **<=** (less or equal) comparison operators perform lexicographical comparison of variable or string constant values. String constants are enclosed in double quotes. Examples of using comparison operators:
@endif
@//----------------------------------------------------------------------------
```
#if (VERSION > "1" && Language == "English") || Language == "Italiano"
   #if TARGET == "HTML"
	   <hr>
	#else
	   ------------
	#endif
#endif
```
@//============================================================================
@if LANGUAGE=="Ru"
* оператор логического И **&&** возвращает true при истинности обоих операндов. Пример использования:
@//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
@else
* the boolean AND operator **&&** returns true if both operands are true. Example usage:
@endif
@//----------------------------------------------------------------------------
```
#if (VERSION > "1" && Language == "English") && defined(PRINT_VERSION)
   ...
#endif
```
@//============================================================================
@if LANGUAGE=="Ru"
* оператор логического ИЛИ **||** возвращает true при истинности хотя бы одного из операндов. Пример использования:
@//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
@else
* the logical OR operator **||** returns true if at least one of the operands is true. Example usage:
@endif
@//----------------------------------------------------------------------------
```
#if VERSION > "1" || Language == "English" || defined(PRINT_VERSION)
   ...
#endif
```
@//============================================================================
@if LANGUAGE=="Ru"
* оператор отрицания **!** инвертирует значение находящегося за ним выражения. Пример использования:
@//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
@else
* the negation operator **!** inverts the value of the expression behind it. Example usage:
@endif
@//----------------------------------------------------------------------------
```
#if !(VERSION > "1" || Language == "English") || !defined(PRINT_VERSION)
   ...
#endif
```
@//============================================================================
@if LANGUAGE=="Ru"
Выражение, которое не умещается на одной строке, может быть перенесено на следующую строку если указать символ `\` последним в строке, например
@//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
@else
An expression that does not fit to one line can be carried to the next line by specifying the `\` character last in the line, for example
@endif
@//----------------------------------------------------------------------------
```
#if !(VERSION > "1" || Language == "English") || \
   !defined(PRINT_VERSION)
   ...
#endif
```

@//============================================================================
@if LANGUAGE=="Ru"
### Директивы для автоматической нумерации

В текстах, в которых необходимо автоматически нумеровать части, разделы, рисунки и пр. используются директивы **$number** и **$ref**. Директива **$number** заменяется на увеличенное на единицу значение заданного счетчика. Она записывается в тексте следующим образом
@//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
@else
### Directives for automatic numbering

In texts where you want parts, sections, pictures, etc. to be numbered automatically, the **$number** and **$ref** directives are used. The **$number** directive is replaced by a count incremented by one. It is written in the text as follows
@endif
@//----------------------------------------------------------------------------
```
DOLLAR_SIGNnumber{ имя_номера | имя_счетчика }
```
@//============================================================================
@if LANGUAGE=="Ru"
где `имя_номера` - строка, которая позволяет ссылаться на полученное в этом месте текста значение счетчика в директиве **$ref** в других частях текста; `имя_счетчика` - счетчик, значение которого увеличивается при вставке номера вместо директивы **$number**. Имена номера и счетчика представляют собой текстовые строки без пробелов и знаков препинания. Например, следующие директивы
@//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
@else
where `number_name` is a string that allows you to refer to the counter value obtained in this place of text in the directive **$ref** in other parts of the text; `number_name` is a counter, the value of which increases when inserting the number instead of $number directive. Number and counter names are text strings without spaces and punctuation marks. For example, the following directives
@endif
@//----------------------------------------------------------------------------
```
DOLLAR_SIGNnumber{number_intro | chapters}
DOLLAR_SIGNnumber{number_mainpart | chapters}
DOLLAR_SIGNnumber{number_figure_structure | figures}
DOLLAR_SIGNnumber{number_figure_streams | figures}
DOLLAR_SIGNnumber{number_conclusion | chapters}
```
@//============================================================================
@if LANGUAGE=="Ru"
будут преобразованы в такой текст
@//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
@else
will be transformed into this text
@endif
@//----------------------------------------------------------------------------
```
1
2
1
2
3
```
@//============================================================================
@if LANGUAGE=="Ru"
Для ссылки в тексте на номер, вставленный директивой **$number**, используется директива **$ref**. Записывается она следующим образом
@//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
@else
The **$number** directive is used to refer in the text to the number inserted by the **$number** directive. It is written in the following way
@endif
@//----------------------------------------------------------------------------
```
DOLLAR_SIGNref{ имя_номера }
``` 
@//============================================================================
@if LANGUAGE=="Ru"
где `имя_номера` - имя номера, на который ссылается директива. Например текст
@//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
@else
where `number_name` is the name of the number the directive refers to. For example the text
@endif
@//----------------------------------------------------------------------------
```
See [DOLLAR_SIGNref{Susanto19}, DOLLAR_SIGNref{Amalia20}].

References
DOLLAR_SIGNnumber{Amalia20|References}. Amalia, C.F. Title...
DOLLAR_SIGNnumber{Susanto19|References}. Susanto, D.A. Title...
```
@//============================================================================
@if LANGUAGE=="Ru"
будет преобразован в текст
@//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
@else
will be transformed into
@endif
@//----------------------------------------------------------------------------
```
See [2, 1].

References
1. Amalia, C.F. Title...
2. Susanto, D.A. Title...
```
@//============================================================================
@if LANGUAGE=="Ru"
При автоматической генерации номеров в текстах, состоящих из нескольких отдельных файлов, важен порядок обработки файлов. Он может быть задан в командной строке при запуске программы опцией `-o` (`--order`), после которой следует имя файла со списком файлов в порядке обработки. В этом файле каждое имя файла задается на отдельной строке. Также можно использовать способ, когда все требуемые файлы включаются в один главный с помощью директив **#include**. В этом случае порядок обработки файлов определяется порядком директив **#include**.
@//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
@else
When automatically generating numbers in texts consisting of several separate files, the order in which the files are processed is important. The order can be set on the command line when starting the program with the `-o` (`--order`) option, followed by the file name with a list of files in processing order. In this file, each file name is specified on a separate line. You can also use the method when all required files are included in one main file using **#include** directives. In this case the order of file processing is determined by the order of **#include** directives.
@endif
@//----------------------------------------------------------------------------

@//============================================================================
@if LANGUAGE=="Ru"
### Директивы для именования фрагментов текста

Иногда необходимо вставить в текст строку, которая появляется в тексте в другом месте. Это может потребоваться, чтобы не дублировать текст, а также, чтобы при изменении текста в одном месте, он автоматически изменялся в другом. 

Для обозначения текста, который будет скопирован в другое место, используется директива **$name**. Формат ее записи следующий
```
DOLLAR_SIGNname{имя_текста | строка}
```
где `имя_текста` - уникальное имя, которое позволяет вставлять строку в других частях текста с помощью директивы $named; *строка* - последовательность символов. Для вставки в текст строки из директивы **$name** используется директива **$named**, для которой указывается имя строки, которую нужно вставить. Например, текст
@//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
@else
### Directives for naming text fragments

Sometimes you need to insert a line in the text that appears elsewhere in the text. This may be necessary to avoid duplicating the text, and also to make sure that if you change the text in one place, it will automatically change in another. 

The **$name** directive is used to indicate the text that will be copied elsewhere. The format of the directive is
```
DOLLAR_SIGNname{text_name | string}
```
where `text_name` is a unique name that allows you to insert a string into other parts of the text using the $named directive; `string` is a sequence of characters. To insert a string from the **$name** directive into the text, the **$named** directive is used, for which the name of the string to be inserted is specified. For example, the text
@endif
@//----------------------------------------------------------------------------
```
1. DOLLAR_SIGNname{Chapter1 | Introduction}
...
In chapter <b>DOLLAR_SIGNnamed{Chapter1}</b>...
```
@//============================================================================
@if LANGUAGE=="Ru"
будет преобразован в текст
@//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
@else
will be transformed into
@endif
@//----------------------------------------------------------------------------
```
1. Introduction
...
In chapter <b>Introduction</b>...
```

@//============================================================================
@if LANGUAGE=="Ru"
### Примеры

С примерами использования директив можно ознакомиться в примерах, которые находятся в файлах с каталоге `/example` репозитория.
@//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
@else
### Examples

Examples of how to use the directives can be found in the examples, which are located in the `/example` directory of the repository.
@endif
@//----------------------------------------------------------------------------

@//============================================================================
@if LANGUAGE=="Ru"
## Сборка и установка программы

### Linux

Для установки SMACRO под Linux необходимо выполнить сборку выполняемого файла из исходных текстов. Склонируйте репозиторий или загрузите архив с исходными текстами со страницы релизов (https://github.com/vasyutin/smacro/releases) и распакуйте его. Затем перейдите в каталог `/projects/gcc` и запустите команды

```sh
make
sudo make install
```
@//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
@else
## Build and installation

### Linux

To install SMACRO on Linux you have to build program's binary from sources. Just clone the repo or download the sources from the project's releases page (https://github.com/vasyutin/smacro/releases) and unpack them. Then chdir to `/projects/gcc` and run

```sh
make
sudo make install
```
@endif
@//----------------------------------------------------------------------------

@//============================================================================
@if LANGUAGE=="Ru"
### Windows

Исполняемый файл программы может быть загружен со страницы релизов https://github.com/vasyutin/smacro/releases. Файл находится в ZIP-архиве. Извлеките исполняемый файл из архива и запишите его в папку, имя которой находится в списке в переменной окружения PATH. Это позволит запускать его из командной строки.

**Сборка исполняемого файла с помощью MinGW**

Склонируйте репозиторий или загрузите архив с исходными текстами со страницы релизов (https://github.com/vasyutin/smacro/releases) и распакуйте его. Затем перейдите в каталог `/projects/gcc` и запустите команду

```sh
mingw32-make
```
Результат компиляции находится в папке `/build/release`. Скопируйте файл `smacro.exe` в папку, имя которой находится в списке в переменной окружения `PATH`.

**Сборка исполняемого файла с помощью Visual Studio**

Склонируйте репозиторий или загрузите архив с исходными текстами со страницы релизов (https://github.com/vasyutin/smacro/releases) и распакуйте его. Откройте сольюшн `/projects/vs2022/smacro.sln` в среде Visual Studio. Выберите конфигурацию `Release` и запустите процесс сборки. Результат компиляции будет находиться в папке `/build/x64/Release`. Скопируйте файл `smacro.exe` в папку, имя которой находится в списке в переменной окружения `PATH`.

@//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
@else
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

@endif
@//----------------------------------------------------------------------------

## ${LICENCE}

GNU GPL Version 3

## ${AUTHOR}

Sergey Vasyutin (sergey [at] vasyut.in)
