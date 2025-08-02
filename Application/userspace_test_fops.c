#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int8_t write_buf[1024];
int8_t read_buf[1024];

int main(int argc, char *argv[])
{
    int fd;
    char option;
    printf("**********************************\n");
    printf("*****App test file operations*****\n");

    fd = open("/dev/real_device", O_RDWR);
    if(fd < 0)
    {
        printf("Cannot open file!\n");
        return 0;
    }

    while(1)
    {
        printf("***Please Enter the options***\n");
        printf("    1. Write    \n");
        printf("    2. Read     \n");
        printf("    3. Exit     \n");
        printf("**********************************\n");
        scanf(" %c", &option);
        printf("Your option = %c \n", &option);

        switch(option)
        {
            case '1':
                printf("Enter the string you want to write into driver: \n");
                scanf("  %[\\t\\n]", write_buf);
                printf("Data Writing ... ");
                write(fd, write_buf, strlen(write_buf)+1);
                printf("Done!\n");
                break;
            case '2':
                printf("Data reading ... ");
                read(fd, read_buf, 1024);
                printf("Done!\n");
                printf("Data = %s \n\n", read_buf);
                break;
            case '3':
                close(fd);
                exit(1);
                break;
            default:
                printf("Enter invalid option = %c\n", option);
                break;
        }
    }
    close(fd);
    return 0;
}