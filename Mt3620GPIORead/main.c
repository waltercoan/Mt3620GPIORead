#include <signal.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// applibs_versions.h defines the API struct versions to use for applibs APIs.
#include "applibs_versions.h"
#include <applibs/log.h>

#include "mt3620_rdb.h"

static volatile sig_atomic_t terminationRequired = false;
static int gpio = -1;

static void TerminationHandler(int signalNumber)
{
    terminationRequired = true;
}

int main(int argc, char *argv[])
{
    Log_Debug("Application starting.\n");

    struct sigaction action;
    memset(&action, 0, sizeof(struct sigaction));
    action.sa_handler = TerminationHandler;
    sigaction(SIGTERM, &action, NULL);

	gpio = GPIO_OpenAsInput(MT3620_GPIO5);

    const struct timespec sleepTime = {1, 0};
    while (!terminationRequired) {
		GPIO_Value_Type newState;
		GPIO_GetValue(gpio,&newState);
		Log_Debug("Valor GPIO 5 (%s) \n", (newState == GPIO_Value_High)?"On":"Off");
        nanosleep(&sleepTime, NULL);
    }

    Log_Debug("Application exiting.\n");
    return 0;
}
