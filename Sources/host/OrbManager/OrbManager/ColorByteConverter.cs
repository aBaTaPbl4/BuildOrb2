using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OrbManager
{
    public class ColorByteConverter
    {

        private Dictionary<byte, OrbColor> _colors;

        public ColorByteConverter()
        {
            _colors = new Dictionary<byte, OrbColor>();
            _colors.Add(1, OrbColor.Red);
            _colors.Add(2, OrbColor.Green);
            _colors.Add(4, OrbColor.Blue);
            _colors.Add(3, OrbColor.Yellow);
            _colors.Add(7, OrbColor.White);
            _colors.Add(0, OrbColor.None);
        }

        public byte Color2Byte(OrbColor color)
        {
            if (!_colors.ContainsValue(color))
            {
                return 0;    
            }

            var pair = _colors.Single(x => x.Value == color);
            return pair.Key;
        }

        public OrbColor Byte2Color(byte colorByte)
        {
            if (!_colors.ContainsKey(colorByte))
            {
                return OrbColor.None;
            }
            return _colors[colorByte];
        }
    }
}
