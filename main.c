#include <unistd.h>

int main(int argc, char *argv[])
{
        (void)argc, (void)argv;
        write(STDOUT_FILENO, "fShell$ ", 8);
        return (0);
}