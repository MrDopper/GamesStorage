## Memory: The Stack and the Heap

every variable in every program has lived somewhere in the computer's memory. 

A running C++ program keeps its data in two main regions of memory: the **stack** and the **heap**. 

### The Stack

Eveything time a function is called, a stack frame goes on top, holding that call's parameters and local variables, and when the function returns, its frame comes off again. 

The stack is fast, it's small, and it's automatic. 

The stack is fast because adding and removing a frame is trivial: the program just moves a single marker up and down. It's also limited in size. Use too much of it, the stack overflow will appear. 

### The Heap 

The heap is a much bigger pool of memory. Unlikely the stack, nothing on the heap is automatic. When you want memory from the heap, you ask for it, an when you're done with it, you have to give it back.
 **Note**: Forget to give it back, and that memory stays taken until your program ends. THat's called a **memory leak**, and in a long-running program like a game, it can be a serious problem. 

If the stack is a neat pile of plates, the heap is the ware house out back. It's vastly bigger, but nobody keeps track of what you took. You have to remember. 

Three reasons to use heap:

1. **lifetime beyond a single function**. A variable on the stack dies when its function returns. Sometimes we need data that outlives the function that created it. 

2. **Big things**. The stack is small. something big, such as texture, a level's worth of tiles, or a list of ten thousand enenimes, belongs on the heap.

3. **Sizes you don't know in advance**. Sometimes you don't know how much data you'll need until the program is running. THe stack needs every size to be known when the program is complied, and the heap doesn't.

## References, Revisited

References have two important rules:
- First, a reference must be set up when it's created. you can't create a reference to nothing and decide later.

- Second, once a reference is tied to a variable, it can never be tied to a different one. 

References are safe, clean, and limited. They always refer to something real, they can never refer to nothing, and they can never be moved to a differnt target. When all you need is another name for an exisiting variable, a reference is almost always the right tool.

## Raw Pointers 

A pointer is a variable whose value is a momory address. Where an ordinary int stores a number like 100, a pointer stores the location of an int somewhere in memory. 

An address on its own isn't much use. WE want to read or write the value that lives there. For that, we **dereference** the pointer with `*`. 

**Note**: 
- A reference is another name.
- A pointer holds an address.
