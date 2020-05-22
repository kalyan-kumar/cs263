### Definition
Coroutine is a piece of code that 
* can be invoked by a caller,
* returns to the caller,
* can suspend itself before completion and
* can be resumed from where it was suspended.

So conceptually, these can be seen as a generalization of subroutines with suspend and resume support.

### Why Coroutines?
To execute a program that consists of various multiple independent sub tasks, we typically try to execute them asynchronously on multiple threads. There are two main problems in this model -
1. Threads are considerably expensive and increasing the number of threads with complexity of code is not scalable.
1. If a thread blocks, either due to expensive computations in a task or a blocking function call, it could block many other tasks increasing their time to finish.

* Coroutines on the other hand are very lightweight and some implementations claim to be able to support billions of them in a program. This is because Coroutines act as a state machine that requires a simple state object to be stored as opposed to an environment to be maintained for a thread.
* Coroutines enable cooperative multitasking at the programming language-level. Thus, an application could be coded in such a way that it would yield control to other tasks if it takes a long time to finish. This task could be restarted, thus distributing the huge computation time over a period, rather than at one point.
* Processes that depend on each other and need to have interleaved operation with each other (as in a Producer-Consumer model) can be implemented very easily in one single program, without the need of separate threads that communicate with IPC or shared memory (adding the over head of race-condition handling).

