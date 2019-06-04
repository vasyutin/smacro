# SMACRO
Simple macro processor. A tool that helps to prepare documentation.

### Usage

```sh
smacro <switches>
```
The switches are:\
*\-i\<input folder\>* - the folder, containing documentation to process.\
*\-o\<output folder\>* - the destination folder for the processed documentation.\
*\-v\<variables file\>* - the file, containing values of the variables for the current run. The text in the file is assumed to be in UTF-8.\
*\-e\<masks\>* - the masks of filenames to exclude from processing. This files are only copied to the output folder. The masks are separated by commas.

All the files being processed (except the files excluded from the processing with the *-e* switch) are assumed to be in UTF-8 encoding.

To understand how this program works see */example* folder in the source tree.

Example of using the program in Windows:
```sh
smacro -i..\..\example\source -o..\..\build\doc_res -v..\..\example\config -e*.txt;*.png
```

Example of using the program in Linux:
```sh
smacro -i../../example/source -o../../build/doc_res -v../../example/config -e*.txt;*.png
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

### License
GPL

### Author
Sergey Vasyutin (sergey [at] vasyut.in)