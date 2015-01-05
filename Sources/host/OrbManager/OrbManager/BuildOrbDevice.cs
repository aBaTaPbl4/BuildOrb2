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
        private readonly ArgParser _argValues;
        private readonly UsbDevice _orbDevice;

        public BuildOrbDevice(ArgParser argValues, UsbDevice orbDevice)
        {
            _argValues = argValues;
            _orbDevice = orbDevice;
        }

        public void TurnLightningOn()
        {
            //Зажигаем
            var converner = new ColorToByteConverter();
            byte colorByte = converner.Convert(_argValues.Color);
            UsbSetupPacket packet = new UsbSetupPacket((byte)(UsbCtrlFlags.RequestType_Vendor | UsbCtrlFlags.Recipient_Device | UsbCtrlFlags.Direction_Out), 1, (short)colorByte, 0, 0);
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
                return "Прочитано значние " + data[0].ToString();
            }
            return "";
        }
    }
}
