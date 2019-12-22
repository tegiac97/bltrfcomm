#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <pthread.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>

char msg[] = "HLW";
char msg_temp[] = "0\n\n\n1\t\t\t2\r\r\r3";

pthread_t pMain_thread, pReconnect;

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
        srand(time(0));
        num = (rand() %
               (upper - lower + 1)) +
              lower;
        printf("%d ", num);
    }
    return num;
}

void createConnection()
{
    printf("Creating connection...\n");
    // allocate socket
    s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

    loc_addr.rc_family = AF_BLUETOOTH;
    // loc_addr.rc_bdaddr = *BDADDR_ANY;
    loc_addr.rc_channel = (uint8_t)1;
    bind(s, (struct sockaddr *)&loc_addr, sizeof(loc_addr));
printf("Connection started\n");
printf("Listening...\n");
    // put socket into listening mode
    listen(s, 1);

    client = accept(s, (struct sockaddr *)&client_addr, &opt);

    str2ba("18:BB:26:AC:76:95", &loc_addr.rc_bdaddr);
    ba2str(&loc_addr.rc_bdaddr, buf);
    fprintf(stderr, "%s\n", buf);
    memset(buf, 0, sizeof(buf));
    
}

void closeConnection()
{
    close(client);
    close(s);
    printf("Connection closed\n");
}

void *reconnect()
{
    while (1)
    {
        sleep(1);
        printf("Reconnect thread\n");
        // closeConnection();
        // createConnection();
    }
}

void *main_thread()
{
    createConnection();
    while (1)
    {
        // read data from the client
        bytes_read = read(client, buf, sizeof(buf));
        char *msg_send = NULL;
        int select = printRandoms(0, 1, 1);
        // int select = 1;
        // printf("%f\n",select);
        if (select == 0 && bytes_read > 0)
        {
            printf("%s\n", buf);
            write(client, msg, sizeof(msg));
            // write(client, "Stop", sizeof("Stop"));
        }
        else
        {
            printf("%s\n", buf);
            // write(client, msg2, sizeof(msg2));
        }
        printf("%s", msg_send);
    }

    // close connection
    closeConnection();
}

int main(int argc, char **argv)
{
    pthread_create(&pMain_thread, NULL, main_thread, NULL);
    pthread_create(&pReconnect, NULL, reconnect, NULL);

    pthread_join(pMain_thread, NULL);
    pthread_join(pReconnect, NULL);

    return 0;
}
