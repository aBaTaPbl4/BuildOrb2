using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;

namespace OrbManager
{
    public class TcpServerOrb
    {
        private readonly BuildOrbDevice _orb;
        readonly TcpListener _listener;
        private IProgressColorCounter _colorCounter;
        private ColorByteConverter _converter;
        public TcpServerOrb(ArgParser args, BuildOrbDevice orb)
        {
            _orb = orb;
            _converter = new ColorByteConverter();
            if (args.ServerIpAddressDetected)
            {
                ServerIpAddress = IPAddress.Parse(args.ServerAddress);
            }
            else
            {
                var ips = Dns.GetHostAddresses(args.ServerAddress);
                if (ips == null || ips.Length == 0)
                {
                    args.ThrowIncorrectServerAddress(args.ServerAddress);
                }
                if (ips.Any(x => x.AddressFamily.ToString() == ProtocolFamily.InterNetwork.ToString()))
                {
                    ServerIpAddress = ips.First(x => x.AddressFamily.ToString() == ProtocolFamily.InterNetwork.ToString());
                }
                else
                {
                    ServerIpAddress = ips.Last();
                }                
            }
            _listener = new TcpListener(ServerIpAddress, args.PortNumber);
            if (args.CountProgressColors)
            {
                _colorCounter = new ProgressColorCounter(args.ProgressColorForServer, args.SuccessColorForServer);        
            }
            else
            {
                _colorCounter = new NullColorCounter();
            }
        }

        public IPAddress ServerIpAddress { get; private set; }

        public void Start()
        {
            _listener.Start();

            while (true)
            {
                var colorPacket = WaitForColorByteCommand();
                byte colorByteToSend = CalculateColorByteForOrb(colorPacket);
                _orb.TurnLightningOn(colorByteToSend);
            }
        }

        private byte CalculateColorByteForOrb(ColorPacket colorPacket)
        {
            OrbColor receivedColor = _converter.Byte2Color(colorPacket.ColorByte);
            Console.WriteLine("{0} Recieved color {1} from sender {2}", DateTime.Now, receivedColor, colorPacket.SenderName);
            _colorCounter.ProcessColor(receivedColor, colorPacket.SenderName);
            var colorToSendToOrb = _colorCounter.GetCurrentColor();
            var colorByteToSendToOrb = _converter.Color2Byte(colorToSendToOrb);
            return colorByteToSendToOrb;
        }

        private ColorPacket WaitForColorByteCommand()
        {
            TcpClient client = _listener.AcceptTcpClient();
            string clientName = GetClientNameSafe(client);
            var cliStream = client.GetStream();
            byte receivedColorByte = (byte) cliStream.ReadByte();
            client.Close();
            return new ColorPacket() {ColorByte = receivedColorByte, SenderName = clientName};
        }

        private string GetClientNameSafe(TcpClient client)
        {
            try
            {
                return ((IPEndPoint)client.Client.RemoteEndPoint).Address.ToString();
            }
            catch (Exception ex)
            {
                return "unknown";
            }
            
        }

        // Остановка сервера
        ~TcpServerOrb()
        {
            if (_listener != null)
            {
                _orb.TurnLightningOff();
                _listener.Stop();
            }
        }

    }
}
