using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;

namespace BuildOrbManagerTask.Activities
{
    class InteractiveProcess
    {
        private readonly string _exePath;
        private readonly string _args;
        private StringBuilder _output;
        private int _outputLineNum;
        private Process _process;

        public InteractiveProcess(string exePath, string args)
        {
            _exePath = exePath;
            _args = args;
            _output = new StringBuilder();
            _outputLineNum = 0;
        }

        public string Output
        {
            get { return _output.ToString(); }
        }

        public bool Start()
        {
            _process = new Process();
            _process.StartInfo.FileName = _exePath;
            _process.StartInfo.UseShellExecute = false;
            _process.StartInfo.RedirectStandardOutput = true;
            _process.StartInfo.RedirectStandardInput = true;
            _process.StartInfo.Arguments = _args;
            _process.OutputDataReceived += new DataReceivedEventHandler(OutputHandler);
            return _process.Start();
        }

        public void WaitForExit()
        {
            _process.WaitForExit();
        }

        private void OutputHandler(object sendingProcess, DataReceivedEventArgs outLine)
        {
            // Collect the sort command output. 
            if (!String.IsNullOrEmpty(outLine.Data))
            {
                _outputLineNum++;

                // Add the text to the collected output.
                _output.Append(Environment.NewLine +
                    "[" + _outputLineNum.ToString() + "] - " + outLine.Data);
            }
        }
    }
}
