using System;
using System.Net.Sockets;

namespace OrbManager
{
    public class TcpClientOrb : IDisposable
    {
        private readonly ColorByteConverter _converter;
        private TcpClient _client;
        private NetworkStream _stream;

        public TcpClientOrb(ArgParser args, ColorByteConverter converter)
        {
            _converter = converter;
            _client = new TcpClient();
            var result = _client.BeginConnect(args.ServerIp, args.PortNumber, null, null);
            var success = result.AsyncWaitHandle.WaitOne(TimeSpan.FromSeconds(2));
            if (!success)
            {
                throw new ApplicationException(string.Format("Failed to connect to host {0} port {1}.", args.ServerIp, args.PortNumber));
            }

            // we have connected
            _client.EndConnect(result);
        }

        public void Send(OrbColor color)
        {
            var colorByte = _converter.Color2Byte(color);
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
