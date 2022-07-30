# Projects for LUT course CT30A3370
These are my course project work for System development and OS course. From selection given to us I chose the following projects:
1. Project 1: Warmup to C and Unix programming
2. Project 2: Unix Utilities
3. Porject 3: Unix Shell

Selection was based from the list below:
https://github.com/remzi-arpacidusseau/ostep-projects

## Compiling my solutions
I've added an Makefile to my project root, so if you want to comile any of my projects you can do this with command make. You also need gcc installed to your Linux distro as
these applications are testing and developed in Ubuntu 20.04. Some of them works also in Mac OSX.

Example of compiling all is given below
```shell
make clean all
```

to run application you need to type to program you want run after compilation

Running reverse program
```shell
./reverse
```

list of programs:
- reverse (reverses given text from sdtout or from file)
- my-cat (cat implementation)
- my-zip (zipping using RLE)
- my-unzip (unzip RLE encoded files)
- my-grep (grep implementation)
- wish (my shell implementation)

folder test_files hold example text file to be used in zipping and unzipping
