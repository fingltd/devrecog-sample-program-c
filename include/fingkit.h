#ifndef FING_FINGKIT_H
#define FING_FINGKIT_H

#ifdef __cplusplus
extern "C"
{
#endif
#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
    #define FINGKIT_CALL __stdcall
    #define FINGKIT_DLL_EXPIMP extern __declspec(dllexport)
#else
    #define FINGKIT_CALL
    #define FINGKIT_DLL_EXPIMP
#endif
    /**
     * The callback invoked when events complete.
     * @param result the JSON result, if any.
     * @param statusCode Status code: 0 on success, >0 on failure.
     */
    typedef void(FINGKIT_CALL * HandleFingEvent)(const char *result, int statusCode);


    // ---- LICENSE VALIDATION ----
    /**
     * Validates a license key, notifying results to the given completion handler.
     *
     * @param licenseKey           the key to validate
     * @param userAgentToken       user agent token to be validated via webhook
     * @param callback             the handler of the notification event at method completion.
     */
    FINGKIT_DLL_EXPIMP void FINGKIT_CALL validateLicenseKey(const char *licenseKey, const char *userAgentToken, HandleFingEvent callback);

    FINGKIT_DLL_EXPIMP void FINGKIT_CALL getNetworkInterfaces();

    FINGKIT_DLL_EXPIMP void FINGKIT_CALL configureFingKit(const char *config);

    // discovery lifecycle
    FINGKIT_DLL_EXPIMP void FINGKIT_CALL startFingKit();
    FINGKIT_DLL_EXPIMP void FINGKIT_CALL refreshFingKitDiscovery(); // to force refresh before round interval
    FINGKIT_DLL_EXPIMP void FINGKIT_CALL stopFingKit();

    // --- FingKit library version
    FINGKIT_DLL_EXPIMP const char * FINGKIT_CALL getFingKitVersion();

#ifdef __cplusplus
}
#endif

#endif //FING_FINGKIT_H
