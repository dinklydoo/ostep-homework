#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <unistd.h>

void wait_or_die() {
    int rc = wait(NULL);
    assert(rc > 0);
}

int fork_or_die() {
    int rc = fork();
    assert(rc >= 0);
    return rc;
}

#define Time_GetSeconds() ({ struct timeval t; int rc = gettimeofday(&t, NULL); assert(rc == 0); (double) t.tv_sec + (double) t.tv_usec/1e6; })

double t_start;

struct pid_map {
    int pid;
    char name[10];
    struct pid_map *next;
};

struct pid_map *head = NULL;

void Space(char c) {
    int i;
    for (i = 0; i < 5 * (c - 'a'); i++) {
        printf(" "); 
    }
}

char *Lookup(int pid) {
    struct pid_map *curr = head;
    while (curr) {
        if (curr->pid == pid) 
	           return(curr->name);
	       curr = curr->next;
    }
    return NULL;
}

void Record(int pid, char *m) {
    struct pid_map *n = malloc(sizeof(struct pid_map));
    assert(n);
    n->pid = pid;
    strcpy(n->name, m);
    n->next = head;
    head = n;
}

void Wait(char *m) {
    int rc = wait(NULL);
    assert(rc > 0);
    double t = Time_GetSeconds() - t_start;
    printf("%3d ", (int)t);
    Space(m[0]);
    char *n = Lookup(rc);
    assert(n != NULL);
    printf("%s<-%s\n", m, n);
}

void Sleep(int s, char *m) {
    sleep(s);
    double t = Time_GetSeconds() - t_start;
    printf("%3d %s\n", (int)t, m);
}

void Fork(char *p, char *c) {
    double t = Time_GetSeconds() - t_start;
    printf("%3d ", (int)t);
    Space(p[0]);
    printf("%s->%s\n", p, c);
}

void __Begin(char *m) {
    double t = Time_GetSeconds() - t_start;
    printf("%3d ", (int)t);
    Space(m[0]);
    printf("%s+\n", m);
}

void __End(char *m) {
    double t = Time_GetSeconds() - t_start;
    printf("%3d ", (int)t);
    Space(m[0]);
    printf("%s-\n", m);
}

#define Begin(m) { __Begin(m); }
#define End(m)   { __End(m); exit(0); }

int main(int argc, char *argv[]) {
    int rc;
    t_start = Time_GetSeconds();
    Begin("a");
    Fork("a", "b");
    if ((rc = fork_or_die()) == 0) {
        sleep(2);
        Begin("b");
        Fork("b", "c");
        if ((rc = fork_or_die()) == 0) {
            sleep(3);
            Begin("c");
            End("c");
        }
        Record(rc, "c");
        Wait("b");
        Fork("b", "d");
        if ((rc = fork_or_die()) == 0) {
            sleep(8);
            Begin("d");
            Fork("d", "e");
            if ((rc = fork_or_die()) == 0) {
                sleep(4);
                Begin("e");
                Fork("e", "f");
                if ((rc = fork_or_die()) == 0) {
                    sleep(10);
                    Begin("f");
                    Fork("f", "g");
                    if ((rc = fork_or_die()) == 0) {
                        sleep(5);
                        Begin("g");
                        End("g");
                    }
                    Record(rc, "g");
                    Wait("f");
                    End("f");
                }
                Record(rc, "f");
                Wait("e");
                End("e");
            }
            Record(rc, "e");
            Wait("d");
            End("d");
        }
        Record(rc, "d");
        Wait("b");
        End("b");
    }
    Record(rc, "b");
    Wait("a");
    return 0;
}

