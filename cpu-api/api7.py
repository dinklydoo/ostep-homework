import os
from os import waitpid, close
from pty import STDOUT_FILENO

pid = os.fork()
if (pid == 0):
    close(STDOUT_FILENO)
    print("Not here") # error message, closed stdout
else:
    waitpid(0, 0)
    print("OK")