#include <sched.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>



int main(){
	struct sched_param p;
	p.sched_priority = sched_get_priority_max(SCHED_RR);
	sched_setscheduler(0, SCHED_RR, &p);
	execlp("bash", "bash", NULL);

	return 0;
}
