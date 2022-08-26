

#include <fcntl.h>
#include <sys/file.h>
int file_lock(const char *file, int *fd)
{
    int _fd;

    if ((NULL == file) || (NULL == fd))
        return -1;
    *fd = -1;

    _fd = open(file, O_RDONLY);
    if (0 > _fd)
        return -1;

    if (0 != flock(_fd, LOCK_EX | LOCK_NB))
    {
        close(_fd);
        return -1;
    }

    *fd = _fd;
    return 0;
}
int file_unlock(int *fd)
{
    if (NULL == fd)
        return -1;
    if (0 > *fd)
        return 0;
    flock(*fd, LOCK_UN);
    close(*fd);
    *fd = -1;
    return 0;
}




#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <string.h>
#define MAX_BAR_LEN  200
//len:进度条'#'的长度
void print_progress_bar(const char *title, unsigned char percent, unsigned int len, const char *style)
{
    char bar[MAX_BAR_LEN + 1];
    struct winsize size;
    size_t title_len;
    unsigned int ws_col;
    unsigned int left_len;


    if ((NULL == title) || (0 == isatty(fileno(stdout))))
        return;

    if (100 < percent)
        percent = 100;

    if (10 > len)
    {
        title_len = strlen(title);
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
        ws_col = size.ws_col;
        if (0 == ws_col)
            ws_col = 80;

        if (ws_col > title_len + 10 + 11 + 3)
            len = ws_col - title_len - 11 - 3;
        else
            len = 10;
    }
    if (MAX_BAR_LEN < len)
        len = MAX_BAR_LEN;

    left_len = len * percent / 100;
    memset(bar, '#', left_len);
    memset(bar + left_len, '.', len - left_len);
    bar[len] = '\0';
    if (style)
        printf("%s", style);
    printf("\r%s\e[0m: [%3d%%] [%s]", title, percent, bar);
    fflush(stdout);
}
