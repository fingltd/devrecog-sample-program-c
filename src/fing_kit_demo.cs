using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;
using System.Threading;

namespace FingKitConsoleApp
{
    class Program
    {

        [UnmanagedFunctionPointer(CallingConvention.StdCall)]
        public delegate void HandleFingEvent([MarshalAs(UnmanagedType.LPStr)]string result, int statusCode);

        [DllImport("fingkit.dll", EntryPoint = "validateLicenseKey", CallingConvention = CallingConvention.StdCall)]
        public static extern void validateLicenseKey(IntPtr licenseKey, IntPtr userAgentToken, IntPtr callback);
        [DllImport("fingkit.dll", EntryPoint = "configureFingKit", CallingConvention = CallingConvention.StdCall)]
        public static extern void configureFingKit(IntPtr config);
        [DllImport("fingkit.dll")]
        public static extern void getNetworkInterfaces();
        [DllImport("fingkit.dll")]
        public static extern void startFingKit();
        [DllImport("fingkit.dll")]
        public static extern void refreshFingKitDiscovery();
        [DllImport("fingkit.dll")]
        public static extern void stopFingKit();
        static readonly object mutex = new object();

        static void worker()
        {
            string config = "{ " +
                               " \"networkinterface\": \"default\",\n" +
                               " \"discoveryinterval\": \"60000\",\n" +
                               " \"discoveryround\": \"0\",\n" +
                               " \"fullprotocolinfo\": \"true\",\n" +
                               " \"discoverydatachunksize\": \"25\"\n" +
                               "}";

            string license = "";
            string userToken = "";

            Console.WriteLine("Insert license key that enable the usage of Fing Kit: ");
            license = Console.ReadLine();

            // define a progress callback delegate
            HandleFingEvent callback =
                (result, code) =>
                {
                    lock (mutex)
                    {

                        Console.WriteLine("Progress = .... {0} {1}", result, code);
                        Monitor.Pulse(mutex);
                    }
                };

            int x;

            try
            {
                lock (mutex)
                {
                    // Converting callback  into a function pointer that can be
                    // used in unmanaged code.
                    IntPtr intptr_delegate = Marshal.GetFunctionPointerForDelegate(callback);

                    var licensePtr = Marshal.StringToHGlobalAnsi(license);
                    var userTokenPtr = Marshal.StringToHGlobalAnsi(userToken);
                    validateLicenseKey(licensePtr, userTokenPtr, intptr_delegate);

                    Monitor.Wait(mutex);
                }
                lock (mutex) {
                    getNetworkInterfaces();
                    Monitor.Wait(mutex);
                }
                lock (mutex)
                {
                    Console.WriteLine("Insert appropriate JSON configure : ");
                    config = Console.ReadLine();
                    var configPtr = Marshal.StringToHGlobalAnsi(config);

                    configureFingKit(configPtr);
                    Monitor.Wait(mutex);
                }

                bool startKit = false;
                while (!startKit)
                {
                    Console.WriteLine("Do you want start FingKit? (yes or no): ");
                    string resp = Console.ReadLine();
                    if (resp.Equals("yes")) break;
                    else if (resp.Equals("no")) return;
                }
                startFingKit();

                x = 100000;
                while (--x != 0)
                {
                    //refreshFingKitDiscovery();
                    System.Threading.Thread.Sleep(12*60 * 60 * 1000);
                }
                stopFingKit();
            }
            catch (Exception error)
            {
                // catch for error in adding
                Console.WriteLine("exception: {0}", error);
            }
        }

        [STAThread]
        static void Main(string[] args)
        {
            try
            {
                System.Threading.Thread th = new Thread(worker);
                th.Start();

                th.Join();

                Console.WriteLine("DONE...");
            }
            catch (Exception e)
            {
                Console.WriteLine(e.Message);
            }
        }
    }
}
