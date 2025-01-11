setlocal tabstop=4
setlocal softtabstop=4
setlocal shiftwidth=4

syntax keyword hobBoolean
	\ true
	\ false

syntax keyword hobKeywords 
	\ var
	\ fun
	\ if
	\ else
	\ return
	\ extern
	\ module
	\ as
	\ while

syntax keyword hobType
	\ i8
	\ u8
	\ i16
	\ u16
	\ i32
	\ u32
	\ i64
	\ u64
	\ bool
	\ void

syntax match hobNumber "\v<\d+>"
syntax region hobChar start=/'/ skip=/\\'/ end=/'/
syntax region hobComment start='#' end='\n'

highlight default link hobKeywords Keyword
highlight default link hobType Type
highlight default link hobComment Comment
highlight default link hobNumber Number
highlight default link hobBoolean Boolean
highlight default link hobChar String