### Examples
#### Producer-Consumer problem
This example is taken from [Simon Tatham's blog post](https://www.chiark.greenend.org.uk/~sgtatham/coroutines.html).
**Producer -** 
```
/* Decompression code */
while (1) {
        c = getchar();
        if (c == EOF)
            break;
        if (c == 0xFF) {
            len = getchar();
            c = getchar();
            while (len--)
                emit(c);
        } else
            emit(c);
    }
    emit(EOF);
```

**Consumer -**
```
 /* Parser code */
while (1) {
        c = getchar();
        if (c == EOF)
            break;
        if (isalpha(c)) {
            do {
                add_to_token(c);
                c = getchar();
            } while (isalpha(c));
            got_token(WORD);
        }
        add_to_token(c);
        got_token(PUNCT);
    }
```
As we can see, the Decompresser would parse input and produce one character at a time, that will be consumed by the parser. In this form, they will have to be run on separate processes with IPC between them or as threads with shared memory. Both are very expensive solutions for a task as simple as passing data from one function to the other.

We could instead modify the parser to take in a character and operate on it, while producer invokes it. The consumer code would then get very messy as it would have to hold state about the what characters have been parsed up until now.

With Coroutines, this could be implemented easily with the consumer as a suspendable method. It would be in the same form as above, but would suspend itself after every character parsed. The Producer then gets the control back and can invoke the parser with the next character decompressed.

### Existing Implementations
* Kotlin has a full support for Coroutines in their recent releases.
* C++ has some standardized implementations.
* Golang has goroutines, that are implemented in a different way and are much more powerful than coroutines. I'm yet to explore how they are implemented.
* Lua has an [implementation of coroutines](https://www.lua.org/pil/9.1.html), but they are asymmetric rather than symmetric coroutines

### Possible directions of work
* Explore the techniques of implementing coroutine support and compare them in terms of added overhead and power of the construct.
* Implement coroutine support in a high-level language that lacks it.

### How can Coroutines be implemented?
#### Function calls on heap
* Programming languages implement function calls on the stack. Once a function is invoked, it runs till completion and then the corresponding stack frame is popped.
* This model is not suitable for coroutines as we will want to resume the function from the same point of execution.
* A natural solution here would be, when a coroutine wishes to suspend itself, its entire stack frame and the current instruction pointer could be saved on the heap.
* A pointer to this heap location would be stored in a table in memory that is indexed by the coroutine function name.
* When a suspended coroutine is to be resumed, we look for its saved address in our table and copy the stack frame from heap and jump to the saved instruction pointer.
**Disadvantages**
* There would be huge overhead due to copying the stack frame onto the heap for each coroutine invoke and suspend. Since the solution we are aiming for needs to support these invocations on a very huge scale, this overhead might be unacceptable.

* The C++ library we mentioned above implements a heap based solution, with optimizations to prevent this multiple memory copies. We plan to analyze how they do these optimizations in runtime.

#### Using static variables statements
* This solution is at the language-level. It maintains state of the funciton as a static variable and based on its value multiplexes to the desired part of the function using goto statements.
**Disadvantages**
* The programmer has to bear the overhead of defining the states properly and controlling their flow. Further, this method of implementation makes the code hard to follow. 
* Now, we would ideally like an implementation that does not put too much overfead on the programmer and also looks simple to parse. The following implementation method is one simple and elegant way in which this can be achieved.

**Macros to the rescue**
```
#define crBegin static int state=0; switch(state) { case 0:
#define crReturn(i,x) do { state=i; return x; case i:; } while (0)
#define crFinish }
int function(void) {
    static int i;
    crBegin;
    for (i = 0; i < 10; i++)
        crReturn(1, i);
    crFinish;
}
```
The above snippet suspends itself after each iteration of the loop and returns the value i. As we can see, the library simply provides the three macros that need to be used in a specific format to generate valid code. This lets the programmer use coroutines easily.

### Reference
* [Simon Tatham blog post](https://www.chiark.greenend.org.uk/~sgtatham/coroutines.html)
* [C++ Implementation Talk](https://www.youtube.com/watch?v=YYtzQ355_Co)
* [Why suspend?](https://medium.com/@elizarov/blocking-threads-suspending-coroutines-d33e11bf4761)

#### General Theory
* Activiation Frame for Coroutines consist of two parts - coroutine frame and stack frame. Coroutine frame consists of the data that needs to be persisted across suspensions, while stack frame has the data that need not be persisted.
* Stack frame can be implemented on the stack. Compiler has to have the intelligence of finding which data needs to go where.
* Suspend points are identified by the keywords `co_await` and `co_yield`.
* Things done when a coroutine hits a suspension point -
	* Prepare for suspension by storing register values and an identifier of which suspension point caused the suspension.
	* The coroutine is now considered suspended. It is given a handle to the coroutine frame and an opportunity to execute additional logic before transferring control back to the caller. This opportunity is to prevent a need for synchronization that would be needed due to a potential race condition (explained later).
	* Now the Coroutine can either transfer control back or resume execution. If control is transferred back, the stack frame is popped off the stack.
* A coroutine is resumed when `void resume()` is invoked on the coroutine frame. A stack frame is allocated as a normal function call and control is transferred to the resumption point.
* Destroy operation can be done on a coroutine frame handle, only on a suspended coroutine. Similar to the Resume operation, the activation frame is re-activated by allocating stack space and storing the return address. But instead of jumping to the last suspend piont, an alternate code path is taken that invokes destructors of all data in the coroutine frame. `void destroy()` is invoked on the coroutine frame handle to invoke the destroy operation.
* The caller of a coroutine has the same interface as a regular function. Stack space is allocated as usual and control is transferred to the coroutine. The coroutine is then responsible for allocating space on the heap for the coroutine frame and transfer the needed data to the coroutine frame.
* Return operation -
	* `co_return` statement executed from inside the coroutine to indicate returning.
	* The return-value is stored somewhere (customizable by the coroutine) and destructs any in-scope data structures.
	* An opporunity to execute some additional logic is given. Here, the return-value could be published, or another suspended coroutine
	* The coroutine can now either suspend or destroy itself. Control is transferred back according to the suspend or destroy operation semantics.
	* The return value given to the caller is different from the return value of the return semantics. The return-value of return semantics could go to the caller long after the caller has resumed execution.
* How do we access local variables on the stack? What if the allocated memory runs out?

#### Implementation HLD
* Facilities provided by the C++ Coroutines TS are like a low-level assembly-language for coroutines. These can be difficult to use directly in a safe way and are intended to be used by library-writers to build higher-level abstractions that application developers can work with safely.
