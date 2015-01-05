using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OrbManager
{
    public class ColorToByteConverter
    {

        public byte Convert(OrbColor color)
        {
            switch (color)
            {
                    case OrbColor.Red:
                    return 1;

                    case OrbColor.Green:
                    return 2;

                    case OrbColor.Blue:
                    return 4;

                    case OrbColor.Yellow:
                    return 3;

                    case OrbColor.White:
                    return 7;
            }
            return 0;
        }
    }
}
