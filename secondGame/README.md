A variable is a named container for a piece of data.
There are two things we always have to tell the computer when we create a variable: its type and its name. 
* The type says what kind of thing will live in the box, such as a whole number, a decimal, a letter, a word, or a true-or-false value. 
* The name is what we use to refer to it in our code. 

float and double 

Similarity: Both types store floating-point numbers, so called because the decimal points can "float" to wherever it's needed.

difference: The difference between them is precision. A double uses twice as much memory as a float, and in return in keeps about twice as many significant digits. A float keeps about seven, and a double about 15.

Long and long long

Occasionally, an int isn't big enough. 
Here's the catch, and it catches plenty of experienced programmers too. ON windows, a long is exactly the same size as an int. However, on Linux and macos, it's bigger. In the preceding code, the L on the end marks the literal as a long, just as f marks a float. 

The type that's big enough on every system is long long, which is always at least 64 bits. the LL suffix marks the literal as a long long. 

**Unsigned Types and Exact Sizes**

Everything in memory is stored as bits. A bit is a single 0 or 1, and eight bits together make a byte. The number of bits a type uses decides how many different values it can hold. Each extra bit doubles the count, so eight bits make 256 different patterns. 

Formula to calculate number of pattern:
$2^n$ 
where n is number of bits.

An int uses 32 bits, which gives about 4.3 billion patterns. It spends half of them on negative numbers, which is why an int tops out at about 2.1 Billion. 

For example: Uint8

AN unsigned type spends all of its patterns on zero and up instead. So an unsigned int runs from 0 to about 4.3 bilion, and a Uint8, with only eight bits, from 0 to 255.

When the size really matters, C++ has types with the size built into their names, which come from a header called cstdint. 

an example of exact type: std::uint8_t

Ad leading u means unsigned, and without it the type is signed, meaning it can hold negative numbers. then comes int. the number of bits, and _t, which is just the standard library's habit. 

for signed ones: Sint8 or Sint64

size_t, the startdard library uses for sizes and counts 

**constexpr**
C++ also has a newer keyword, constexpr. it's simiilar to const, but stronger. It promises that the value is known at compile time, while the compiler is building the program, not just that it can't cahnge while the program runs. 

**C++ Structs**
Sometimes a single piece of data isn't really a single thing. A player's position on screen is two values, and x and a y. Colors are three values: red, green, and blue. We could store each part in a separate variable, but they belong together, and treating them as a group makes our code easier to think about.

A struct, short for structure, bundles several variables into one named unit. 

SDL_FRect is a struct for rectangle. 

SDL_Event bundles together the details of every kind of event SDL can report, and event.key is the part that describes a keyboard event. That part is itself a struct, of type SDL_KeyboardEvent, and its key member says which key it was. So event.key.key reaches in two levels: into the event's keyboard part, and then into its key.