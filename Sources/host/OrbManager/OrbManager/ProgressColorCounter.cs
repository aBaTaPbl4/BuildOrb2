using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace OrbManager
{
    public class ProgressColorCounter : IProgressColorCounter
    {
        private readonly OrbColor _progressColor;
        private readonly OrbColor _successColor;
        private Queue<DateTime> _receivedProgressColorDates;
        private OrbColor _curColor;
        private OrbColor _lastNotProgressColor;
        private TimeSpan _avgBuildDuration;
        public ProgressColorCounter(OrbColor progressColor, OrbColor successColor, TimeSpan avgBuildDuration)
        {
            _progressColor = progressColor;
            _successColor = successColor;
            _curColor = progressColor;
            _receivedProgressColorDates = new Queue<DateTime>();
            _avgBuildDuration = avgBuildDuration;
        }

        public ProgressColorCounter(OrbColor progressColor, OrbColor successColor) : this(progressColor, successColor, TimeSpan.FromHours(1))
        {            
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
                _lastNotProgressColor = color;
                if (_receivedProgressColorDates.Count > 0)
                {
                    DateTime buidStarted = _receivedProgressColorDates.Dequeue();
                    if (color == _successColor)
                    {
                        _avgBuildDuration = DateTime.Now - buidStarted;
                        _avgBuildDuration += TimeSpan.FromMinutes(5);
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
            if (_receivedProgressColorDates.Count == 0)
            {
                return;
            }
            while (_receivedProgressColorDates.Count > 0 && (DateTime.Now - _receivedProgressColorDates.Peek() > _avgBuildDuration))
            {
                _receivedProgressColorDates.Dequeue();                
            }
        }

        public OrbColor GetCurrentColor()
        {
            DequeExpiredProgressColors();
            if (_receivedProgressColorDates.Count == 0 && _curColor == _progressColor)
            {
                _curColor = _lastNotProgressColor;
            }
            return _curColor;
        }
    }
}
