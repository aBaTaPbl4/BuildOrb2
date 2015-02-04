using System;
using System.Threading;
using NUnit.Framework;
using OrbManager;

namespace UnitTests
{
    [TestFixture]
    public class ProgressCounterTests
    {

        private ProgressColorCounter _counter;

        [SetUp]
        public void Setup()
        {
            _counter = new ProgressColorCounter(OrbColor.Blue, OrbColor.Green, TimeSpan.FromMilliseconds(
                200
//                2000000
                ));
        }

        [Test]
        public void Test1()
        {
            _counter.ProcessColor(OrbColor.Blue);
            CurrentColorCheck(OrbColor.Blue);
            _counter.ProcessColor(OrbColor.Green);
            CurrentColorCheck(OrbColor.Green);
        }

        [Test]
        public void Test2()
        {
            _counter.ProcessColor(OrbColor.Blue);
            _counter.ProcessColor(OrbColor.Blue);
            _counter.ProcessColor(OrbColor.Green);
            CurrentColorCheck(OrbColor.Blue);
            _counter.ProcessColor(OrbColor.Green);
            CurrentColorCheck(OrbColor.Green);
        }

        [Test]
        public void Test3()
        {
            _counter.ProcessColor(OrbColor.Blue);
            _counter.ProcessColor(OrbColor.Blue);
            _counter.ProcessColor(OrbColor.Green);
            CurrentColorCheck(OrbColor.Blue);
            _counter.ProcessColor(OrbColor.Red);
            CurrentColorCheck(OrbColor.Red);
        }

        [Test]
        public void Test4()
        {
            _counter.ProcessColor(OrbColor.Blue);
            _counter.ProcessColor(OrbColor.Blue);
            _counter.ProcessColor(OrbColor.Blue);
            _counter.ProcessColor(OrbColor.Red);
            Thread.Sleep(401);
            CurrentColorCheck(OrbColor.Red);
        }

        private void CurrentColorCheck(OrbColor expectedColor)
        {
            Assert.AreEqual(expectedColor, _counter.GetCurrentColor());
        }
    }
}
