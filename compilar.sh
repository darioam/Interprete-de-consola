rm src/lex.yy.c
rm include/mi_parser.h
rm src/mi_parser.c



flex -o src/lex.yy.c lexer.l
bison -d -Wcounterexamples --header=include/parser.h -o src/parser.c parser.y

make