 #include <unistd.h>


int main()
{

int fd = dup(1);
dup2(2, 1);
dup2(fd, 2);
write(2, "aba", 3);
write(1, "caba", 4);

return 0;
}
