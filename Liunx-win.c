/*---------------------linux-------------------------------------------
파일명 : client.c
기  능 : echo 서비스를 요구하는 TCP(연결형) 클라이언트
리눅스 : gcc -o client client.c
사용법 : ./client 192.168.81.73
----------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#define BUF_LEN 128
#define PORT 2049

int main(int argc, char *argv[]) {
    int s, n, len_in, len_out;
    struct sockaddr_in server_addr;
    char *haddr;
    char buf[BUF_LEN + 1];

    if (argc != 2) {
        printf("usage: %s ip_address\n", argv[0]);
        exit(0);
    }
    haddr = argv[1];

    if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Can't create socket\n");
        exit(0);
    }

    /* echo 서버의 소켓주소 구조체 작성 */
    memset((char *)&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(haddr);
    server_addr.sin_port = htons(PORT);

    /* 연결요청 */
    if (connect(s, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        printf("Can't connect.\n");
        close(s);
        exit(0);
    }

    /* 키보드 입력을 받음 */
    printf("Input any string : ");
    if (fgets(buf, BUF_LEN, stdin)) {
        buf[BUF_LEN] = '\0';
        len_out = strlen(buf);
    } else {
        printf("fgets error\n");
        close(s);
        exit(0);
    }

    /* echo 서버로 메시지 송신 */
    if (send(s, buf, len_out, 0) < 0) {
        printf("send error\n");
        close(s);
        exit(0);
    }

    /* 수신된 echo 메시지 화면 출력 */
    printf("Echoed string : ");
    for (len_in = 0, n = 0; len_in < len_out; len_in += n) {
        if ((n = recv(s, &buf[len_in], len_out - len_in, 0)) < 0) {
            printf("recv error\n");
            close(s);
            exit(0);
        }
    }
    printf("%s", buf);
    close(s);
    return 0;
}