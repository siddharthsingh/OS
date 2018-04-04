#include "types.h"
#include "user.h"
char buf[10];

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
  /* index into the arra from where we should start printing */
  int start_to_print = 0;
  int temp_data = 0;
  int pointer_generator_counter = 0;
  char *pointer_to_last_n_lines[numl];
  int length_each_line[numl];
  int count_each_line[numl];

  char *pointer_to_last_n_lines2[numl];

  for(;pointer_generator_counter<numl; pointer_generator_counter++){
    if(debug == 1){
      printf(1, "%d creating pointers" , pointer_generator_counter);
    }
    pointer_to_last_n_lines[pointer_generator_counter] = malloc( sizeof(char) * ( lenght ) );
    pointer_to_last_n_lines2[pointer_generator_counter]  = pointer_to_last_n_lines[pointer_generator_counter];
    length_each_line[pointer_generator_counter] = 100;
    count_each_line[pointer_generator_counter] = 0;

  }


  while((n = read(fd, buf, sizeof(buf))) > 0)
   {
     // printf(1,"%d \n" , fd);
     temp_data = 1;
     for(i=0;i<n;i++)
     {
       //store last line here
       //Check if the lastline array is full





       if(count_each_line[index_to_store] == length_each_line[index_to_store] -3){
         if(debug == 1){
         printf(1," realloc called\n" );
        }
         //expand the array size by relloc and update the length

        char *temp = realloc(pointer_to_last_n_lines2[index_to_store] ,  length_each_line[index_to_store] , sizeof(char)*( length_each_line[index_to_store]*2));
        pointer_to_last_n_lines2[index_to_store] = temp;
        length_each_line[index_to_store] *= 2;
        if(debug == 1){
          printf(1," realloc called 2\n" );
        }

         pointer_to_last_n_lines2[index_to_store][count_each_line[index_to_store]] = buf[i];
         if(debug == 1){
         printf(1,"realloc called 3\n" );
       }
       }else{
       pointer_to_last_n_lines2[index_to_store][count_each_line[index_to_store]] = buf[i];

       }
       // printf(1,"this works");

      count_each_line[index_to_store]++;
      if(debug == 1){
      printf(1,"this works count is %d %c \n" , count_each_line[index_to_store] , buf[i]);
    }
      if(buf[i] == '\n'){
          temp_data = 0;
          pointer_to_last_n_lines2[index_to_store][count_each_line[index_to_store]] = '\0';
          index_to_store = (index_to_store + 1)%numl;
          start_to_print = (index_to_store)%numl;
          count_each_line[index_to_store] = 0;


        l++;
        }


      }
    }
    // printf(1, "%d" , n);
    if(temp_data == 1){
      start_to_print = (++index_to_store)%numl;
    }
  // printf(1, "the last line %d lines are \n" , numl);
  pointer_generator_counter = 0;
  for(;pointer_generator_counter<numl; pointer_generator_counter++){
    printf(1, "%s", pointer_to_last_n_lines2[(pointer_generator_counter+start_to_print)%numl]);


  }

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

  if(argc>1 && argv[1][0]=='-')
  {
   for(i=1;i<sizeof(abc);i++)
     abc[i-1]=argv[1][i];
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
