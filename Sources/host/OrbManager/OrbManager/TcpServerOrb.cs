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

        public TcpServerOrb(ArgParser args, BuildOrbDevice orb)
        {
            _orb = orb;
            _listener = new TcpListener(IPAddress.Parse(args.ServerIp), args.PortNumber);
        }

        public void Start()
        {
            _listener.Start();

            while (true)
            {
                var client = _listener.AcceptTcpClient();
                var cliStream = client.GetStream();
                byte colorByte = (byte)cliStream.ReadByte();
                _orb.TurnLightningOn(colorByte);
                client.Close();
            }
        }

        // Остановка сервера
        ~TcpServerOrb()
        {
            if (_listener != null)
            {
                _listener.Stop();
            }
        }

    }
}
