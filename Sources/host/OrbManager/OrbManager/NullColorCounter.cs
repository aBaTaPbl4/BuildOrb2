using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OrbManager
{
    class NullColorCounter : IProgressColorCounter
    {
        private OrbColor _currentColor = OrbColor.None;
        

        public void ProcessColor(OrbColor color)
        {
            _currentColor = color;
        }

        public OrbColor GetCurrentColor()
        {
            return _currentColor;
        }
    }
}
