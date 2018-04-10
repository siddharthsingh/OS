/*
Tail implementation for XV6
created by Siddharth Singh (01siddharthsingh@gmail.com)

The reason why tail implementation is lot more difficult than head is because
  1. We can't keep the whole file in memory as the file may be very large. We need to keep a array of pointers, each pointing to a 
    //char array containing a line.
  2. We don't know what the size of each line is so we can't allocate memory before we see the line.
    //the line can be 1000s of character long so wee need to keep dynmically allocating more memory to store the line.


*/


#include "types.h"
#include "user.h"
char buf[10];
//default number of lines to print
int numl = 10;
int debug = 0;
#define NULL 0

/* Copy data from souce to destination memory  */
void memcpy(void *dest, void *src, uint n)
{
   // Typecast src and dest addresses to (char *)
   char *csource = (char *)src;
   char *cdestination = (char *)dest;

   // Copy contents of src[] to dest[]
   for (int i=0; i<n; i++)
       cdestination[i] = csource[i];
}

/* Reallocate the memory */
void *realloc(void *ptr, uint originalLength, uint newLength)
{

  if(debug == 1){
    printf(1,"original length = %d , new length = %d " , originalLength , newLength);
  }
   if (newLength == 0)
   {
      free(ptr);
      if(debug == 1){
      printf(1,"lenght of new memmory allocation is 0");
    }
      return NULL;
   }
   else if (!ptr)
   {
     if(debug == 1){
     printf(1, "original pointer passed was null");
   }
      return malloc(newLength);
   }
   else if (newLength <= originalLength)
   {
     if(debug == 1){
     printf(1, "Realloc: New memory cannot be less than original memory");
   }
      return ptr;
   }
   else
   {
      // assert((ptr) && (newLength > originalLength));
      void *ptrNew = malloc(newLength);
      if (ptrNew)
      {
          memcpy(ptrNew, ptr, originalLength);
          if(debug == 1){
            printf(1,"allocated");
          }
	  /* Free old memory */
          free(ptr);
      }
      return ptrNew;
    }
}

