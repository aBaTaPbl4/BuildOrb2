using System;
using System.Collections.Generic;
using System.Security;

namespace OrbManager
{

    public class BuilderInfos : Dictionary<string, BuilderInfo>
    {
        private readonly ProgressColorCounter _counter;

        public BuilderInfos(ProgressColorCounter counter)
        {
            _counter = counter;
        }

        public BuilderInfo GetInfo(string sender)
        {
            if (!ContainsKey(sender))
            {
                Add(sender, new BuilderInfo(sender, _counter));
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

    public class BuilderInfo
    {
        private readonly string _sender;
        private readonly ProgressColorCounter _counter;

        public BuilderInfo(string sender, ProgressColorCounter counter)
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
            return DateTime.Now - _counter.BuildTimeout;
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

        private BuilderInfos _builders;

        private ColorInfo _curColor;
        private ColorInfo _lastNotProgressColor;
        private TimeSpan _buildTimeout;
        public ProgressColorCounter(OrbColor progressColor, OrbColor successColor, TimeSpan buildTimeout)
        {
            _lastNotProgressColor = new ColorInfo(OrbColor.Red, DateTime.Now.AddSeconds(-5)); 

            _progressColor = progressColor;
            _successColor = successColor;
            _curColor = new ColorInfo(progressColor, DateTime.Now); 
            _builders = new BuilderInfos(this);
            _buildTimeout = buildTimeout;
        }

        public TimeSpan BuildTimeout
        {
            get { return _buildTimeout; }
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
            BuilderInfo builderInfo = _builders.GetInfo(sender);
            if (color == _progressColor)
            {
                DateTime buildStartTime = DateTime.Now;
                builderInfo.ProgressQueue.Enqueue(buildStartTime);
                _curColor = new ColorInfo(color, buildStartTime);
            }
            else
            {
                DateTime expectedBuildStartedTime = builderInfo.GetFirstBuildTimeStarted();
                if (_builders.OrdersCount > 0)
                {
                    DateTime buidStarted = builderInfo.ProgressQueue.Dequeue();
                    if (color == _successColor)
                    {
                        _buildTimeout = CalcBuildTimeout(buidStarted);
                    }                    
                    DequeExpiredProgressColors();
                }
                
                if (_builders.OrdersCount == 0)
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

        private TimeSpan CalcBuildTimeout(DateTime buidStarted)
        {
            var avgBuildDuration = DateTime.Now - buidStarted;
            avgBuildDuration = TimeSpan.FromSeconds(_buildTimeout.Seconds*2);
            avgBuildDuration += TimeSpan.FromMinutes(5);
            return avgBuildDuration;
        }

        private void DequeExpiredProgressColors()
        {
            if (_builders.OrdersCount == 0)
            {
                return;
            }
            foreach (var sender in _builders.Values)
            {
                while (sender.ProgressQueue.Count > 0 && (DateTime.Now - sender.ProgressQueue.Peek() > _buildTimeout))
                {
                    sender.ProgressQueue.Dequeue();
                }                
            }

        }

        public OrbColor GetCurrentColor()
        {
            DequeExpiredProgressColors();
            if (_builders.OrdersCount == 0 && _curColor.Color == _progressColor)
            {
                _curColor = _lastNotProgressColor;
            }
            return _curColor.Color;
        }
    }
}
