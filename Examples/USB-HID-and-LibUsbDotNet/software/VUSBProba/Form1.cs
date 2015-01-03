using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading;
using System.Windows.Forms;

using LibUsbDotNet;
using LibUsbDotNet.Info;
using LibUsbDotNet.Main;

namespace VUSBProba
{
    public partial class Form1 : Form
    {
        public static UsbDevice MyUsbDevice;
        public static UsbDeviceFinder MyUsbFinder = new UsbDeviceFinder(0x16c0, 0x05df);
               
        
        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            MyUsbDevice = UsbDevice.OpenUsbDevice(MyUsbFinder);
            if (MyUsbDevice != null)
            {
                label2.Text = " подключено !";
            }
            else label2.Text = " не найдено !";
        }

        private void button1_Click(object sender, EventArgs e)
        {
            //Зажигаем
            UsbSetupPacket packet = new UsbSetupPacket((byte)(UsbCtrlFlags.RequestType_Vendor | UsbCtrlFlags.Recipient_Device | UsbCtrlFlags.Direction_Out), 1, (short)1, 0, 0);
            int countIn;
            byte[] data = new byte[1];
            MyUsbDevice.ControlTransfer(ref packet, data, 0, out countIn);
         }

        private void button3_Click(object sender, EventArgs e)
        {
            //Гасим
            UsbSetupPacket packet = new UsbSetupPacket((byte)(UsbCtrlFlags.RequestType_Vendor | UsbCtrlFlags.Recipient_Device | UsbCtrlFlags.Direction_Out), 1, (short)0, 0, 0);
            int countIn;
            byte[] data = new byte[1];
            MyUsbDevice.ControlTransfer(ref packet, data, 0, out countIn);
        }

        private void button2_Click(object sender, EventArgs e)
        {
            MyUsbDevice = UsbDevice.OpenUsbDevice(MyUsbFinder); //Чтение данныйх
            UsbSetupPacket packet = new UsbSetupPacket((byte)(UsbCtrlFlags.RequestType_Vendor | UsbCtrlFlags.Recipient_Device | UsbCtrlFlags.Direction_In), 2, (short)0, (short)0, (short)0);
            int countIn;
            byte[] data = new byte[1];
            if (MyUsbDevice.ControlTransfer(ref packet, data, 1, out countIn) && (countIn == 1))
            {
               label3.Text = "Прочитано значние " + data[0].ToString();
            }
        }

        private void Form1_FormClosed(object sender, FormClosedEventArgs e)
        {
            UsbSetupPacket packet = new UsbSetupPacket((byte)(UsbCtrlFlags.RequestType_Vendor | UsbCtrlFlags.Recipient_Device | UsbCtrlFlags.Direction_Out), 1, (short)0, 0, 0);
            int countIn;
            byte[] data = new byte[1];
            MyUsbDevice.ControlTransfer(ref packet, data, 0, out countIn);
        }
     
    }
}
