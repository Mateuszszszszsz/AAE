#include <math.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>

char result[128];

void remove_spaces(char msg[]) {
  char *read = msg;
  char *write = msg;
  while (*read != '\0') {
    if (*read != ' ') {
      *write++ = *read;
    }
    read++;
  }
  *write = '\0';
}

void process_msg(char msg[]) {
  float a, b;
  char op;

  result[0] = '\0';

  remove_spaces(msg);

  if (sscanf(msg, "%f%c%f", &a, &op, &b) != 3) {
    sprintf(result, "Invalid input.\n");
    return;
  }

  switch (op) {
  case '+':
    sprintf(result, "\t= %.5g\n", a + b);
    break;
  case '-':
    sprintf(result, "\t= %.5g\n", a - b);
    break;
  case '*':
    sprintf(result, "\t= %.5g\n", a * b);
    break;
  case '/':
    sprintf(result, "\t= %.5g\n", a / b);
    break;
  case '%':
    if (a - round(a) != 0 || b - round(b) != 0) {
      sprintf(result, "To use modulo operator, please provide integer values.\n");
      return;
    }
    sprintf(result, "\t= %d\n", (int)a % (int)b);
    break;
  default:
    sprintf(result, "Invalid operator: %c\n", op);
    return;
  }

  printf("Calculated: %.5g %c %.5g %s\n", a, op, b, result);

  return;
}

int main(int argc, char **argv) {
  int socket_fd;
  int port_no;
  struct sockaddr_in serveraddr;
  struct sockaddr_in clientaddr;
  char *buf;

  if (argc != 2) {
    printf("Usage: %s <port_no>\n", argv[0]);
    exit(0);
  }

  port_no = atoi(argv[1]);

  socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
  if (socket_fd < 0) {
    perror("Failed to open socket");
    exit(1);
  }

  bzero(&serveraddr, sizeof serveraddr);
  serveraddr.sin_family = AF_INET;
  serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
  serveraddr.sin_port = htons((unsigned short)port_no);

  if (bind(socket_fd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0) {
    perror("Failed to bind socket");
    exit(1);
  }

  socklen_t len = sizeof(clientaddr);
  while (1) {
    buf = malloc(800);

    int n = recvfrom(socket_fd, buf, 800, 0, (struct sockaddr *)&clientaddr, &len);
    buf[n] = '\0';
    printf("Received: %s", buf);
    process_msg(buf);

    sendto(socket_fd, result, strlen(result), 0, (struct sockaddr *)&clientaddr, len);
  }
}

// For connecting to the server, use:
// $nc localhost <port_no> -u