all:
	gcc *.c -o 'sudokuExec'

run:
	./sudokuExec

clean:
	rm sudokuExec