# Guidelines for JPEG Pleno reference software development

## Introduction

C++ is the development language used to develop the JPEG Pleno's codec and required by the committee. This language language has many features, which can induce bug-prone code and make it harder to read and maintain.

The goal of this guide is to manage this complexity by describing in generality the rules to make the JPEG Pleno reference software readable while still allowing coders to use C++ language features productively.

Since this document is intended to be a general styling recommendation, it do not describe in detail the dos and don'ts of writing C++ code. If the developer identify some feature use that is not present in this document, we recommend the use of [Google C++ Style Guide](https://www.google.com "Google C++ Style Guide"). In fact, this document is inspired in the Google C++ Style Guide. However, in case of conflicts between this document and the Google's C++ code style, this document must precede.

The use of the following rules is mandatory for all software implementation tasks. Code commits that do not comply with these rules should not be accepted by the software coordinators.

The JPEG Pleno software contains legacy code using different styles. When working on sections using old styles, that are different from the rules in this document, it is encouraged to update the related sections to the new style. If you found some code snippet that differs from the rules in this document, please report it.


## ITU/ISO/IEC License

The license of all JPEG Pleno reference software must to be compliant with ITU/ISO/IEC. So, when adding a file, the copyright disclaimer, as listed below, shall be included at the beginning of the file.

```
/* The copyright in this software is being made available under the BSD
 * License, included below. This software may be subject to other third party
 * and contributor rights, including patent rights, and no such rights are
 * granted under this license.
 *
 * Copyright (c) 2010-2019, ITU/ISO/IEC
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  * Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *  * Neither the name of the ITU/ISO/IEC nor the names of its contributors may
 *    be used to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */
```

## C++ Version

Use C++17. Code should avoid features that have been removed from the latest language version (currently C++17), as well as the rare cases where code has a different meaning in that latest version.

Use of some C++ features is restricted or disallowed. Do not use *non-standard extensions*.


## Naming

Classes, methods, functions and variables must be given descriptive and meaningful names describing their functionality and purpose.

Abbreviations should be avoided.

### File Names

Filenames should be all lowercase and can include underscores (_) or dashes (-).

C++ files should end in `.cpp` and header files should end in `.h`.

### Type Names

Derivative types (i.e., classes, structs, enums, unions, typedefs, and type template parameters) shall be named using nouns and have an initial capital letter and have a capital letter for each new word with no underscores (**PascalCase**): `ExampleClass`, `AbstractEncoder`.

Consecutive upper-letters are allowed for acronyms and obvious abbreviations: `CODECInterface`, `DCTBlock`, `IOManager`.

### Variable Names

Variable names (including class and struct data members) must have all **lower-case** letters with underscores between words.

```
DCTBlock next_block; // GOOD - uses underscore
DCTBlock nextblock; // BAD - concatenated words
DCTBlock nextBlock; // BAD - avoid camelCase
```

Do not use type prefixes (like `iVar`, `uiVar`, etc). These tend to be confusing whether the data type changes and the variable is not renamed.


### Constant Names

Constants are declared as `constexpr` or `const` variables and must have all **UPPER_CASE** letters with underscores between words.


### Enumerator Names

Enumerators should be named like constants.

Enumeration name should be named like classes.

```
enum AvailableColorSpaces {
  RGB = 0,
  YCOCG = 1,
  YUV = 2,
};
```

### Functions and Methods Names

Methods and functions shall be named using verbs describing the activity and have an initial lower-case letter and **camelCase** shall be used to connect multiple words, where each subsequent word in a name starts with an upper-case letter.

Ordinarily, functions should not start with a capital letter except for constructors.

```
void doSomething(); // GOOD - uses camelCase
void do_something(); // BAD - it is not a variable
void DoSomething(); // BAD - only constructor starts with UPPER CASE
DCTBlock(); // GOOD - only constructor starts with UPPER CASE
```

### Namespace Names

Namespace names are all lower-case.


## Header Files

In general, .cpp file should have an associated .h file. There are some exceptions, such as unittests and small .cpp files containing a `main()` function.

### Pre-processor Macros

Do not use `#define` for function definitions. Use `static inline` functions instead.

Avoid using `#define` to define constants. Use `constexpr` instead.

Do not use `#define` to to enable or disable a tool or code snipet. Use a config file associated with polymorphism instead.

