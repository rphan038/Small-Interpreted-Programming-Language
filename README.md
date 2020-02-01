# Small-Interpreted-Programming-Language
This project is a small interpreted programming language that takes input from a user in a specific format. The program saves
this input and the user is able to evaluate or add on to this expression using a series of commands. Expressions can be
appended to each other and variables can be defined in however the user desires. An example of a possible set of commands 
is demonstrated below where 'c' creates an expression, 'p' prints an expression, 'd' defines a variable, and 'e' evaluates
the expression:
c
(+ 2 4)
p
(+ 2 4)
d
x
4
d
y
6
d
z
(+ x y)
p
z
(+ x y)
e
z
10
d
b10
(+ 2 4 (- 10 7) (* 3 (+ 2 5))) 
p
b10
(+ 2 4 (- 10 7) (* 3 (+ 2 5)))

Project Title: Small interpreted programming language

Date: 12/8/19

Authors: Richard Phan
