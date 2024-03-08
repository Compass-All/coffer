#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

void test_mkdir()
{
    int ret = mkdir("/new_dir", S_IRWXU | S_IRWXG | S_IRWXO);
    printf("mkdir ret = %d\n", ret);
    if (ret < 0) {
        perror("mkdir error: ");
        exit(0);
    }

    int fd = open(
        "/new_dir/tmp_file",
        O_WRONLY | O_CREAT,
        S_IRWXU | S_IRWXG | S_IRWXO
    );
    printf("new dir fd = %d\n", fd);
    if (fd < 0) {
        perror("Error: ");
        exit(0);
    }

    char string[] = "file in new dir\n";
    write(fd, string, sizeof(string));
    close(fd);

    fd = open("/new_dir/tmp_file", O_RDONLY);
    printf("fd = %d\n", fd);
    if (fd < 0)
    {
        perror("Error: ");
        exit(0);
    }
    
    char buffer[80];
    read(fd, buffer, sizeof(buffer));
    close(fd);

    printf("%s", buffer);
}

void test_create()
{
    int fd;
    char s[] = "Hello World!\n", buffer[80];

    char pathname[] = "/temp";
    int flags = O_WRONLY | O_CREAT;
    int mode = S_IRWXU | S_IRWXG;

    fd = open(pathname, flags, mode);
    printf("fd = %d\n", fd);
    if (fd < 0)
    {
        perror("Error: ");
        exit(0);
    }

    write(fd, s, sizeof(s));
    close(fd);


    fd = open("/temp", O_RDONLY);
    printf("fd = %d\n", fd);
    if (fd < 0)
    {
        perror("Error: ");
        exit(0);
    }

    read(fd, buffer, sizeof(buffer));
    close(fd);

    printf("%s", buffer);
}

void test_fopen()
{
    printf("test_fopen\n");

    FILE * fp;

    fp = fopen ("/new_dir/file.txt", "w+");
    if (!fp) {
        perror("Error: ");
        exit(0);
    }

    fprintf(fp, "%s %s %s %d", "We", "are", "in", 2014);
    fclose(fp);

    int c;
  
    fp = fopen("/new_dir/file.txt","r");
    if (!fp) {
        perror("Error: ");
        exit(0);
    }

    while(1)
    {
        c = fgetc(fp);
        if( feof(fp) )
        { 
            break;
        }
        printf("%c", c);
    }
    fclose(fp);

    return;
}

int main()
{
    test_create();
    test_mkdir();
    test_fopen();
}