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
Multithreaded HTTP example(`examples/http_nolibc.hob`)
```
import "std/process.hob" as process;
import "std/tcp.hob" as tcp;
import "std/io.hob" as io;
import "std/fs.hob" as fs;

const PORT: u16 = 8080;
const BACKLOG: i32 = 16;

const BUFFER_SIZE: i32 = 10;

fun handle_conn(client: tcp::Socket) -> void {
	var filename = "test.html\0";
	var file = fs::open_read(filename);
	if file < 0 {
		io::println("cannot open file `test.hml`");
		return;
	}

	var buf: [BUFFER_SIZE]u8;
	var buf_slice = (&buf) as []u8;
	while io::read_from(client, &buf_slice) as i32 == BUFFER_SIZE {
		# io::print(buf_slice);
	}
	io::write_to(client, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n");
	io::full_forward(file, client);
	tcp::close(client);
	io::println("wow, closed the connection!");
}

fun main() -> i32 {
	var server = tcp::bind(PORT, BACKLOG);
	if server < 0 {
		io::println("cannot bind a socket :(");
		return 1;
	}
	# unnecessary but.. why not?)
	var handle_func: fun (tcp::Socket) -> void = handle_conn;
	while true {
		var client = tcp::accept(server);
		if client < 0 {
			io::println("cannot accept a socket :(");
		} else {
			if process::fork() != 0 {
				handle_func(client);
			}
		}
	}
	return 0;
}

fun _start() -> void {
	process::exit(main());
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
