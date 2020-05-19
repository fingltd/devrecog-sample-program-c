//
// Created by Giuseppe Badoni on 17/10/2018.
//

#include <stdio.h>
#include <string.h>

// system dependant includes
#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
#include <windows.h>
    #define SLEEP(x) Sleep(x * 1000)
    CRITICAL_SECTION lock;
	CONDITION_VARIABLE condition;

    #define MUTEX_INIT          InitializeCriticalSection(&lock)
    #define MUTEX_LOCK          EnterCriticalSection(&lock)
    #define MUTEX_UNLOCK        LeaveCriticalSection(&lock)
    #define CONDITION_INIT		InitializeConditionVariable(&condition)
    #define CONDITION_WAIT      SleepConditionVariableCS(&condition, &lock, INFINITE)
    #define CONDITION_NOTIFY    WakeConditionVariable(&condition)

	size_t                  /* O - Length of string */
	strlcat(char       *dst,        /* O - Destination string */
              const char *src,      /* I - Source string */
          size_t     size)      /* I - Size of destination string buffer */
	{
	  size_t    srclen;         /* Length of source string */
	  size_t    dstlen;         /* Length of destination string */


	 /*
	  * Figure out how much room is left...
	  */

	  dstlen = strlen(dst);
	  size   -= dstlen + 1;

	  if (!size)
		return (dstlen);        /* No room, return immediately... */

	 /*
	  * Figure out how much room is needed...
	  */

	  srclen = strlen(src);

	 /*
	  * Copy the appropriate amount...
	  */

	  if (srclen > size)
		srclen = size;

	  memcpy(dst + dstlen, src, srclen);
	  dst[dstlen + srclen] = '\0';

	  return (dstlen + srclen);
	}

	size_t getline(char **lineptr, size_t *n, FILE *stream) {
        char *dest = NULL, *ret, *newline;
        size_t len = 1024;

        if ((dest = malloc(len)) == NULL) {
                goto error;
        }
        
        /* Fetch up to line_length bytes from the file, or up to a newline */
        ret = fgets(dest, (int) (len-1), stream);
        if (ret == NULL) {
                if (feof(stream) != 0) {
                        dest[0] = '\0';
                        len = 0;
                        return 0;
                } else {
                        goto error;
                }
        }

        /* If the line was too long, and so doesn't contain a newline, carry on
         * fetching until it does, or we hit the end of the file. */
        while ((newline = strchr(dest, '\n')) == NULL) {
                char *new_dest, *tmp;

                /* Create a new storage space the same size as the last one, and carry
                 * on reading. We'll need to append this to the previous string - fgets
                 * will just overwrite it. */
                if ((tmp = malloc(len)) == NULL) {
                        goto error;
                }

                ret = fgets(tmp, (int) (len-1), stream);
                if (ret == NULL) {
                        /* This probably shouldn't happen... */
                        if (feof(stream) != 0) {
                                free(tmp);
                                break;
                        } else {
                                free(tmp);
                                goto error;
                        }
                }

                len *= 2;
                if ((new_dest = realloc(dest, (size_t)len)) == NULL) {
                        free(tmp);
                        goto error;
                }

                dest = new_dest;
                strlcat(dest, tmp, len);
                free(tmp);
        }

        /* Don't include the newline in the line we return. */
        if (newline != NULL)
                *newline = '\0';
		*lineptr = dest;
        return (size_t) (newline - dest + 1);

error:
        free(dest);
        dest = NULL;
        len = 0;
        return -1;
}
#else
#include <unistd.h>
#include <signal.h>
#define SLEEP(x) sleep(x)
#include <pthread.h>
#include <sys/time.h>

pthread_mutex_t lock;
pthread_cond_t  condition;

#define MUTEX_INIT pthread_mutex_init(&lock,NULL)
#define MUTEX_LOCK pthread_mutex_lock(&lock)
#define MUTEX_UNLOCK pthread_mutex_unlock(&lock)
#define CONDITION_INIT pthread_cond_init(&condition, NULL)
#define CONDITION_WAIT pthread_cond_wait(&condition,&lock)
#define CONDITION_NOTIFY pthread_cond_signal(&condition)

#endif

#include <fingkit.h>

int shutdown_r;
int refreshed;
int terminated;
int loop;


