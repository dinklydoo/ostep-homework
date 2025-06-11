import os

file = open("buffer.txt", 'r+')
file.truncate(0)
pid = os.fork()

if (pid > 0):
    while True:
        file.seek(0)
        if (file.readline() != ''): break
     # infinite loop on file (assuming concurrency or some form of process handover to child)

    file.truncate(0) # clear the buffer file
    print("GoodBye")
else:
    file.write("Finished!")
    file.flush()
    print("Hello")