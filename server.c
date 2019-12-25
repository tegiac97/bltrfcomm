#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include <sys/select.h>
#include <errno.h>

char msg[] = "sdfasdfsdaf";
char msg2[] = "1111111111111";
struct sockaddr_rc loc_addr = {0}, client_addr = {0};
char buf[1024] = {0};
int s, client, bytes_read;
socklen_t opt = sizeof(client_addr);
fd_set set;
struct timeval timeout = {5, 0};

int printRandoms(int lower, int upper,
                 int count)
{
    int i, num;
    for (i = 0; i < count; i++)
    {
        num = (rand() %
               (upper - lower + 1)) +
              lower;
        // printf("%d ", num);
    }
    return num;
}

int main(int argc, char **argv)
{

    // allocate socket
    s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

    // setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (const char *)&timeout, sizeof(struct timeval));

    // FD_ZERO(&set);
    // FD_SET(s, &set);

    //set timeout
    // int rv = select(s + 1, &set, NULL, NULL, &timeout);
    loc_addr.rc_family = AF_BLUETOOTH;
    // loc_addr.rc_bdaddr = *BDADDR_ANY;
    loc_addr.rc_channel = (uint8_t)1;
    bind(s, (struct sockaddr *)&loc_addr, sizeof(loc_addr));
    printf("server started\n");
    // put socket into listening mode
    listen(s, 1);
    printf("Server listening...\n");

    client = accept(s, (struct sockaddr *)&client_addr, &opt);

    // str2ba("74:40:BB:03:D9:C4", &loc_addr.rc_bdaddr);

    fprintf(stderr, "%s\n", buf);
    memset(buf, 0, sizeof(buf));
    while (1)
    {
        // read data from the client
        // clock_t start;
        write(client, "msg", sizeof("msg"));
        printf("waiting to reading...\n");
        bytes_read = read(client, buf, sizeof(buf));
        if (bytes_read <= 0)
        {
            printf("Read failed\n");
            printf("%s\n", strerror(errno));
            // close(s);
            close(client);
            // put socket into listening mode
            listen(s, 1);
            printf("Server listening...\n");

            client = accept(s, (struct sockaddr *)&client_addr, &opt);
        }
        else
        {
            printf("Data read: ");
            printf("%s\n", buf);
        }

        char *msg_send = NULL;
        int select = printRandoms(0, 1, 1);
        // int select=0;
        // printf("%f\n",select);
        // if (select == 0 && bytes_read > 0)
        // {
        //     printf("%s\n", buf);
        //     write(client, msg, sizeof(msg));
        // }
        // else
        // {
        //     printf("%s\n", buf);
        //     write(client, msg2, sizeof(msg2));
        // }
        // printf("%s", msg_send);
    }

    // close connection
    close(client);
    close(s);
    clock_t stop = clock();

    // printf("\n%f seconds", (double)(stop - start) / CLOCKS_PER_SEC);
    return 0;
}
