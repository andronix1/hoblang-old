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
C# API like example(`examples/cstest.hob`)
```
import "csharp.hob" as csharp;

use csharp::console;
use csharp::convert;

fun main(): void {
	var str = console::read_line();
	console::write("your number: \0");
	console::write_line(str);
	console::write_line("incrementing it...\0");
	var int = convert::to_int32(str);
	console::write_line(convert::int32_to_string(int + 1));
}
```
### To compile
```
cd examples # Must be run in examples folder, i'll fix this later
mkdir .build
../.build/hoblang compile cstest.hob .build/cstest.o
gcc .build/cstest.o -o .build/cstest
.build/cstest
```
