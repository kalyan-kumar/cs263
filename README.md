# cs263
Runtime Systems course project

## Project Ideas
### 1. Concurrent Programming (Coroutines/Fibres)
* Multitasking is of two types - cooperative and preemptive.
	* In Cooperative style, the processes yield control voluntarily. Coroutines enable cooperative multitask implementation at the programming language level and are a generalization of subroutines. Fibres are an implementation at the system level.
	* Preemptive style is the most common kind of multitasking. Threads are system level implementation.
* Coroutines are implemented with async/await in single-threaded event loop programming languages. 
* This kind is suited for resource-constrained devices.

#### Probable directions of work
* Understand and analyse the benefits of each multitasking type. Explore the difficulties of implementing support at the programming language or system level.
* Explore how modern programming languages implement support for cooperative multitasking. Compare performance of these the different techniques employed.
* Implement full support for cooperative multitasking in a modern progarmming language that lacks it.

### 2. Meta Programming
* Metaprogramming is writing programs that operate on other programs. In particular metaprogramming also deals with writing a program that operates on itself.
* If a program simply looks at and reports on itself, we call this introspection. 
* If the program also modifies itself, we call this reflection.

#### Probable directions of work
* Explore the usefulness of meta programming in production level software.
* Analyse and compare how modern programming languages implement support for meta programming.
* Implement a general tool or framework to enable a developer to carry out meta programming easily.

### References
* [Metaprogramming](https://cs.lmu.edu/~ray/notes/metaprogramming/)
