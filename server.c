#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include <sys/select.h>
#include <errno.h>

char msg[] = "string 1";
char msg1[] = "string 2";
char msg2[] = "string 3";

struct sockaddr_rc loc_addr = {0}, client_addr = {0};
char buf[1024] = {0};
int s, client, bytes_read;
socklen_t opt = sizeof(client_addr);

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

    //set timeout
    loc_addr.rc_family = AF_BLUETOOTH;
    // loc_addr.rc_bdaddr = *BDADDR_ANY;
    loc_addr.rc_channel = (uint8_t)1;
    bind(s, (struct sockaddr *)&loc_addr, sizeof(loc_addr));
    printf("server started\n");
    // put socket into listening mode
    listen(s, 1);
    printf("Server listening...\n");

    client = accept(s, (struct sockaddr *)&client_addr, &opt);
    ba2str(&client_addr.rc_bdaddr, buf);
    // str2ba("74:40:BB:03:D9:C4", &loc_addr.rc_bdaddr);

    fprintf(stderr, "Connection from %s\n", buf);
    memset(buf, 0, sizeof(buf));
    while (1)
    {
        // read data from the client
        printf("waiting to reading...\n");
        bytes_read = read(client, buf, sizeof(buf));
        if (bytes_read <= 0)
        {
            printf("Read failed\n");
            printf("%s\n", strerror(errno));
            printf("Restarting server...\n");
            close(client);
            // put socket into listening mode
            listen(s, 1);
            printf("Server listening...\n");

            client = accept(s, (struct sockaddr *)&client_addr, &opt);
            ba2str(&client_addr.rc_bdaddr, buf);
            fprintf(stderr, "Connection from %s\n", buf);
            memset(buf, 0, sizeof(buf));
        }
        // else
        // {
        printf("Data read: ");
        printf("%s\n", buf);
        int select = printRandoms(0, 2, 1);
        printf("%d\n", select);
        if (select == 0)
        {
            write(client, msg, sizeof(msg));
        }
        else if (select == 2)
        {
            write(client, msg2, sizeof(msg2));
        }
        else
        {
            write(client, msg1, sizeof(msg1));
        }
        // }
    }

    // close connection
    close(client);
    close(s);

    return 0;
}
