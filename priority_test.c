#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int main() {
	printf(1, "Default priority = %d\n", get_prio());
	printf(1, "Set priority 1\n");
	set_prio(1);
	printf(1, "After set priority 1, current priority is %d\n", get_prio());
	printf(1, "Set priority 10\n");
	set_prio(10);
	printf(1, "After set priority 10, current priority is %d\n", get_prio());
	exit();
}
