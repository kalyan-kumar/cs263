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

#### Scope
* In this project we will be implementing a low level language that supports coroutines by interpreting programs.
* We are not aiming for a fully-functional language but a Proof of Concept implementation that could be used to easily understand and appreciate the usefulness of Coroutines.
* Initially we wanted to extend the Java interpreter, but realized it would be too hard to work on top of JVM.
* So we decided to work on a simple language that supports minimal features and focus solely on coroutines.
* Stackless Coroutines need language support, which is why most of the existing implementations stack-based libraries.
* We observe that it would be possible to interpret programs to support Stackless Coroutines. This will allow us have a library implementation.

#### High level design
* We will be implementing Corouties for C language.
* [PicoC](https://gitlab.com/zsaleeba/picoc) is an interpreter for C. It supports most of the features of C language.
* We aim to implement Coroutine support in PicoC.
* Our interpreter would implement operations as a stack machine, without any registers. Thus, when Coroutine needs to be suspended, we will have to preserve the stack memory and resume address.
* At a high level, when the interpreter encounters a coroutine invocation, it would set certain flags to signal that the function might be suspended. When the suspend instruction is hit, current function activation frame is stored to the heap, along with metadata indicating which coroutine the frame corresponds to and control a pointer to the frame is returned.
* When the caller wishes to resume the function, it will have to invoke resume on the frame's data structure. This will cause the stack frame to be copied back from heap onto the stack.

#### References
* [CPP Coro - Github Repo](https://github.com/lewissbaker/cppcoro)
