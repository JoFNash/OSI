#include <sys/types.h>
#include <ctype.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h> // sockaddr_in
#include <arpa/inet.h> // htons
#include <unistd.h>
#include <string.h>
#include "erproc.h"

int struct_counter;

typedef struct Char_counter
{
  char charakter;
  int count;
} char_counter;


void Process_dict(char buf[], FILE ** file, char_counter ** res);

int main() {

  FILE * tmp_file;

  int server = Socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in adr = {0};
  adr.sin_family = AF_INET;
  adr.sin_port = htons(34540);
  Bind(server, (struct sockaddr *) &adr, sizeof adr);
  Listen(server, 5);
  socklen_t adrlen = sizeof adr;
  int fd = Accept(server, (struct sockaddr *) &adr, &adrlen);
  ssize_t nread;
  char buf[256] = {'\0'};
  //char * string = "";
  char bufstring[256] = {'\0'};

  char_counter * res = NULL;

  while ( (nread = read(fd, buf, 256)) > 0) {

    if (nread == -1) {
      perror("read failed");
      exit(EXIT_FAILURE);
    }
    if (nread == 0) {
      printf("END OF FILE occured\n");
    }

    if (nread < 256){
      Process_dict(buf, &tmp_file, &res);
      memset(buf, 0, sizeof(buf));
    }
  }

  if (!(tmp_file = fopen("tmp.txt", "r")))
    perror("File open error");

  size_t nbytes;
  int sent;

  while ((nbytes = fread(bufstring, sizeof(char), sizeof(tmp_file), tmp_file)) > 0)
    {
      sent = send(fd, bufstring, nbytes, 0);
    }

  fclose(tmp_file);
  close(fd);
  close(server);
  return 0;
}

void Process_dict(char str[], FILE ** tmp_file, char_counter **res){

  int i, j;

  if ((*res) == NULL)
    struct_counter = 0;

  for (i = 0; i < strlen(str); i++){
    if (isalpha(str[i])) {
      int flag = 0;
      for (j = 0; j < struct_counter; j++)
        {
          if (str[i] == (*res)[j].charakter)
            {
              (*res)[j].count += 1;
              flag = 1;
            }
        }
      if (flag == 0)
        {
          (*res) = realloc((*res), (++struct_counter) * sizeof(char_counter));
          (*res)[struct_counter - 1].charakter  = str[i];
          (*res)[struct_counter - 1].count = 1;
        }
    }
  }

  if (!((*tmp_file) = fopen("tmp.txt", "w")))
    perror("File open error");
  else {

    for (i = 0; i < struct_counter; i++)
      {
        fprintf((*tmp_file), "%c: %d occurences\n", (*res)[i].charakter, (*res)[i].count);
      }
  }

  fclose(*tmp_file);
}