void tail(int fd)
{

  int n,i,l=0;
  /* initial length of the memory */
  int lenght = 100;

  /*index at which to store current line */
  int index_to_store = 0;
  /* index into the array from where we should start printing */
  int start_to_print = 0;
  //this is to check if there is any line in the input when we press control + d to quit tail.
  int temp_data = 0;
  //used to generate pointers to the last numl lines
  int pointer_generator_counter = 0;
  //array of pointers to the last numl lines
  char *pointer_to_last_n_lines[numl];
  //length of each line. This will grow as we add more data to the line
  int length_each_line[numl];
  //tells us till what index have we filled the line
  int count_each_line[numl];

  char *pointer_to_last_n_lines2[numl];
  //generating an array of pointers which will point to the last numl lines.
  for(;pointer_generator_counter<numl; pointer_generator_counter++){
    if(debug == 1){
      printf(1, "%d creating pointers" , pointer_generator_counter);
    }
    //allocate memory
    pointer_to_last_n_lines[pointer_generator_counter] = malloc( sizeof(char) * ( lenght ) );
    //just copying the pointer
    pointer_to_last_n_lines2[pointer_generator_counter]  = pointer_to_last_n_lines[pointer_generator_counter];
    //initial length of line
    length_each_line[pointer_generator_counter] = 100;
    //where to insert data into line
    count_each_line[pointer_generator_counter] = 0;

  }

  //reading from file discriptor
  while((n = read(fd, buf, sizeof(buf))) > 0)
   {
     // printf(1,"%d \n" , fd);
     temp_data = 1;
     for(i=0;i<n;i++)
     {
       
      //check if we are near the end of the memory allocated to the line. If we are we reallocate more memory and 
      //copy the old memory to new memeory and free old memory
       if(count_each_line[index_to_store] == length_each_line[index_to_store] -3){
         if(debug == 1){
         printf(1," realloc called\n" );
        }
         //expand the array size by relloc and update the length
        //reallocating twice the memory as it was before
        char *temp = realloc(pointer_to_last_n_lines2[index_to_store] ,  length_each_line[index_to_store] , sizeof(char)*( length_each_line[index_to_store]*2));
        pointer_to_last_n_lines2[index_to_store] = temp;
        //increasing the size of line 
        length_each_line[index_to_store] *= 2;
        if(debug == 1){
          printf(1," realloc called 2\n" );
        }

         pointer_to_last_n_lines2[index_to_store][count_each_line[index_to_store]] = buf[i];
         if(debug == 1){
         printf(1,"realloc called 3\n" );
       }
       }else{
      //we are not near the end of the allocated memory so we keep on adding characters to the memory
       pointer_to_last_n_lines2[index_to_store][count_each_line[index_to_store]] = buf[i];

       }
       // printf(1,"this works");

      count_each_line[index_to_store]++;
      if(debug == 1){
      printf(1,"this works count is %d %c \n" , count_each_line[index_to_store] , buf[i]);
    }

    //new line character
      if(buf[i] == '\n'){
          temp_data = 0;
          //put a null at the end of the current line so that we can print it.
          pointer_to_last_n_lines2[index_to_store][count_each_line[index_to_store]] = '\0';
          //increment the index to where we store the line. Note that this moves in a circular way
          //so if we have to print last 10 lines after writing to the 9th array index it will write at the 0th position
          index_to_store = (index_to_store + 1)%numl;
          //as it is storing in a circular way we need to know from were we should start printing. 
          //this keeps track of it
          start_to_print = (index_to_store)%numl;
          count_each_line[index_to_store] = 0;


        l++;
        }


      }
    }
    // printf(1, "%d" , n);
    if(temp_data == 1){
      //if there was data in the last line and it didn't end with a new line character we increment the start_to_print
      //as you can see from the code above  start_to_print only increments when we encounter a new line character
      start_to_print = (++index_to_store)%numl;
    }
 
  //using this variable to loop over all the lines
  pointer_generator_counter = 0;
  //printing the last numl lines
  for(;pointer_generator_counter<numl; pointer_generator_counter++){
    printf(1, "%s", pointer_to_last_n_lines2[(pointer_generator_counter+start_to_print)%numl]);


  }

  //if we are not able to read anything from the file discrptor
  if(n < 0)
   {
    printf(1, "tail: read error\n");
    exit();
   }
}

int main(int argc, char *argv[])
{
  char abc[10];
  int fd, i;


  //getting the arguments, argc==1 when we call tail without any parameters.
  if(argc>1 && argv[1][0]=='-')
  {
   for(i=1;i<sizeof(abc);i++)
     //reading the number from the argument, one character at a time
     abc[i-1]=argv[1][i];
   //convert the number to integer
   numl=atoi(abc);
 }else{

   // the first parameter may be a file name. Lets check if the second parameter is a number
   if(argc==3 && argv[2][0]=='-')
   {
    for(i=1;i<sizeof(abc);i++)
      abc[i-1]=argv[2][i];
    numl=atoi(abc);
    if((fd = open(argv[1], 0)) < 0 ){
      printf(1, "tail: cannot open %s\n", argv[1]);
  //this can be the number of lines to print and input from pipe
      tail(0);
      exit();
  }

  tail(fd);
  close(fd);
  exit();
  }

 }
  //if argc == 1, tail was called without any arguments
  if(argc <= 1){
    tail(0);
    exit();
  }

  i=2;
  if(argc==2)
   i=1;
  for(; i < argc; i++){
  
    if((fd = open(argv[i], 0)) < 0 ){
      printf(1, "tail: cannot open %s, it may input parameter\n", argv[i]);
//this can be the number of lines to print and input from pipe
      tail(0);
      exit();
    }

    tail(fd);
    close(fd);

  }
  exit();
}
