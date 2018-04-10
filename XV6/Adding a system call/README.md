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

First we add the call to the list in **syscall.c**.
<!-- ToDo: Add pic -->

Next, assign it a number in **syscall.h**

<!-- ToDo: Add pic -->


give it a prototype in **user.h**:


Add it to **usys.S**, which generates the user-space assembly code for it


Finally we add the implementation somewhere (e.g. **sysproc.c**)

