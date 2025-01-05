# Hob-lang
## Why?
Main idea hidden in the name of this lang. It is HOBBY language. It means that it creates only in recreaional purposes. I know what code is so bad, i want to refactor it in near future
## Getting Started
Requires GNU `gcc`, `cmake` and `llvm`
``` bash
mkdir .build
cd .build
cmake ../
cmake --build ./
./hoblang compile <source> <output>
```
## Syntax
```
extern fun say(success: bool): void

fun works(): void { say(true); }
fun so_bad(): void { say(false); }

fun sum(a: i64, b: i64): i64 { return a + b; }

fun main(): void {
	var a = 3;
	var b = 4;
	if sum(a, b) == a + b {
		works();
	} else {
		so_bad();
	}
}
```
