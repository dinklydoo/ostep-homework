import os
#Exercise 1-2
x = 100

file = open("buffer.txt", 'r+')
file.truncate(0)
pid = os.fork()

if (pid > 0):
    file.write("this is the parent writing\n")

    print("I am the parent\n", os.getpid())
    print("Parent x:", x-10)
else:
    file.write("this is the child writing\n")

    print("I am the child\n", os.getpid())
    print("Child x:", x-20)



