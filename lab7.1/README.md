
Compile server first using the foloowing command in the terminal in your directory containing the files

		gcc server1.c -o server1.o

Then run it with an '&' as parameter

		./server1.o &

Then compile the client1 file

		gcc client1.c -o client1.o

Run it passing the proper parameters

		./client1.o <operation> [list of number separated by space]

Operations supported:

Additions: + [num1] [num2]
Substraction: - 'Remember doing - 3 4 will substract 3 and 4 from 0
Multiplication: x [num1] [num2]
