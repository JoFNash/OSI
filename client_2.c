#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> // inet_pton
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "erproc.h"
#define SIZE 256


int main(int argc, char * argv[]) {

  //int SIZE  = 256;
  int i;
  FILE * input_file;

  char * data_file = malloc(sizeof(char) * sizeof(input_file));
  int fd = Socket(AF_INET, SOCK_STREAM, 0);

  struct sockaddr_in adr = {0};
  adr.sin_family = AF_INET;
  adr.sin_port = htons(34540);

  Inet_pton(AF_INET, "127.0.0.1", &adr.sin_addr);
  Connect(fd, (struct sockaddr *) &adr, sizeof adr);

  size_t offset = 0;
  size_t nbytes = 0;
  size_t nread = 0;
  int sent;

  for (int i = 1; i < argc; i++){

    if (!(input_file = fopen(argv[i], "rb")))
      perror("file error");

    while ((nbytes = fread(data_file, sizeof(char), sizeof(input_file), input_file)) > 0)
      {
        sent = send(fd, data_file, nbytes, 0);
      }
  }
  shutdown(fd, 1);

  char buf[SIZE] = {'\0'};
  while (1) {

    nread = recv(fd, buf, 5, 0);

    if (nread == -1) {
      perror("read failed");
      exit(EXIT_FAILURE);
    }
    if (nread == 0) {
      printf("EOF occured\n");
    }
    //write(STDOUT_FILENO, buf, nread);
    printf("%s", buf);
    if (nread != 5)
      break;
  }
  // shutdown(fd, 1);
  fclose(input_file);
  close(fd);
  return 0;
}
