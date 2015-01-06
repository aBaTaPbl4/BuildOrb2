using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using LibUsbDotNet;
using LibUsbDotNet.Main;

namespace OrbManager
{
    public class BuildOrbDevice
    {
        private readonly UsbDevice _orbDevice;

        public BuildOrbDevice(UsbDevice orbDevice)
        {
            _orbDevice = orbDevice;
        }

        public void TurnLightningOn(OrbColor color)
        {
            //Зажигаем
            var converner = new ColorToByteConverter();
            byte colorByte = converner.Convert(color);
            TurnLightningOn(colorByte);            
        }

        public void TurnLightningOn(byte colorByte)
        {
            //Зажигаем
            UsbSetupPacket packet = new UsbSetupPacket((byte)(UsbCtrlFlags.RequestType_Vendor | UsbCtrlFlags.Recipient_Device | UsbCtrlFlags.Direction_Out), 1, colorByte, 0, 0);
            int countIn;
            byte[] data = new byte[1];
            _orbDevice.ControlTransfer(ref packet, data, 0, out countIn);

        }

        public void TurnLightningOff()
        {
            //Гасим
            UsbSetupPacket packet = new UsbSetupPacket((byte)(UsbCtrlFlags.RequestType_Vendor | UsbCtrlFlags.Recipient_Device | UsbCtrlFlags.Direction_Out), 1, (short)0, 0, 0);
            int countIn;
            byte[] data = new byte[1];
            _orbDevice.ControlTransfer(ref packet, data, 0, out countIn);            
        }

        public string ReadLightningState()
        {
            UsbSetupPacket packet = new UsbSetupPacket((byte)(UsbCtrlFlags.RequestType_Vendor | UsbCtrlFlags.Recipient_Device | UsbCtrlFlags.Direction_In), 2, (short)0, (short)0, (short)0);
            int countIn;
            byte[] data = new byte[1];
            if (_orbDevice.ControlTransfer(ref packet, data, 1, out countIn) && (countIn == 1))
            {
                return "Прочитано значние цвета" + data[0].ToString();
            }
            return "";
        }

        public string ReadPortState()
        {
            UsbSetupPacket packet = new UsbSetupPacket((byte)(UsbCtrlFlags.RequestType_Vendor | UsbCtrlFlags.Recipient_Device | UsbCtrlFlags.Direction_In), 3, (short)0, (short)0, (short)0);
            int countIn;
            byte[] data = new byte[1];
            if (_orbDevice.ControlTransfer(ref packet, data, 1, out countIn) && (countIn == 1))
            {
                return string.Format("Прочитано значние порта {0}", data[0]);
            }
            return "";            
        }
    }
}
