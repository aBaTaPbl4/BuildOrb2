using System;
using System.Collections.Generic;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;

namespace OrbManager
{
    public enum OrbColor
    {
        Red,
        Green,        
        Blue,
        Yellow,
        White,
        Unknown
    }

    public class ArgumentsParseException : ApplicationException
    {
        public ArgumentsParseException(string msg) : base(msg)
        {
            
        }

        public ArgumentsParseException(string fmt, params object[] arguments) : base(string.Format(fmt, arguments))
        {
            
        }
    }

    public class ArgParser
    {
        public ArgParser(string[] args)
        {
            if (args.Length == 0)
            {
                throw new ArgumentsParseException(ArgumentsUsage);
            }
            PortNumber = DefaultPortNumber;
            IsClientMode = false;
            IsLocalMode = true;
            ServerIp = null;
            Color = OrbColor.Unknown;
            IsClientMode = !args.Any(x => x.StartsWith("/startServer:", true, CultureInfo.CurrentCulture));
            if (IsClientMode)
            {
                string colorArg = args.FirstOrDefault(x => x.StartsWith("/color:", true, CultureInfo.CurrentCulture));
                if (colorArg == null)
                {
                    throw new ArgumentsParseException("You must specify color!");
                }
                var colorArgParts = colorArg.Split(':');
                string colorArgValue = colorArgParts[1];
                OrbColor resultColor;
                if (OrbColor.TryParse(colorArgValue, true, out resultColor))
                {
                    Color = resultColor;
                }
                else
                {
                    throw new ArgumentsParseException("You must specify color!");
                }
                string serverIpArg = args.FirstOrDefault(x => x.StartsWith("/startClient:", true, CultureInfo.CurrentCulture));
                if (serverIpArg != null)
                {
                    ServerIp = GetServerIp(serverIpArg);
                }
                PortNumber = GetPort(args);
            }
            else
            {
                string serverIpArg = args.FirstOrDefault(x => x.StartsWith("/startServer:", true, CultureInfo.CurrentCulture));
                ServerIp = GetServerIp(serverIpArg);
                PortNumber = GetPort(args);
            }            
        }


        private string GetServerIp(string serverIpArg)
        {
            var serverIpArgParts = serverIpArg.Split(':');
            var serverIpArgValue = serverIpArgParts[1];
            if (!Regex.IsMatch(serverIpArgValue, IpAddressRegexMask))
            {
                throw new ArgumentsParseException("ServerIp '{0}' does not match to ip address", serverIpArgValue);
            }
            return serverIpArgValue;
        }


        private int GetPort(string[] args)
        {
            string portArg = args.FirstOrDefault(x => x.StartsWith("/port:", true, CultureInfo.CurrentCulture));
            if (portArg == null)
            {
                return DefaultPortNumber;
            }

            string portArgValue = portArg.Split(':')[1];
            int portNum;
            if (int.TryParse(portArgValue, out portNum))
            {
                return portNum;
            }
            else
            {
                return DefaultPortNumber;
            }
        }

        private const short DefaultPortNumber = 8888;

        private const string IpAddressRegexMask =
            @"(?<First>2[0-4]\d|25[0-5]|[01]?\d\d?)\.(?<Second>2[0-4]\d|25[0-5]|[01]?\d\d?)\.(?<Third>2[0-4]\d|25[0-5]|[01]?\d\d?)\.(?<Fourth>2[0-4]\d|25[0-5]|[01]?\d\d?)";

        private const string ArgumentsUsage = @"
OrbManager - console tool to set color on build orb device connected via usb
Parameters:
/color:col (red | green | yellow | blue | white) - Start locally and set color on device
/port:xxxx - set port to listen
/startServer:ServerIpAddress - Start server and listen port 8888 for passed ip
/startClient:ServerIpAddress - Start Client and connect to server and set color on server side

Examples:
'OrbManager /color:red'
'OrbManager /startServer:192.168.0.1'
'OrbManager.exe /startClient:192.168.0.1 /color:yellow'
";

        public int PortNumber { get; private set; }
        public string ServerIp { get; private set; }
        public bool IsClientMode { get; private set; }
        public bool IsLocalMode { get; private set; }
        public OrbColor Color { get; private set; }
    }


}
