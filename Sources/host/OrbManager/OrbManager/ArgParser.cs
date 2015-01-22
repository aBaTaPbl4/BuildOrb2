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
        None
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
            Color = OrbColor.None;
            CountProgressColors = false;
            ProgressColorForServer = OrbColor.Blue;
            SuccessColorForServer = OrbColor.Green;
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
                    IsLocalMode = false;
                }
                PortNumber = GetPort(args);
            }
            else
            {
                string serverIpArg = args.FirstOrDefault(x => x.StartsWith("/startServer:", true, CultureInfo.CurrentCulture));
                ServerIp = GetServerIp(serverIpArg);
                PortNumber = GetPort(args);
                if (args.Any(x => x.StartsWith("/countProgessColors", true, CultureInfo.CurrentCulture)))
                {
                    CountProgressColors = true;
                }

                //reading progress color
                string progressColorArg = args.FirstOrDefault(x => x.StartsWith("/progressColor:", true, CultureInfo.CurrentCulture));
                if (progressColorArg != null)
                {

                    string progressColorValue = progressColorArg.Split(':')[1];
                    OrbColor resultColor;
                    if (OrbColor.TryParse(progressColorValue, true, out resultColor))
                    {
                        ProgressColorForServer = resultColor;
                    }
                }

                //reading success color
                string successColorArg = args.FirstOrDefault(x => x.StartsWith("/successColor:", true, CultureInfo.CurrentCulture));
                if (successColorArg != null)
                {

                    string successColorValue = successColorArg.Split(':')[1];
                    OrbColor resultColor;
                    if (OrbColor.TryParse(successColorValue, true, out resultColor))
                    {
                        SuccessColorForServer = resultColor;
                    }
                }

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
/countProgessColors - Key is required if you have several build servers (in tfs words - build agents), which sends commands to orb
/progressColor - default value is blue. Use with countProgessColors key, to determine 'build is processing' color for orb server, is used by your company
/successColor  - default value is green. Use with countProgessColors key, to determine 'build is succeeded' color for orb server, is used by your company

Examples:
'OrbManager /color:red'
'OrbManager /startServer:192.168.0.1'
'OrbManager /startServer:192.168.0.1' /countProgressColors /progressColor:white /successColor:red
'OrbManager.exe /startClient:192.168.0.1 /color:white'
";

        public int PortNumber { get; private set; }
        public string ServerIp { get; private set; }
        public bool IsClientMode { get; private set; }
        public bool IsLocalMode { get; private set; }

        public bool IsRemoteClientMode
        {
            get { return IsClientMode && !IsLocalMode; }
        }

        public bool CountProgressColors { get; private set; }

        public OrbColor ProgressColorForServer { get; private set; }
        public OrbColor SuccessColorForServer { get; private set; }

        public OrbColor Color { get; private set; }
    }


}
