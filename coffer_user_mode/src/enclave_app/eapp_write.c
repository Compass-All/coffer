#include <enclave_util.h>
#include <unistd.h>

int main()
{
	char string[] = "Hello world from write.\n";
	write(1, string, sizeof(string));
}