#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include<sys/ioctl.h>

#define WR_VALUE    _IOW('a', 'a', int32_t*)
#define RD_VALUE    _IOR('a', 'b', int32_t*)

int main(int argc, char *argv[])
{
    int fd;
    int32_t value, number;
    printf("*****************************\n");
    printf("*********ioctl test**********\n");

    printf("\nOpening driver\n");
    fd = open("/dev/ioctl_device", O_RDWR);
    if(fd < 0)
    {
        printf("Cannot open device file ... \n");
        return 0;
    }

    printf("Enter the value to send\n");
    scanf("%d", &number);
    printf("Writing value to Driver\n");
    ioctl(fd, WR_VALUE, (int32_t) &number);

    printf("Reading value from Driver\n");
    ioctl(fd, RD_VALUE, (int32_t) &value);
    printf("Value is %d \n", value);

    printf("Closing driver!\n");
    close(fd);

    return 0;
}