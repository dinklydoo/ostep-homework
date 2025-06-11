import os

pipefd = os.pipe() # pipefd = [read, write]
pid = os.fork()

if (pid == 0):
    os.close(pipefd[0]) # close read end of pipe

    buffer = "Wassup Beijing"
    os.write(pipefd[1], buffer.encode())
    os.close(pipefd[1])
    print("bing chilling")
else:
    os.close(pipefd[1])
    r = os.fdopen(pipefd[0], "rb")
    print(r.read().decode())
    os.waitpid(pid, 0)
