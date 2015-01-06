using System;
using LibUsbDotNet;
using LibUsbDotNet.Main;


namespace OrbManager
{
    class Program
    {
        public static UsbDevice MyUsbDevice;

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
                    using (var serverClient = new TcpClientOrb(argValues, new ColorToByteConverter()))
                    {
                        serverClient.Send(argValues.Color);
                        Console.WriteLine("\r\nDone!\r\n");
                    }                    
                }

                // Find and open the usb device.
                MyUsbDevice = UsbDevice.OpenUsbDevice(MyUsbFinder);
                // If the device is open and ready
                if (MyUsbDevice == null) throw new Exception("Device Not Found.");

                var orb = new BuildOrbDevice(MyUsbDevice);
                if (argValues.IsClientMode)
                {
                    orb.TurnLightningOn(argValues.Color);
                }
                else
                {
                    var server = new TcpServerOrb(argValues, orb);
                    Console.WriteLine("Server was started on interface {0}", argValues.ServerIp);
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
                if (MyUsbDevice != null)
                {
                    if (MyUsbDevice.IsOpen)
                    {
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
        }
    }
}
