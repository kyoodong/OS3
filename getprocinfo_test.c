#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"
#include "processInfo.h"

int main() {
	int max_pid;
	struct processInfo pinfo;

	printf(1, "%s\t%s\t%s\t%s\n", "PID", "PPID", "SIZE", "Number of Context Switch");
	max_pid = get_max_pid();

	for (int i = 1; i <= max_pid; i++) {
		if (get_proc_info(i, &pinfo) < 0)
			continue;

		printf(1, "%d\t%d\t%d\t%d\n", i, pinfo.ppid, pinfo.psize, pinfo.numberContextSwitches);
	}
	exit();
}