All header files should have `#define` guards to prevent multiple inclusion. To guarantee uniqueness, the format of the symbol name should be &lt;PROJECT&gt;\_&lt;PATH&gt;\_&lt;FILE&gt;\_H\_. For instance, the file at `jplm/source/Lib/Utils/PixelMapFileIO.h` should define:

```
#ifndef JPLM_LIB_UTILS_PIXELMAPFILEIO_H
#define JPLM_LIB_UTILS_PIXELMAPFILEIO_H

...

#endif  // JPLM_LIB_UTILS_PIXELMAPFILEIO_H
```

## Functions and Methods

### Write Short Functions

Functions should be kept to a reasonable size. If a function exceeds 40 lines of code, consider splitting it into multiple smaller functions.

>> "The first rule of functions is that they should be small. The second rule of functions is that they should be smaller than that. This implies that the blocks within if statements, else statements, while statements, and so on should be one line long. Probably that line should be a function call. Not only does this keep the enclosing function small, but it also adds documentary value because the function called within the block can have a nicely descriptive name." (Clean Code, Robert C Martin)

### Function Arguments

A function should not have more than 3 arguments. When a function seems to need more than three arguments, probably some of those arguments should be wrapped into a class of their own. For instance:

```
void wrappPixel(uint32_t input_s,
    uint32_t input_t,
    uint32_t input_v,
    uint32_t input_u,
    uint16_t original_pixel_value,
    uint32_t& output_s,
    uint32_t& output_t,
    uint32_t& output_v,
    uint32_t& output_u,
    uint16_t& wrapped_pixel_value); // BAD CASE - function declaration

void wrappPixel(input_s, input_t, input_u,
                original_pixel_value,
                output_s, output_t, output_u, output_v,
                wrapped_pixel_value&); // BAD CASE - function call


Point4D wrappPoint(Point4D reference_point); // GOOD CASE - function declaration

Point4D wrapped = wrappPoint(reference_point); // GOOD CASE - function call
```

### Reference Arguments

In order to avoid dangerous side-effects, all parameters passed by lvalue reference must be labeled `const`.

```
void foo(const string &in); // GOOD
void foo(string &in); // BAD
```

### Return Arguments

A function should return something. Avoid use an input parameter as return.

```
// BAD:
void badSumTwoNumbers(int input1, int input2, int* output) {
  *output = input1 + input2;
}

int output;
badSumTwoNumbers(1, 2, &output);


// GOOD:
int goodSumTwoNumbers(int input1, int input2) {
  return input1 + input2;
}

int output = goodSumTwoNumbers(1, 2);
```


## Third-Part Libraries

The JPEG Pleno reference code uses third part libraries present at `jplm/sources/Lib/ThirdPart`.

Third-part libraries must be header-only. Prefer single-file header-only libraries. Use multiple files for a single third-part library is not allowed.

Third-part libraries must be licensed with **permissive software licenses** (i.e., BSD-like). **Protective licenses are strictly prohibited**.


## Templates

Avoid complicated template programming.

Code that uses templates is often unreadable, undebuggable and hard to maintain.

If you are using templating, you should expect to put considerable effort into minimizing and isolating its complexity.

The best approach is to use templating for being used in the scope of package.


## Unused Code

Unused code will be completely removed from the source code.

Commenting out for keeping old version is not allowed.

Using `#defines` that evaluate to false for keeping old versions ir not allowed.

Revisions and versioning should be keep in the revision control system.


## Code Duplication

Avoid duplication of code. Check if some functionality exists before implement it. If there is a similar functionality, please extend or modify it in order to make it more general.

## Optimization

Make the code readable and easy to maintain. Do not optimize it if the readability would be impaired.

## Data types

Avoid aliases for data types.

Use `<cstdint>` types (e.g. int16_t, int32_t, etc) for all data variables (i.e., variables thar carry encoding information).

Primitive integer types (e.g. `unsigned char`, `int`, `signed short`, etc) should be used only for counters and auxiliary variables that do not store encoding information.

Avoid using unsigned types. Use signed types only if you have a good reason to it.

Avoid using the `NULL` macro. Use `nullptr` instead.

## Type casting

Use C++-style casting (`static_cast`, `reinterpret_cast`, `const_cast`, `dynamic_cast`).

Avoid C-style casting.


```
int* p = (int*) malloc(10); // BAD - C-Style casting
int* p = static_cast<int*>(malloc(10)); // GOOD - C++-Style casting, but you should never use malloc. Always use new!
```

