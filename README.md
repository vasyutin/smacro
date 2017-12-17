# SMACRO
Simple macro processor. A tool that helps to prepare documentation.

### Usage

```sh
$ smacro <switches>
```
The switches are:
**-i<input folder>** - the folder, containing documentation to process.
**-o<output folder>** - the destination folder for the processed documentation.
**-v<variables file>** - the file, containing values of the variables for the current run. The text in the file is assumed to be in UTF-8.
**-e<masks>** - The masks of filenames to exclude from processing. This files are only copied to the output folder. The masks are separated by semicolons.

All the files being processed (except the files excluded from the processing with the **-e** switch) are assumed to be in UTF-8 encoding.

To understand how this program works see **example** folder in the source tree.

Example of using the program in Windows:
```sh
smacro -i..\..\example\source -o..\..\build\doc_res -v..\..\example\config -e*.txt;*.png
```

Example of using the program in Linux/Unix:
```sh
smacro -i../../example/source -o../../build/doc_res -v../../example/config -e*.txt;*.png
```

### Installation

The QNetMap can be build in different ways depending of what build tools you are using. The standard distribution contains projects files for QMake/Qt Creator (for both Windows and Linux) and Visual Studio 2015. It is assumed that you have Qt5 installed and all the necessary build tools like compiler and **qmake** are available.

#### How to build and install with QMake in Linux

Just navigate to the folder **<project path>/projects/qmake** and run

```sh
$ qmake smacro.pro
$ make
$ make install
```

#### How to build with QMake in Windows

In Windows you don't have to make install step. You have to do it manually. 'Change directory' to folder **<project path>\projects\qmake** and run the following command in the command prompt

```sh
$ qmake smacro.pro
```
Then run the make tool that is installed in your system and is able to build Qt projects (depending on your setup it can be **nmake**, **make** etc.)

After that step put the result file **<project path>\build\release\smacro.exe** in the directory, that is listed in your PATH environment variable, to ensure that it can be called from a command prompt.

#### How to build with Qt Creator (Linux and Windows)

Just open the main project file **<project path>/projects/qmake/smacro.pro** and start building it using Qt Creator controls. As mentioned above all the result binaries are placed in the /build/release or /build/debug folder depending on configuration. You have to copy the result binary (**smacro** in Linux and **smacro.exe** in Windows) to proper location to make it available outside Qt Creator.

#### How to build with Visual Studio 2015 (Windows)

By now I supply solution (.sln) with the project files (.vcproj) files for Visual Studio 2015 Community Edition with Qt5. It is assumed that the environment variable QTDIR is defined for current version of Qt. And it contains the path for the Qt root folder (thus the path to the Qt binaries is '%QTDIR%\bin'). You don't have to install 'Visual Studio Add-in for Qt' (or some similar tool) to build the software.

To build the software open the solution file **<project path>\projects\VS2015-Qt5\smacro.sln** and start the build process. The compiled binaries will be placed in the folder **<project path>\build\Platform\Configuration**, where the 'Platform' is 'Win32' or 'x64' and the 'Configuration' is 'Debug' or 'Release'. To run the program outside Visual Studio, you have to copy the result binary file **<project path>\build\Platform\Configuration\smacro.exe** to the folder, that is added to your PATH environment variable.

License
----

GPL

### Author

Sergey Vasyutin (svpro [at] outlook.com)