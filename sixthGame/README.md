# Memory: The Stack and the Heap

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


# Smart Pointers

Everything that goes wrong with raw pointers, from the leaks to the dangling pointers and the double trees, comes from having to match every `new` with exactly one `delete`, by hand. A smart pointer solves that. 

It's a small object that holds a raw pointer and does the cleanup for you: when the smart pointer goes away, it gives the memory back automatically. That measn no manual `delete`, no leaks from early returns, and no dangling pointers from forgetting to null things out.

Smart pointers come from the standard library, in the `<memory>` header. 

A smart pointer can point at any type, so you say which type in angle bracket, as in `std::unique_ptr<int>`, 

## unique_ptr

there's exactly one `unique_ptr` for the object at any time, and when that `unique_ptr` goes away, the object goes with it. 

`std::make_unique<int>(100)` creates an int on the heap, sets it to 100, and wraps it in a `unique_ptr`.

Because unique_ptr has exactly one object, there copying has to switched off on purpose. Another way to hands the object oer from one owner to anther, use `move(...)`

### When should you use a `unique_ptr`?
The short answer is: by default. Most of the time, when you create something on the heap, there's a single clear owner for it. It 's as fast as a raw pointer, it's safe, and it makes your intent obvious to anyone reading. 

## shared_ptr

Sometimes ownership really is shared. Several parts of a program hold on to the same object, and it should only go away when the last of them is finished with it. That's what `std::shared_ptr` is for. 

A `shared_ptr` keeps a count of how many `shared_ptr` values point at the same object. Each time you copy one, the count goes up, and each time one goes away, the count goes down. When it reaches zero, nobody is left holding the object, so it's destroyed.

use `use_count()` to keep trach on how many owners it has. 
