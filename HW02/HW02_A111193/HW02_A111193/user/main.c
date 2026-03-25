#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h> 

int main() 
{
    const char *to_driver = "Prepare to write"; 
    int size = strlen(to_driver);
    
    char to_user[size + 1];
    memset(to_user, 0, sizeof(to_user));
    
    int fd = open("/dev/demo", O_RDWR);
    if (fd == -1) {
        printf("Can not open file");
        return -1;
    }

    ssize_t ret = write(fd, to_driver, size);
    if (ret == -1) {
        printf("Write fail\n");
    } 
    
    ret = read(fd, &to_user[0], size);
    if (ret == -1) {
        printf("Read fail\n");
    } else {
        to_user[ret] = '\0';
        printf("%s\n", to_user);
    }

    close(fd);
    return 0;
}
