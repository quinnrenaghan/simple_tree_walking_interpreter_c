# Simple Tree-Walking Interpreter in C
## Background
This repository contains a C implementation for a tree-walking interpreter. It can read and evaluate code written in a made-up language, and has support for string, integer, and boolean operations, conditional expressions, and functions. Language syntax and capabilities are shown below:

```
    set greeting = "hello"

    set make_greeting = fn(name, greeting){ 
                           return (name + " " + greeting);}

    make_greeting("quinn", greeting);

    set pemdas = 3 + 1 * 3;

    if(pemdas == 6){
      return "correct!";
    } else {
      return "incorrect!";
    }

```

## How it Works

The interpreter works in 3 main steps:
1. The **lexer** reads the input as a string, and converts it into tokens, the smallest units of syntax. These tokens are represented by a struct, which holds a token_type and a value.
   
2. The **parser** takes these tokens, and builds an AST (abstract syntax tree), which captures the relationship between each statement/expression. The nodes in the AST contain data, and pointers to other nodes.
   For example, an if-else statement would have nodes for the condition, consequence, and alternative (optional), which would be connected in a way that describes the program "flow".
  
3. The **evaluator** recursively "walks" the AST that the parser created, does what each node represents, and returns the value. It also utilizes an environment where identifiers will be stored (i.e. set x = 100), allowing for local/global variables.

To use the interpreter, download the source files and run start (found in REPL.C), with stdin and stdout or other FILE*.

## Application Examples:

### Operator Precendence
```
> set x = 3 + 8 * 9;
75
> set y = !(x == 75) == false;
true
> set z = 3 + 8 * 9 * 2 + 1 / (8 - 7) * 2
149
```

### Conditional Statements
```
> set num = 1001;
> set greaterthan = fn(x) {
                    if( x > 1000) {
                      return "x was greater than 1 thousand."
                    } else {
                      return "x was less than or equal to 1 thousand.";
                    }
                  }

> greaterthan(num);
"x was greater than 1 thousand."
```

### Function as Arguments
```
> set callthreetimes = fn(x, func) { return func(func(func(x))); }
> set double = fn(x) { (x * 2); }
> callthreetimes(2, double);
16
```
note: functions implicitly return the value of the last statement (if no other return is specified)!

### Function Scope
```
> set i = 10;
10
> set double = fn(i){ return (i * 2); }
> double(30);
60
> (i + 20)
30
```

## Garbage Collection
For garbage collection, I wrote a reference count garbage collector, which recovers the memory used by tokens, statements, and objects, except those which persist throughout a program (identifiers, function definitions, etc.). These objects are instead freed by the machine when the environment terminates at the end of the program. I deemed this acceptable, as the environment will not take up much memory, especially considering that this interpreter is desgined for small programs.

## Note
This interpreter was meant as a learning experience, and thus does not support many features found in typical programming languages. 
It supports: 
- **data types**: strings, integers, and booleans
- **logic**: math operators, boolean operators, string concatenation, conditional statements, assignments, return statements
- **functions**: higher-order functions, function calls, implicit returns
