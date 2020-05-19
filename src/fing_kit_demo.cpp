/*
 * author : Giuseppe Badoni
 *
 * Copyright 2018 by Fing
 */

#include <iostream>
#include <csignal>
#include <cstdio>

#include <pthread.h>
#include <unistd.h>

//fingkit library
#include <fingkit.h>

FINGKIT_DLL_EXPIMP void FINGKIT_CALL FingKitHE_imp (const char *result, int statusCode)
{
    if (statusCode == 0) std::cout<<"Result: ";
    else std::cout<<"Error: ";

    std::cout<< result<<std::endl;
    std::cout<<"===========================================================================================================================\n";
}


void* thread_run(void* arg);

class FingKitDemo
{
public:
    FingKitDemo() : m_shutdown(false), m_terminated(false), m_forceDiscovery(false)
    {
        pthread_mutex_init(&m_lock, NULL);
        pthread_cond_init(&m_condition, NULL);
    }

    ~FingKitDemo() {
          shutdown();

        {
            std::cout << "exiting.... \n";

            while (!isTerminated()) {
                pthread_mutex_lock(&m_lock);
                pthread_cond_wait(&m_condition, &m_lock);
                pthread_mutex_unlock(&m_lock);
            }
        }
    }

    void run() {
        pthread_create(&m_engineThread, NULL, thread_run, this);

        while (!isTerminated()) {
            pthread_mutex_lock(&m_lock);
            pthread_cond_wait(&m_condition, &m_lock);
            pthread_mutex_unlock(&m_lock);
        }
    }

    //{"networkinterface":"en0", "enablelogging":"TRUE", "fingkit": { "fing": {"logging":{ "enable": "TRUE", "level":"INFO"}}}}

    void runTask() {
        const char* configure = "{\n"
                                " \"networkinterface\": \"default\",\n"
                                " \"discoveryinterval\": \"60000\",\n"
                                " \"discoveryround\": \"0\",\n"
                                " \"fullprotocolinfo\": \"true\",\n"
                                " \"discoverydatachunksize\": \"25\",\n"
                                " \"enablelogging\": \"TRUE\",\n"
                                " \"fingkit\":\n"
                                "{   \"overlook\": { \n"
                                "      \"cloud\": {\n"
                                "        \"endpoint\": {\n"
                                "          \"production\": \"true\"\n"
                                "        }\n"
                                "      }, \n"
                                "      \"fing\": {\n"
                                "        \"logging\": { \n"
                                "           \"enabled\": \"TRUE\", \n"
                                "           \"level\": \"INFO\" \n"
                                "        }\n"
                                "      }\n"
                                "    }\n"
                                //"    \"enrichment\": {\n"
                                //"      \"enabled\": \"true\"\n"
                                //"    },\n"
                                //"    \"license\": \"BGZpbmcERmluZxpmaW5nLXRvbS10ZXN0LWtpdC1lbWJlZGRlZAAAAWRaAr0GjRmilQ\",\n"
                                //"    \"rounds\": 0,\n"
                                //"    \"refresh\": {\n"
                                //"      \"interval\": 3600000\n"
                                //"    }\n"
                                //"   }\n"
                                "}\n"
                                "}";
        const char * license = "BGZpbmcERmluZxpmaW5nLXRvbS10ZXN0LWtpdC1lbWJlZGRlZAAAAWRaAr0GjRmilQ==";
        const char * userToken = "QdAed41q1dbYh+EQRb/5lA==";

        HandleFingEvent gptr = (HandleFingEvent) (void *) &FingKitHE_imp;
        validateLicenseKey(license,userToken, gptr);
        getNetworkInterfaces();
        configureFingKit(configure);
        startFingKit();

        while (!isShutdown()) {
            pthread_mutex_lock(&m_lock);
            pthread_cond_wait(&m_condition, &m_lock);
            if (m_shutdown) break;
            if (m_refresh) refreshFingKitDiscovery();
            pthread_mutex_lock(&m_lock);

            stopFingKit();
            pthread_mutex_lock(&m_lock);
            m_terminated=true;
            pthread_mutex_unlock(&m_lock);
            pthread_cond_signal(&m_condition);
        }

    }


    void sigHup() {
        printf("Refresh command...\n");
        pthread_mutex_lock(&m_lock);
        m_refresh=true;
        pthread_mutex_unlock(&m_lock);
        pthread_cond_signal(&m_condition);
    }

    void shutdown() {
        printf("STOP command...\n");
        pthread_mutex_lock(&m_lock);
        m_shutdown=true;
        pthread_mutex_unlock(&m_lock);
        pthread_cond_signal(&m_condition);
    }

    void setCommandLineArguments(int32_t argc, char* const* argv) {}

private:
    bool isShutdown() {
        pthread_mutex_lock(&m_lock);
        bool shutdown = m_shutdown;
        pthread_mutex_unlock(&m_lock);
        return shutdown;
    }

    bool isTerminated() {
        pthread_mutex_lock(&m_lock);
        bool terminated = m_terminated;
        pthread_mutex_unlock(&m_lock);
        return terminated;
    }

private:
    bool m_shutdown;
    bool m_refresh;
    bool m_terminated;
    bool m_forceDiscovery;
    pthread_mutex_t m_lock;
    pthread_cond_t m_condition;
    pthread_t m_engineThread;

};


FingKitDemo fingKitDemo;

void* thread_run(void* arg)
{
    FingKitDemo* kit = (FingKitDemo*) arg;
    kit->runTask();
    return kit;
}

void shutdownSignalHandler(int sigNum)
{
    fingKitDemo.shutdown();
}

void sigHupSignalHandler(int num)
{
    fingKitDemo.sigHup();
}

void configureSignalHandlers()
{
    signal(SIGINT,  shutdownSignalHandler);
    signal(SIGQUIT, shutdownSignalHandler);
    signal(SIGTERM, shutdownSignalHandler);
    signal(SIGHUP, sigHupSignalHandler);
}


/**
	Fing entry point on Linux and Mac.
**/
int main(int argc, char* const* argv)
{
    configureSignalHandlers();

    fingKitDemo.setCommandLineArguments(argc, argv);

    fingKitDemo.run();


}