## Use of `sizeof`

Use `sizeof(varname)` instead of `sizeof(type)`.

## Use of `auto`

Use the `auto` keywords only in contexts where the reader can easily understand the type of the variable.

```
auto x = 4; // GOOD -  x is integer
auto y = 3.37; // GOOD - y is double
auto foo = x.add_foo(); // BAD - what is the type of foo?
auto value = y.Find(key); // BAD - what is the type of value?
```



## Comments

Comments are absolutely vital to keeping our code readable. However, while comments are very important, the best code is self-documenting. Even better than comments are tests.

Use either the `//` or `/* */` syntax, as long as you are consistent.

### Doxygen comments

After the ITU/ISO/IEC license disclaimer, it expected a comment doc containing some documentation (at least creation date in ISO 8601 format, authors, and a brief description). For instance, a file containing a class definition should starts with:


```
/* The copyright in this software is being made available under the BSD
 * License, included below. This software may be subject to other third party
 * and contributor rights, including patent rights, and no such rights are
 * granted under this license.
 *
 * Copyright (c) 2010-2019, ITU/ISO/IEC
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  * Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *  * Neither the name of the ITU/ISO/IEC nor the names of its contributors may
 *    be used to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

/** \file     JPLEncoder.h
 *  \brief    Implementation of JPEG Pleno encoder.
 *  \details  This class is used to encapsulate and manage all boxes of JPEG Pleno standard.
 *  \author   Pedro Garcia <pedro.gf@samsung.com>
 *  \author   Ismael Seidel <i.seidel@samsung.com>
 *  \date     2019-05-09
 */

#include "CoderSetup.hh"

#ifndef JPLM_APP_ENCODER_JPLENCODER_H
#define JPLM_APP_ENCODER_JPLENCODER_H

class JPLEncoder {
 private:
  CoderSetup setup;
 public:
  JPLEncoder(CoderSetup);
  virtual ~JPLEncoder();
  void run(void);
};

#endif // JPLM_APP_ENCODER_JPLENCODER_H
```

### TODO comments

Use TODO comments for code that is temporary, a short-term solution, or good-enough but not perfect.


## Classes

### Structs vs. Classes

Avoid structs. Prefer classes.

### Access Control

Make member variables and methods as private as possible.

Only methods that have to be called from outside should be public.

Only `static const` (or `constexpr`) data members are allowed to be public.

Global variables should not be used without good reason.

### Declaration Order

The declaration order should be: `public`, `protected`, and `private`.


## Formatting

### Line Length

Each line of text in your code should be at most 80 characters long.

### Identation

Use only spaces, and indent 2 spaces at a time. Do not use tabs in your code.

### Function Declarations and Methods Definitions

Return type on the same line as function name, parameters on the same line if they fit.

If the function declaration exceeds the line length, the parameter definition can be aligned with the previous line or indented with 4 spaces.


```
// GOOD - param3 declaration is aligned with param1 declaration
ReturnType ClassName::ReallyLongFunctionName(Type param1, Type param2,
                                             Type param3) {
  doSomething();
}
```

```
// GOOD - parameters are indented with 4 space indent
ReturnType ClassName::ReallyLongFunctionName(
    Type param1,
    Type param2,
    Type param3) {
  doSomething(); // 2d space indent
}
```

### Function Calls

Do not add spaces after the open paren or before the close parent.

If the arguments do not all fit on one line, they should be broken up onto multiple lines, with each subsequent line aligned with the first argument. Arguments may optionally all be placed on subsequent lines with a four space indent.


```
ReturnType variable = ReallyLongFunctionName(param1, param2,
                                             param3); // GOOD - aligned

ReturnType variable = ReallyLongFunctionName(
    param1,
    param2,
    param3); // GOOD - 4 space indent
```


### Usage and Placement of Braces on Conditionals

Avoid spaces inside parenthesis.

```
if ( block.isFilled() ) // BAD - spaces inside parentheses
  doSomething();

if (block.isFilled()) // GOOD - no spaces inside parentheses
  doSomething();
```

Use spaces between `if` and parenthesis.

```
if (block.isFilled()) // GOOD - spaces between if and parenthesis
  doSomething();

if(block.isFilled()) // BAD - space missing after if
  doSomething();

if(condition) {   // BAD - space missing after if

if (condition){   // BAD - space missing before {

if(condition){    // BAD - Doubly bad
```

