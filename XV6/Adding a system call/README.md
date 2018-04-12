**Adding a system call in XV6 OS**
===
In this I will show you how to add a system call to XV6 OS. We will just add a simple helloworld systemm call which will print hello world and the argument passed to the the system call.


**Steps:**
---
For adding the system call we need to make changes in the follwing files:
1. syscall.c
2. syscall.h
3. user.h
4. usys.S
5. sysproc.c

1. First we add the call to the list in **syscall.c**.

`extern int sys_hello(void);` 

`[SYS_hello]   sys_hello,`

![image](https://github.com/siddharthsingh/OS/blob/master/XV6/images/1.png)

![image](https://github.com/siddharthsingh/OS/blob/master/XV6/images/2.png)



2. Next, assign it a number in **syscall.h

`#define SYS_hello  22`

![image](https://github.com/siddharthsingh/OS/blob/master/XV6/images/3.png)


3. give it a prototype in **user.h**:

`int hello(int);`

![image](https://github.com/siddharthsingh/OS/blob/master/XV6/images/4.png)


4. Add it to **usys.S**, which generates the user-space assembly code for it

`SYSCALL(hello)`

![image](https://github.com/siddharthsingh/OS/blob/master/XV6/images/5.png)

5. Finally we add the implementation somewhere (e.g. **sysproc.c**)

```
int
sys_hello(void) {
    int n;
    if(argint(0, &n) < 0)
        return -1;
    cprintf("Hello world %d\n", n);
    return 0;
}

```

![image](https://github.com/siddharthsingh/OS/blob/master/XV6/images/6.png)





Testing
---

To test if the system call works, create a c file and use the system call in it. Remember to add the c file in Makefile so that you can use it.

![image](https://github.com/siddharthsingh/OS/blob/master/XV6/images/7.png)


![image](https://github.com/siddharthsingh/OS/blob/master/XV6/images/8.png)
