#### Introduction
The goal of the project is three fold:
* To study and understand the working of corotinues.
* Analyse one of the existing implementations of Corotinues for C++ (CPPCoro). 
* Develop a library that supports basic functionalities of Corotinues for Java.

#### Key End Goals
* Implement a corotinue library similar to the existing one for C++ (CPPCoro) for Java.
* Modify existing interpreter for Java in order to efficiently handle corotinues.

#### Motivation
* C++ does not have a managed runtime, thus the library implementation for C++ that we look at is good for low level languages like C++.
* For HLL languages like Java that have a managed runtime, it would be benefical to reap the effects out of the runtime to efficiently implement corotinues.

#### Approach
* In this project, taking into account the time constraints, we aim to firstly port the existing Corotinue implementation for C++ to Java.
* After having a skeletal Corotinue implementation for Java that does not interfere with the runtime, we would ideally like to modify the interpreter to efficiently support Coroutines.
* We aim to modify the interpretor since it is one of the easiest components to play with in a managed runtime for any HLL.
* A major factor for considering Java is that it is one of the first high level langugage that came into existence and many of the other high level languages have more complicated runtimes and were built using Java as a prototype. 
* Thus, implementing a Corotinue that is integrated with the runtime for Java would be useful in designing Corotinues for other HLLs by serving as as a base model.
 
#### References
* [CPP Coro - Github Repo](https://github.com/lewissbaker/cppcoro)
