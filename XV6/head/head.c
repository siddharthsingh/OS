#include "types.h"
#include "user.h"
char buf[10];
int l, numl = 10;
void head(int fd)
{
  int n,i,l=0;
  while((n = read(fd, buf, sizeof(buf))) > 0)
   {
     for(i=0;i<n;i++)
     {
      printf(1,"%c", buf[i]);
      if(buf[i] == '\n')
       {
        l++;
        if(l==numl)
          exit();
       }
     }
   }
  if(n < 0)
   {
    printf(1, "Read error\n");
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
  }
  if(argc <= 1)
  {
    head(0);
    exit();
  }
  i=2;
  if(argc==2)
    i=1;
  for(;i < argc;i++)
  {
    if((fd = open(argv[i], 0)) < 0)
    {
     //this can be the number of lines to print and input from pipe
     head(0);
     exit();
    }
    head(fd);
    close(fd);
  }
  exit();
}
