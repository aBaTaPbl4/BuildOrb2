using System;
using System.Collections.Generic;
using System.Security;

namespace OrbManager
{

    public class SenderInfos : Dictionary<string, SenderInfo>
    {
        private readonly ProgressColorCounter _counter;

        public SenderInfos(ProgressColorCounter counter)
        {
            _counter = counter;
        }

        public SenderInfo GetInfo(string sender)
        {
            if (!ContainsKey(sender))
            {
                Add(sender, new SenderInfo(sender, _counter));
            }
            return this[sender];
        }

        public int OrdersCount
        {
            get
            {
                int count = 0;
                foreach (var senderInfo in Values)
                {
                    count += senderInfo.ProgressQueue.Count;
                }
                return count;
            }
        }
    }

    public class SenderInfo
    {
        private readonly string _sender;
        private readonly ProgressColorCounter _counter;

        public SenderInfo(string sender, ProgressColorCounter counter)
        {
            _sender = sender;
            _counter = counter;
            ProgressQueue = new Queue<DateTime>();
        }

        public string SenderName
        {
            get { return _sender; }
        }
        public Queue<DateTime> ProgressQueue { get; private set; }

        public DateTime GetFirstBuildTimeStarted()
        {
            if (ProgressQueue.Count > 0)
            {
                return ProgressQueue.Peek();
            }
            return DateTime.Now - _counter.AvgBuildDuration;
        }
    }

    public class ColorInfo
    {
        public ColorInfo(OrbColor color, DateTime buildStartTime)
        {
            Color = color;
            BuildStartTime = buildStartTime;
        }

        public OrbColor Color { get; set; }
        public DateTime BuildStartTime { get; set; }
    }
    public class ProgressColorCounter : IProgressColorCounter
    {
        private readonly OrbColor _progressColor;
        private readonly OrbColor _successColor;

        private SenderInfos _senders;

        private ColorInfo _curColor;
        private ColorInfo _lastNotProgressColor;
        private TimeSpan _avgBuildDuration;
        public ProgressColorCounter(OrbColor progressColor, OrbColor successColor, TimeSpan avgBuildDuration)
        {
            _lastNotProgressColor = new ColorInfo(OrbColor.Red, DateTime.Now.AddSeconds(-5)); 

            _progressColor = progressColor;
            _successColor = successColor;
            _curColor = new ColorInfo(progressColor, DateTime.Now); 
            _senders = new SenderInfos(this);
            _avgBuildDuration = avgBuildDuration;
        }

        public TimeSpan AvgBuildDuration
        {
            get { return _avgBuildDuration; }
        }
        public ProgressColorCounter(OrbColor progressColor, OrbColor successColor) : this(progressColor, successColor, TimeSpan.FromHours(1))
        {            
        }

        public void ProcessColor(OrbColor color)
        {
            ProcessColor(color, "UnknownSender");
        }

        public void ProcessColor(OrbColor color, string sender)
        {
            SenderInfo senderInfo = _senders.GetInfo(sender);
            if (color == _progressColor)
            {
                DateTime buildStartTime = DateTime.Now;
                senderInfo.ProgressQueue.Enqueue(buildStartTime);
                _curColor = new ColorInfo(color, buildStartTime);
            }
            else
            {
                DateTime expectedBuildStartedTime = senderInfo.GetFirstBuildTimeStarted();
                if (_senders.OrdersCount > 0)
                {
                    DateTime buidStarted = senderInfo.ProgressQueue.Dequeue();
                    if (color == _successColor)
                    {
                        _avgBuildDuration = DateTime.Now - buidStarted;
                        _avgBuildDuration += TimeSpan.FromMinutes(5);
                    }                    
                    DequeExpiredProgressColors();
                }
                
                if (_senders.OrdersCount == 0)
                {
                    if (expectedBuildStartedTime.Ticks >= _lastNotProgressColor.BuildStartTime.Ticks)
                    {
                        _curColor = new ColorInfo(color, expectedBuildStartedTime);
                    }
                    else
                    {
                        _curColor = _lastNotProgressColor;
                    }
                }
                _lastNotProgressColor = new ColorInfo(color, expectedBuildStartedTime);
            }
        }

        private void DequeExpiredProgressColors()
        {
            if (_senders.OrdersCount == 0)
            {
                return;
            }
            foreach (var sender in _senders.Values)
            {
                while (sender.ProgressQueue.Count > 0 && (DateTime.Now - sender.ProgressQueue.Peek() > _avgBuildDuration))
                {
                    sender.ProgressQueue.Dequeue();
                }                
            }

        }

        public OrbColor GetCurrentColor()
        {
            DequeExpiredProgressColors();
            if (_senders.OrdersCount == 0 && _curColor.Color == _progressColor)
            {
                _curColor = _lastNotProgressColor;
            }
            return _curColor.Color;
        }
    }
}