Curly braces are not required for single-line statements, but they are allowed if you like them. However, be coherent, if one part of an if-else statement uses curly braces, the other part must too.

```
// BAD - curly on IF but not ELSE
if (condition) {
  foo();
} else
  bar();

// BAD - curly on ELSE but not IF
if (condition)
  foo();
else {
  bar();
}

// GOOD - Curly braces around both IF and ELSE
if (condition) {
  foo();
} else {
  bar();
}

// PRETTY GOOD - No unecessary curly braces
if (condition)
  foo();
else
  bar();
```

### Switch Statements

In general, `switch` statements should be used only for selecting a functionality. Do not use `switch` `case` blocks to implement large blocks of code.

```
// BAD - too mutch logic inside case blocks
switch (x) {
  case 11:  // BAD - it is not necessary
  case 13:
    if (dont_be_picky) {
      ABSL_FALLTHROUGH_INTENDED;
    } else {
      anotherConditionalFunction();
      break;
    }
  case 42:
    while (special_condition_counter > 0) {
        doSomethingSpecial();
        special_condition_counter--;
     }
    ABSL_FALLTHROUGH_INTENDED;
  default:
    DoSomethingGeneric();
    break;
}
```

```
// GOOD - switch-case used only for select the right functionality
switch (x) {
  case 13:
    processAnotherConditionalFunction();
    break;
  case 42:
    doManySpecialThings();
    break;
  default:
    DoSomethingGeneric();
    break;
}
```

Replace switch with polymorphism. This approach adheres to the "Tell-Don’t-Ask" principle.

```
// BAD - smell it
class Car {
  double getSpeed() {
    switch (type) {
      case SEDAN:
        return getBaseSpeed();
      case SUV:
        return getBaseSpeed() - getLoadFactor() * numberOfSeats;
      case COUPE:
        return (isElectric) ? 0 : getBaseSpeed();
    }
    throw new RuntimeException("Should be unreachable");
  }
}
```

```
// GOOD - instead of asking an object about its state and then
// performing actions based on this it's much easier to simply
// tell the object what it needs to do and let it decide for
// itself how to do that.
class Car {
public:
  virtual double getSpeed() = 0;
};


class Sedan: public Car {
 public:
  double getSpeed() {
    return getBaseSpeed();
  }
}

class SUV: public Car {
 public:
  double getSpeed() {
    return getBaseSpeed() - getLoadFactor() * numberOfSeats;
  }
}

class Coupe: public Car {
 public:
  double getSpeed() {
    return (isElectric) ? 0 : getBaseSpeed();
  }
}

// Somewhere in client code
double speed = car.getSpeed()
```

### Loops Statements

Braces are optional for single-statement loops.

```
// GOOD - Braces are optional for single-statement loops.
for (int i = 0; i < total; ++i)
  printf("Hello world\n");

for (int i = 0; i < total; ++i) {
  printf("Hello world\n");
}
```

Empty loop bodies should use either empty braces or continue.

```
// GOOD
while (condition) {
  printf("Hello world\n");
}


for (int i = 0; i < total; ++i) {}  // Good - one newline is also OK.

while (condition) continue;  // Good - continue indicates no logic.

while (condition);  // Bad - looks like part of do/while loop.
```

### Pointer and Reference Expressions

No spaces around period or arrow.

Pointer operators do not have trailing spaces.

There are no spaces around the period or arrow when accessing a member.

Pointer operators have no space after the * or &.

When declaring a pointer variable or argument, you may place the asterisk adjacent to either the type or to the variable name:

It is allowed (but not recommended) to declare multiple variables in the same declaration, but it is disallowed if any of those have pointer or reference decorations. Such declarations are easily misread.


```
char *c; // GOOD - asterisk adjacent to the variable name
const string &str; // GOOD - & adjacent to the variable name

char* c; // GOOD - asterisk adjacent to the type
const string& str; // GOOD - & adjacent to the type

int x, *y;  // BAD - no & or * in multiple declaration
char * c;  // BAD - spaces on both sides of *
const string & str;  // BAD - spaces on both sides of &
```

### Boolean Expressions

When you have a boolean expression that is longer than 80 characters, the logical operator is always at the end of the lines when break up the lines.

```
if (this_one_thing > this_other_thing &&
    a_third_thing == a_fourth_thing &&
    yet_another_thing && last_one_thing) {
  ...
}
```

