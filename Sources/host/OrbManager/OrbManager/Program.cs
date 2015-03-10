using System;
using System.Runtime.InteropServices;
using LibUsbDotNet;
using LibUsbDotNet.Main;


namespace OrbManager
{
    class Program
    {
        public static UsbDevice MyUsbDevice;
        private static BuildOrbDevice _orb;

        #region SET YOUR USB Vendor and Product ID!

        public static UsbDeviceFinder MyUsbFinder =
            new UsbDeviceFinder(0x16C0, 0x05DB); // specify vendor, product id

        #endregion

        public static void Main(string[] args)
        {
            
            ErrorCode ec = ErrorCode.None;

            try
            {
                var argValues = new ArgParser(args);

                if (argValues.IsRemoteClientMode)
                {
                    using (var serverClient = new TcpClientOrb(argValues, new ColorByteConverter()))
                    {
                        serverClient.Send(argValues.Color);
                        Console.WriteLine("\r\nDone!\r\n");
                        return;
                    }                    
                }

                // Find and open the usb device.
                MyUsbDevice = UsbDevice.OpenUsbDevice(MyUsbFinder);
                // If the device is open and ready
                if (MyUsbDevice == null) throw new Exception("Device Not Found.");
                SetConsoleCtrlHandler(new HandlerRoutine(ConsoleCtrlCheck), true);
                _orb = new BuildOrbDevice(MyUsbDevice);
                if (argValues.IsClientMode)
                {
                    _orb.TurnLightningOn(argValues.Color);
                }
                else
                {
                    var server = new TcpServerOrb(argValues, _orb);
                    Console.WriteLine("Server was started on interface {0}", server.ServerIpAddress);
                    server.Start();
                }

                Console.WriteLine("\r\nDone!\r\n");
            }
            catch (Exception ex)
            {
                Console.WriteLine();
                Console.WriteLine((ec != ErrorCode.None ? ec + ":" : String.Empty) + ex.Message);
            }
            finally
            {
                ReleaseResourcesIfRequired();
            }
        }

        private static void ReleaseResourcesIfRequired()
        {
            if (MyUsbDevice != null)
            {
                if (MyUsbDevice.IsOpen)
                {
                    _orb.TurnLightningOff();
                    IUsbDevice wholeUsbDevice = MyUsbDevice as IUsbDevice;
                    if (!ReferenceEquals(wholeUsbDevice, null))
                    {
                        // Release interface
                        wholeUsbDevice.ReleaseInterface(1);
                    }

                    MyUsbDevice.Close();
                }
                MyUsbDevice = null;
                // Free usb resources
                UsbDevice.Exit();
            }
        }

        #region OnConsole Closed
        [DllImport("Kernel32")]
        public static extern bool SetConsoleCtrlHandler(HandlerRoutine Handler, bool Add);

        // A delegate type to be used as the handler routine 
        // for SetConsoleCtrlHandler.
        public delegate bool HandlerRoutine(CtrlTypes CtrlType);

        // An enumerated type for the control messages
        // sent to the handler routine.
        public enum CtrlTypes
        {
            CTRL_C_EVENT = 0,
            CTRL_BREAK_EVENT,
            CTRL_CLOSE_EVENT,
            CTRL_LOGOFF_EVENT = 5,
            CTRL_SHUTDOWN_EVENT
        }

        private static bool ConsoleCtrlCheck(CtrlTypes ctrlType)
        {
            ReleaseResourcesIfRequired();
            return true;
        }
        #endregion
    }
}