FINGKIT_DLL_EXPIMP void FINGKIT_CALL FingKitHE_imp (const char *result, int statusCode)
{
    const char * engineTerminated = "{ \"type\":\"engine\",\"statusCode\":0,\"status\":\"OK\", \"result\": {\"state\":\"terminated\"}}";
    MUTEX_LOCK;
    if (statusCode == 0) {
        printf("Result: %s\n", result);
        if (strncmp(result, engineTerminated, strlen(engineTerminated)) == 0) {
            terminated=1;
        }
    } else printf("Error: %s\n", result);

    CONDITION_NOTIFY;

    printf("===========================================================================================================================\n");
    MUTEX_UNLOCK;
}

#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)

/**
	Windows signal handler for standard/console mode.
	On Windows we cannot simply ask for async shutdown
	but also wait until fing completed shutdown sequence.
	In other case Windows has no patience and suddenly kills the program.
**/
BOOL winControlHandler(DWORD fdwCtrlType)
{
	shutdown_r=1;
	return TRUE;
}

void configureSignalHandlers()
{
	SetConsoleCtrlHandler((PHANDLER_ROUTINE) winControlHandler, TRUE);
}
#else
/**
	Linux and Mac signal handler.
**/
void shutdownSignalHandler(int sigNum)
{
    MUTEX_LOCK;
    shutdown_r=1;
    MUTEX_UNLOCK;
    CONDITION_NOTIFY;
}

void sigHupSignalHandler(int num)
{
    MUTEX_LOCK;
    refreshed=1;
    MUTEX_UNLOCK;
    CONDITION_NOTIFY;
}

void configureSignalHandlers()
{
    signal(SIGINT,  shutdownSignalHandler);
    signal(SIGQUIT, shutdownSignalHandler);
    signal(SIGTERM, shutdownSignalHandler);
    signal(SIGHUP, sigHupSignalHandler);
}

#endif

int main()
{
    char *license = NULL;
    const char *userToken = "";
    char *configure = NULL;
    size_t size;
    size_t characters;
    HandleFingEvent gptr = (HandleFingEvent) (void *) &FingKitHE_imp;

    configureSignalHandlers();

	terminated=0;
    shutdown_r=0;
    refreshed=0;
    MUTEX_INIT;
    CONDITION_INIT;
    printf("=== FingKit %s - www.fing.io ===\n\n", getFingKitVersion());


    printf("Insert license key that enable the usage of Fing Kit: \n");
    while ((characters=getline(&license, &size, stdin)) == -1) {
        printf("No line read, Try Again... \n");
    }
    if (license==NULL) {
        printf(">>>>>>  License NULL exit...\n");
        return 1;
    }
    license[characters-1]='\0';
    printf(">>>>>>  License key is:<%s>\n", license);


    validateLicenseKey(license, userToken, gptr);
    MUTEX_LOCK;
    CONDITION_WAIT;
    MUTEX_UNLOCK;

    getNetworkInterfaces();
    MUTEX_LOCK;
    CONDITION_WAIT;
    MUTEX_UNLOCK;

    loop = 2;
    while (loop > 0) {
        shutdown_r=0;
        refreshed=0;
        terminated=0;

        --loop;
        printf("Insert appropriate JSON configure : \n");
        while ((characters=getline(&configure, &size, stdin)) == -1) {
            printf("No line read, Try Again... \n");
        }
        configure[characters-1]='\0';
        printf(">>>>>> JSON configured: <%s>\n", configure);

        configureFingKit(configure);
        MUTEX_LOCK;
        CONDITION_WAIT;
        MUTEX_UNLOCK;

        startFingKit();

        while (1) {
            MUTEX_LOCK;
            CONDITION_WAIT;
            if (shutdown_r==1) {
                MUTEX_UNLOCK;
                break;
            }
            if (refreshed==1) {
                refreshFingKitDiscovery();
                refreshed=0;
            }
            MUTEX_UNLOCK;
        }

        printf("STOP command...\n");
        if (terminated==0)stopFingKit();
        while (terminated==0) {
            MUTEX_LOCK;
            CONDITION_WAIT;
            MUTEX_UNLOCK;
        }
    }

    printf("DONE...\n");
    return 1;
}