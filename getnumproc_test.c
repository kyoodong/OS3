#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int main() {
	int count = get_num_proc();
	printf(1, "Total Number of Active Processes: %d\n", count);
	exit();
}
