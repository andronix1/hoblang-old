# Hob-lang
## Getting Started
Requires GNU `gcc`, `cmake` and `llvm`
``` bash
mkdir .build
cd .build
cmake ../
cmake --build ./
./hoblang compile <source> <output>
gcc <output> -o <binary-output>
./<binary-output>
```
## Syntax
C#-like API example(`examples/cstest.hob`)
```
import "cs-api/lib.hob" as cs_api;
import "libc/lib.hob" as libc;

use cs_api::console;
use cs_api::convert;
use libc::mem as cmem;

type int = libc::int32_t;

type Inner = struct {
	integer: i32
};

type Test = struct {
	inner: Inner
};

fun main(): void {
	var test: Test;	
	defer console::write_line("finished!\0");
	var str = console::read_line();
	defer cmem::free(str as *void);
	console::write("your number: \0");
	console::write_line(str);
	console::write_line("incrementing it...\0");
	test.inner.integer = convert::to_int32(str);
	if test.inner.integer == 68 {
		console::write_line("I am not ChatGPT, but...\0");
		return;
	}
	var istr = convert::int32_to_string(test.inner.integer + 1);
	defer cmem::free(istr as *void);
	console::write_line(istr);
}
```
### To compile
```
cd examples
mkdir .build
hoblang compile examples/cstest.hob .build/cstest.o
gcc .build/cstest.o -o .build/cstest
.build/cstest
```
