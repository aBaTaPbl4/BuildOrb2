using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OrbManager
{
    class ProgressColorCounter : IProgressColorCounter
    {
        private readonly OrbColor _progressColor;
        private readonly OrbColor _successColor;
        private Queue<DateTime> _receivedProgressColorDates;
        private OrbColor _curColor;
        private TimeSpan _avgBuildDuration;
        public ProgressColorCounter(OrbColor progressColor, OrbColor successColor)
        {
            _progressColor = progressColor;
            _successColor = successColor;
            _curColor = progressColor;
            _receivedProgressColorDates = new Queue<DateTime>();
            _avgBuildDuration = TimeSpan.FromHours(1);
        }

        public void ProcessColor(OrbColor color)
        {
            if (color == _progressColor)
            {
                _receivedProgressColorDates.Enqueue(DateTime.Now);
                _curColor = _progressColor;
            }
            else
            {
                if (_receivedProgressColorDates.Count > 0)
                {
                    DateTime buidStarted = _receivedProgressColorDates.Dequeue();
                    if (color == _successColor)
                    {
                        _avgBuildDuration = DateTime.Now - buidStarted;    
                    }                    
                    DequeExpiredProgressColors();
                }
                
                if (_receivedProgressColorDates.Count == 0)
                {
                    _curColor = color;
                }
            }
        }

        private void DequeExpiredProgressColors()
        {
            while (DateTime.Now - _receivedProgressColorDates.Peek() > _avgBuildDuration)
            {
                _receivedProgressColorDates.Dequeue();
            }
        }

        public OrbColor GetCurrentColor()
        {
            return _curColor;
        }
    }
}
