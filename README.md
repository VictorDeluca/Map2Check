<h1>Map2Check</h1>
<h3>Memory Management Test Cases Generation of C Programs</h3>

================ 

          .-.          
          /v\
         // \\    > L I N U X - GPL <
        /(   )\
         ^^-^^
         

<b>Goal:</b> 
<p align="justify">
     Map2Check is a tool of automatically generating and checking memory management unit tests in C programs. 
     The generation of unit tests is based on assertions extracted from safety properties generated by Bounded Model 
     Checking tools. Map2Check checks for properties related to memory safety, such as: memory leaks and invalid free.
</p>

================

- Requirements for using the tool<br>
To use this tool is necessary that the system contains the following software already installed properly:

> - Linux OS
> - Python (v2.7.1 or higher);
> - Perl;
> - GCC compiler; 
> - Pycparser (v2.10) - https://github.com/eliben/pycparser
> - Pyparsing (v1.5.6) - http://pyparsing.wikispaces.com
> - Networkx (v1.9.1) - https://networkx.github.io
> - Uncrustify (v0.60) - http://uncrustify.sourceforge.net
> - Boost - http://www.boost.org
> - Clang - http://clang.llvm.org
> - LLVM - http://www.llvm.org


================


<b>How to install Map2Check?</b>

<p align="justify">
First of all, you need to install the required packages:
</p>

- <b>STEP 0:</b>

> - Pycparser : $ pip install python-pycparser
> - Pyparsing : $ pip install python-pyparsing
> - Networkx  : $ pip install python-networkx
> - Uncrustify : $ apt-get install uncrustify
> - Boost : $ apt-get install libboost-all-dev
> - Clang : $ apt-get install clang
> - LLVM : $ apt-get install llvm-devel llvm-static llvm-libs libedit-devel


- <b>STEP 1:</b>

<p align="justify">
In order to install Map2Check on your PC, you should download and save the map2check.zip file on your disk. 
After that, you should type the following command:
</p>

> $ unzip map2check.zip

or from https://github.com

> $ git clone https://github.com/hbgit/Map2Check.git

- <b>STEP 2:</b>

<p align="justify">
Build tool: $ make  
</p>

- <b>STEP 3:</b>

<p align="justify">
Testing tool. Map2Check can be invoked through a standard command-line interface. Map2Check should be called 
in the installation directory as follows:  
</p>

> $ ./map2check code_samples/samples/false_free_2.c 

For help and others options: 

> $ ./map2check -h



===========================

<b> Authors </b>

Maintainers:
  Herbert O. Rocha (since 2014), Federal University of Roraima, Brazil <\br>
  Rafael Menezes   (since 2016), Federal University of Roraima, Brazil <\br>

Questions and bug reports:  
  E-mail: map2check.tool@gmail.com




