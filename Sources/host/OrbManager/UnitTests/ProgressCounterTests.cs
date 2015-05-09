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

        [Test]
        public void Test5()
        {
            _counter.ProcessColor(OrbColor.Blue);
            Thread.Sleep(401);
            CurrentColorCheck(OrbColor.Red);
        }

        [Test]
        public void Test6()
        {
            //	Сначала стартуем успешный билд…пошел билд процесс. Затем стартует еще один билд. Через минуту он падает. Еще через 2 минуты завершается саксесный билд успехом(так как в исходниках, на момент его старта, не было кривого чекина). В итоге статус билда будет светится зеленым, что не есть гуд.
            _counter.ProcessColor(OrbColor.Blue, "builder1");
            Thread.Sleep(20);
            _counter.ProcessColor(OrbColor.Blue, "builder2");
            Thread.Sleep(20);
            _counter.ProcessColor(OrbColor.Red, "builder2");
            Thread.Sleep(20);
            _counter.ProcessColor(OrbColor.Green, "builder1");
            CurrentColorCheck(OrbColor.Red);
        }

        [Test]
        public void BBGGFrom_Different_BuildersTest()
        {
            //•	Сценарии синий, синии, зеленый, зеленый. Зеленый должен загореться только после второго получения зеленого.
            _counter.ProcessColor(OrbColor.Blue, "builder1");
            Thread.Sleep(20);

            _counter.ProcessColor(OrbColor.Blue, "builder2");
            Thread.Sleep(20);

            _counter.ProcessColor(OrbColor.Green, "builder1");
            Thread.Sleep(10);

            CurrentColorCheck(OrbColor.Blue);

            _counter.ProcessColor(OrbColor.Green, "builder2");

            CurrentColorCheck(OrbColor.Green);
        }

        [Test]
        public void BGBBGGFrom_Different_BuildersTest()
        {
            //•	Сценарии синий, синии, зеленый, зеленый. Зеленый должен загореться только после второго получения зеленого.
            _counter.ProcessColor(OrbColor.Blue, "builder1");
            Thread.Sleep(20);

            _counter.ProcessColor(OrbColor.Green, "builder1");
            Thread.Sleep(20);

            _counter.ProcessColor(OrbColor.Blue, "builder1");
            Thread.Sleep(20);

            _counter.ProcessColor(OrbColor.Blue, "builder2");
            Thread.Sleep(20);

            _counter.ProcessColor(OrbColor.Green, "builder1");

            CurrentColorCheck(OrbColor.Blue);

            _counter.ProcessColor(OrbColor.Green, "builder2");

            CurrentColorCheck(OrbColor.Green);
        }

        [Test]
        public void BBGGFrom_Unknown_BuildersTest()
        {
            //•	Сценарии синий, синии, зеленый, зеленый. Зеленый должен загореться только после второго получения зеленого.
            _counter.ProcessColor(OrbColor.Blue);
            Thread.Sleep(20);

            _counter.ProcessColor(OrbColor.Blue);
            Thread.Sleep(20);

            _counter.ProcessColor(OrbColor.Green);
            Thread.Sleep(10);

            CurrentColorCheck(OrbColor.Blue);

            _counter.ProcessColor(OrbColor.Green);

            CurrentColorCheck(OrbColor.Green);
        }


        private void CurrentColorCheck(OrbColor expectedColor)
        {
            Assert.AreEqual(expectedColor, _counter.GetCurrentColor());
        }
    }
}
