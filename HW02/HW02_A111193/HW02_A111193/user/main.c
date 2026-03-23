#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>


int main() 
{
    char *to_driver ="Prepare to write"; 
    int size = strlen(to_driver);
    char to_user[size];
    
    int fd = open("/dev/demo", O_RDWR);
    ssize_t ret;
    if (fd == -1)
    {
        printf("Can not open file\n");
        return -1;
    }
    ret = write(fd, to_driver, size);
    if (ret > 0)
        printf("Write fail\n");

    ret = read(fd, &to_user[0], size);
    if (ret > 0)
        printf("Read fail\n");
    printf("%s\n", to_user);
    close(fd);

    return 0;
}

