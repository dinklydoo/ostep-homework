import os
from os import WNOHANG, WUNTRACED

file = open("buffer.txt", 'r+')
file.truncate(0)

pid = os.fork()
if pid == 0:
    file.write("This should not appear in my buffer unless if i flush")
    #os.fsync() #flush before exec

    os.execlp("ls", "ls")
    print("This should never print")
    # never executes, process is replaced by exec completely
else:
    #os.wait()
    os.waitpid(0, 0)
# OPTIONS :
    # 0: default behaviour
    # WNOHANG: return if no child exited
    # WUNTRACED: return if a child has stopped
    # WCONTINUED: return if stopped child has resumed

    print("HEE HEE HA HA")