Implementing lottery scheduling in XV6
===

About
---
In this blog, I will show you how to implement lottery scheduling for XV6 operating system. XV6 by default uses Round Robin algorithm. Please note that in this implementation, the process sets its own tickets which is not ideal as it may take a huge number and the other processes would have very less chance to run.

What is XV6?
---
xv6 is a re-implementation of Dennis Ritchie's and Ken Thompson's Unix Version 6 (v6).  xv6 loosely follows the structure and style of v6, but is implemented for a modern x86-based multiprocessor using ANSI C.


What is lottery scheduling?
---
Lottery scheduling is a probabilistic scheduling algorithm for processes in an operating system. Processes are each assigned some number of lottery tickets, and the scheduler draws a random ticket to select the next process. The distribution of tickets need not be uniform; granting a process more tickets provides it a relatively higher chance of selection. This technique can be used to approximate other scheduling algorithms, such as Shortest job next and Fair-share scheduling.

Lottery scheduling solves the problem of starvation. Giving each process at least one lottery ticket guarantees that it has a non-zero probability of being selected at each scheduling operation.


How lottery scheduling works?
---
We have two processes A and B. A has 60 tickets (ticket number 1 to 60) and B have 40 tickets (ticket no. 61 to 100).
Scheduler picks a random number from 1 to 100. If the picked no. is from 1 to 60 then A is executed otherwise B is executed. (source)

Steps
---
We need to do the following things to implement lottery scheduling in XV6

Make a system call which allows you to set the tickets for a process.

Code to generate a random number.

In the scheduler function count the total number of tickets for all processes that are runnable.

Generate a random number between 0 and the total tickets calculated above.

When looping through the processes keep the counter of the total number of tickets passed.

Just when the counter becomes greater the random value we got, run the process.

Put a break at the end of for loop so that we don't execute the processes following the process we just run.


Making a system call
---
Each process has a ticket value assigned to it. So to implement this we need to change the proc structure which holds the information about a process and add int tickets which will store its ticket value. 

Follow this blog to know how to add system call. In the part where you have to write the actual implementation use the following code which sets the number of tickets for a process.
```
int sys_settickets(void)

{

   int n;

  if(argint(0, &n) < 0)

    return -1;

  proc->tickets = n;

  return n;

}
```


Allocate default value of tickets when a process is created

In the proc.c file, enter the following line of code in "allocproc" function just below "found".
```
 p->tickets = 10;  
```

This sets the default ticket value to 10.


Creating Random number
---
Include rand.o\ in OBJS in Makefile

Copy files rand.c and rand.h in the XV6 directory.

After doing these two steps you can include rand.h header file in your proc.c file which is responsible for scheduling all processes.



Scheduler Function
---
The scheduler function has two for loops, one inside the other. The outer for loop just keeps on running forever. The inside for loop loops over all the processes and selects the first process that it finds in a RUNNABLE state. It then runs this process till its time quanta expire or the process yields voluntarily. It then selects the next process in RUNNABLE state and so on. This is the way it implements Round Robin.

For lottery scheduling, we need the total number of tickets of the processes that are in the RUNNABLE state. To calculate this we put a for loop inside the outer for loop just before the inside for loop executes. This calculates the total number of tickets.

Now we generate a random number between 0 and the total number of tickets. After we have the random number we execute the for loop that runs processes. When this for loop loops over processes, we keep counting the total number of tickets passed of processed that are in a RUNNABLE state. As soon as the total tickets passed get higher than the random number we got we run that process. 

Now, after the process runs, we need to put a break at the end of for loop which executes all processes. This is because:

If we don't break the value of total tickets passed will keep on increasing and will always be higher than the random number.

Also, after we run a winning process we need to recompute the total number of tickets of all RUNNABLE processes as that value might have changed.
























