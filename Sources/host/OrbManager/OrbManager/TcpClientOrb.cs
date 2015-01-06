using System;
using System.Net.Sockets;

namespace OrbManager
{
    public class TcpClientOrb : IDisposable
    {
        private readonly ColorToByteConverter _converter;
        private TcpClient _client;
        private NetworkStream _stream;

        public TcpClientOrb(ArgParser args, ColorToByteConverter converter)
        {
            _converter = converter;
            _client = new TcpClient(args.ServerIp, args.PortNumber);
        }

        public void Send(OrbColor color)
        {
            var colorByte = _converter.Convert(color);
            var stream = _client.GetStream();
            stream.WriteByte(colorByte);
            stream.Close();
        }


        public void Dispose()
        {
            if (_client != null)
            {
                _client.Close();
                _client = null;
            }
        }
    }
}
