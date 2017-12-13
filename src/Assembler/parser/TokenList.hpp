#pragma once

/* TokenList example
#define LEX_TOKEN_LIST				\
    LEX_TOKEN (EXAMPLE_1, "ex1")		\
    // The lexer need theses following tokens !
    LEX_TOKEN (_EOF_, "<lex::eof>")		\
    LEX_TOKEN (OTHER, "<lex::other>")
*/

#define LEX_TOKEN_LIST				\
    LEX_TOKEN (CLS, "cls")			\
    LEX_TOKEN (RET, "ret")			\
    LEX_TOKEN (JP, "jp")			\
    LEX_TOKEN (CALL, "call")		\
    LEX_TOKEN (SE, "se")			\
    LEX_TOKEN (SNE, "sne")			\
	LEX_TOKEN (LD, "ld")			\
	LEX_TOKEN (ADD_VxByte, "add")	\
	LEX_TOKEN (OR, "or")			\
	LEX_TOKEN (AND, "and")			\
	LEX_TOKEN (XOR, "xor")			\
	LEX_TOKEN (SUB, "sub")			\
	LEX_TOKEN (SHR, "shr")			\
	LEX_TOKEN (SUBN, "subn")		\
	LEX_TOKEN (SHL, "shl")			\
	LEX_TOKEN (RND, "rnd")			\
	LEX_TOKEN (DRW, "drw")			\
	LEX_TOKEN (SKP, "skp")			\
	LEX_TOKEN (SKNP, "sknp")		\
	LEX_TOKEN (SYS, "sys")			\
	LEX_TOKEN (DOWN, "sub")			\
	LEX_TOKEN (RIGHT, "right")		\
	LEX_TOKEN (LEFT, "left")		\
	LEX_TOKEN (LOW, "low")			\
	LEX_TOKEN (HIGH, "high")		\
	LEX_TOKEN (EXIT, "exit")		\
    LEX_TOKEN (_EOF_, "<lex::eof>")	\
    LEX_TOKEN (OTHER, "<lex::other>")
