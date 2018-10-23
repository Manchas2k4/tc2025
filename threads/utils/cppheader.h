#ifndef TIMER_H
#define TIMER_H

#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <sys/time.h>
#include <sys/types.h>

const int N = 10;
const int DISPLAY = 100;
const int MAX_VALUE = 10000;

class Timer {
private:
    timeval startTime;
    bool 	started;

public:
    Timer() :started(false) {}

    void start(){
    	started = true;
        gettimeofday(&startTime, NULL);
    }

    double stop(){
        timeval endTime;
        long seconds, useconds;
        double duration = -1;

        if (started) {
			gettimeofday(&endTime, NULL);

			seconds  = endTime.tv_sec  - startTime.tv_sec;
			useconds = endTime.tv_usec - startTime.tv_usec;

			duration = (seconds * 1000.0) + (useconds / 1000.0);
			started = false;
        }
		return duration;
    }
};

void random_array(int *array, int size) {
	int i;

	srand(time(0));
	for (i = 0; i < size; i++) {
		array[i] = (rand() % 100) + 1;
	}
}

void fill_array(int *array, int size) {
	int i;

	for (i = 0; i < size; i++) {
		array[i] = (i % MAX_VALUE) + 1;
	}
}

void display_array(const char *text, int *array) {
	int i;

	printf("%s = [%4i", text, array[0]);
	for (i = 1; i < DISPLAY; i++) {
		printf(",%4i", array[i]);
	}
	printf(", ... ,]\n");
}

#endif
