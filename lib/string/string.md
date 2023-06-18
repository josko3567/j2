# `j2.string.[...]`

`j2string` is the string type of the `j2` enviorment. `j2.string.[...]` is the space in which functions that operate on `j2string's` are stored. 

Currently it is not faster than the standard `<string.h>` library but future releases will attempt to speed it up to its standard.

`j2string` is my attempt at making a *dynamic and static safe string library*. with *fancy syntax*.

## Logic
---
The logic of how strings are stored in `j2string` is very
similar to [sds](https://github.com/antirez/sds).

To summerize how *sds* stores strings, unlike most other string libraries that store data in 2 blocks inside memory :          
 * A `struct` that stores our *string info*,       
 * A `char* `that stores the *string contents*.

*sds* instead stores everything in one block of memory like this :                                              
                                                        
```
+--------+----------------------+-----+       
| struct | char*                | nil | 
+--------+----------------------+-----+      
         ^                                          
     sds' Pointer   

```
Due to the pointer being at the start of the *string contents* we can pass the pointer to any *standard C library*, `j2string` works in on the same principle so the following code is possible with `j2string` *strings*.
```C
j2string s = j2.string.new.st("Hi!");
printf("%s", s);
```
## Initalizing a string
---
To initalize a `j2string` we use the following functions:
* **`j2.string.init()`** initilazes a `j2string` as a *static or dynamic string*.
    - [Param.] `c_string` accepts a *`char*` string* that is going to be the inital value for the new *`j2string` strings content*.
    - [Param.] `parameters` accepts a *`j2string_param` structure:* 
        + `parameters.allocate` to set the inital size of the string,
        if set to `0` or less `parameters.allocate` is set to the length of `c_string`.
        + `parameters.max` sets the maximum size the string is allowed to grow to, if set to `0` or less `parameters.max` is set to `J2_STRING_MAX_SIZE` ~ 1MiB.
        + `parameters.padding` sets the step a dynamic string takes to fit new input inside of it, values are clamped to be never be less than `0`. `parameters.padding` determines whether a string is *static* or *dynamic* by the following ruleset:
            - If `parameters.padding` is equal to 0, the `j2string` is initalized as a **_static string_**.
            - If `parameters.padding` is bigger than 0, the `j2string` is initalized as a **_dynamic string_**.

```C
// Initalize a dynamic string with 6 bytes initaly allocated, 
// that can grow to a size of 25 bytes and grows in steps of 1.
j2string s = j2.string.init("Hello world!", (j2string_param){
        .allocate = 6,
        .max      = 25,
        .padding  = 1
});
```
* **`j2.string.new.st()`** initalizes a `j2string` as a *static string*. It is a call to `j2.string.init` with the param. `parameters` filled to create a generic static string that can fit the string `c_string` inside of itself.
    - [Param.] `c_string` accepts a *`char*` string* that is going to be the inital value for the new *`j2string` strings content*.

```C
// Initalize a generic static string.
j2string s = j2.string.new.st("Hello world!");
```

* **`j2.string.new.dy()`** initalizes a `j2string` as a *dynamic string*. It is a call to `j2.string.init` with the param. `parameters` filled to create a generic dynamic string that can fit the string `c_string` inside of itself (`parameters.padding` is set to 1).
    - [Param.] `c_string` accepts a *`char*` string* that is going to be the inital value for the new *`j2string` strings content*.

```C
// Initalize a generic dynamic string.
j2string s = j2.string.new.dy("Hello world!");
```

## Standard string operations
---
**`j2.string.ccpy()`**