### Return Values

Do not needlessly surround the `return` expression with parentheses.

### Preprocessor Directives

The hash mark that starts a preprocessor directive should always be at the beginning of the line, even when preprocessor directives are within the body of indented code.

```
// BAD - indented directives
  if (condition) {
    #if DISASTER_PENDING  // BAD - The "#if" should be at beginning of line
    DropEverything();
    #endif                // BAD - Do not indent "#endif"
    BackToNormal();
  }

// GOOD - directives at beginning of line
  if (condition) {
#if DISASTER_PENDING      // BAD -- Starts at beginning of line
    DropEverything();
#endif
    BackToNormal();
  }
```

### Class Format

Sections in `public`, `protected` and `private` order, each indented one space. These keywords should be preceded by a blank line. Do not leave a blank line after these keywords. The public section should be first, followed by the protected and finally the private section.

```
class CustomClass : public AbstractBaseClass {
 public:      // Note the 1 space indent!
  CustomClass();  // Regular 2 space indent.
  ~MyClass() {}

 protected:
  void parentClass();

 private:
  bool someInternalFunction();
  int some_internal_var_;
  int some_other_var_;
};
```

### Constructor Initializer Lists

Constructor initializer lists can be all on one line or with subsequent lines indented four spaces.

```
CustomClass::CustomClass(int var)
    : some_var_(var),             // 4 space indent
      some_other_var_(var + 1) {
  DoSomething(); // 2 space indent
}
```

### Namespace Formatting

The contents of namespaces are not indented.

### Horinzontal Spacing

Never put trailing whitespace at the end of a line.

Use horizontal whitespace between reserved keywords (`if`, `switch`, `while`, `for`, etc) and parenthesis.

Use horizontal whitespace between parenthesis and braces.

Do not use space after `(` and before `)`.

Use horizontal whitespace on binary operators.

Do not use horizontal whitespace to separe unary operators and their arguments.

Never use spaces inside the angle brackets (`<` and `>`), before  `<`, or after `>`.

```
std::vector<string> x; // GOOD
std::vector< string > x; // BAD
std::vector <string> x; // BAD

y = static_cast<char*>(x); // GOOD
y = static_cast<char *>(x); // Spaces between type and pointer
y = static_cast <char*>(x); // BAD
```

### Vertical Spacing

Minimize use of vertical whitespace.

Separate functions and method declarations with 2 vertical spaces.

```
// BAD - only 1 vertical space between functions
void functionOne(Param p) {
  ...
}

void functionTwo(Param p) {
  ...
}
```


```
// GOOD - 2 vertical spaces between functions
void functionOne(Param p) {
  ...
}


void functionTwo(Param p) {
  ...
}
```

Do not use blank lines at the beginning or end of a function.

Do not use blank lines inside a if-else block.

A blank line before a comment line usually helps readability.

## Automated Formatting

As developers, we known that keeping the style conformance is often mistake-prone. To make the development easier, the code can be formatted using the *clang-format tool* and the configuration file attached to these guidelines (available at `doc/software-guidelines/jplm.clang-format`). Example:

```
$ clang-format -style=file -i doc/software-guidelines/jplm.clang-format source/App/Decoder/jpl-decoder.cpp
```

### Visual Studio Code

Install the C/C++ extension and customize settings.json to include:

```
"C_Cpp.clang_format_path": "/path/to/<clang-format-executable>",
"C_Cpp.clang_format_style": "file",
"C_Cpp.clang_format_fallbackStyle": "none"
```

### Emacs

Add the line `(load "/path/to/clang-format.el")` to `~/.emacs.d/init.el`. Format a source file with `M-x clang-format-region`.

### CLion

1. Go to **File->Settings->Tools->External Tools** and click on the plus sign. A window should pop up. Choose a name, for example "clang-format".
2. For the Tool settings tab, use this configuration:
    * Program: clang-format
    * Arguments: --style=file -i $Projectpath$/doc/software-guidelines/jplm.clang-format $FileName$
    * Working directory: $FileDir$

Now, with your file open, you can go to **Tools->External** tools and run the config above. It basically calls clang-format and does inplace formatting using the style define in the first .clang-format file found in a parent directory.

More information about IDE integration and *clang-format tool* usage is available at [https://clang.llvm.org/docs/ClangFormat.html](https://clang.llvm.org/docs/ClangFormat.html).