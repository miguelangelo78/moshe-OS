#include "task\thread.h"

void sleep(int ms) {
	int tck = ms + get_tick_count();
	while (tck > get_tick_count());
}