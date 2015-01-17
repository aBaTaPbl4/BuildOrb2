using System;
using System.Collections.Generic;
using System.Activities;
using System.IO;
using Microsoft.TeamFoundation.Build.Client;
using Microsoft.TeamFoundation.Build.Workflow.Tracking;
using Microsoft.TeamFoundation.Build.Workflow.Activities;

namespace BuildOrbManagerTask.Activities
{

    enum EColor
    {
        None = 0,
        Red,
        Green,
        Yellow,
        Blue,        
        White
    }

     [BuildActivity(HostEnvironmentOption.All)]  
    public sealed class BuildOrbManager : CodeActivity
    {
        
        protected override void Execute(CodeActivityContext context)
        {
            LogMessage(context, "OrbManager activity started");
            IBuildDetail buildDetail = context.GetExtension<IBuildDetail>();
            if (buildDetail == null)
            {
                LogMessage(BuildMessageImportance.High, context, "Error: Failed to get BuildDetail argument.");
                return;
            }
                        
            string targetBuildDefinitionName = context.GetValue(this.TargetBuildDefinitionName);

            if (!string.Equals(buildDetail.BuildDefinition.Name, targetBuildDefinitionName, StringComparison.CurrentCultureIgnoreCase))
            {
                LogMessage(context, "Build orb supposed working for over build definition, than '{0}'. See BuildDefinitionName property for activity.  Exiting...", buildDetail.BuildDefinition.Name);
                return;
            }

            var curColor = DetectCurrentBuildColor(context);
            
            List<string> serverIps = context.GetValue(this.ServerIps);

            if (serverIps == null || serverIps.Count == 0)
            {
                LogMessage(context, "ServerIps list is empty. Exiting...");
                return;
            }

            string orbManagerToolPath = Path.Combine(context.GetValue(this.BuildOrbManagerFolder), @"OrbManager.exe");

            foreach (var serverIp in serverIps)
            {
                LogMessage(context, "Connecting to server {0}...", serverIp);
                string[] serverInfo = serverIp.Split(':');
                string servIp;
                string servPort = null;
                if (serverInfo.Length > 1)
                {
                    servIp= serverInfo[0];
                    servPort = serverInfo[1];
                }
                else
                {
                    servIp = serverIp;
                }
                string args = string.Format("/startClient:{0} /color:{1}", servIp, curColor);
                if (servPort != null)
                {
                    args = string.Format("{0} /port:{1}", args, servPort);
                }

                var orbClientProcess = new InteractiveProcess(orbManagerToolPath, args);
                orbClientProcess.Start();
                orbClientProcess.WaitForExit();
                LogMessage(context, "Orb client process output: {0}",  orbClientProcess.Output);
            }
        }

        private void LogMessage(CodeActivityContext context, string fmt, params object[] args)
        {
            LogMessage(BuildMessageImportance.Normal, context, fmt, args);
        }

        private void LogMessage(BuildMessageImportance logLevel, CodeActivityContext context, string fmt, params object[] args)
        {
            var msg = string.Format(fmt, args);
            LogMessage(logLevel, context, msg);
        }

        private void LogMessage(CodeActivityContext context, string message)
        {
            LogMessage(BuildMessageImportance.Normal, context, message);
        }

        private void LogMessage(BuildMessageImportance logLevel, CodeActivityContext context, string message)
        {
            BuildInformationRecord<BuildMessage> record =
             new BuildInformationRecord<BuildMessage>()
             {
                 Value = new BuildMessage()
                 {
                     Importance = logLevel,
                     Message = message,
                 },
             };

            context.Track(record);
        }

         private EColor GetColor(string colorPropertyValue, EColor defaultColor)
         {
             if (colorPropertyValue == null)
             {
                 return defaultColor;
             }
             EColor userColor;
             if (EColor.TryParse(colorPropertyValue, true, out userColor))
             {
                 return userColor;
             }
             else
             {
                 return defaultColor;
             }
         }

        private EColor DetectCurrentBuildColor(CodeActivityContext context)
        {
            IBuildDetail buildDetail = context.GetExtension<IBuildDetail>();
            EColor successColor = GetColor(context.GetValue(SuccessColor), EColor.Green);
            
            EColor compilationFailedColor = GetColor(context.GetValue(CompilationFailedColor), EColor.Red);
            EColor testsFailedColor = GetColor(context.GetValue(TestsFailedColor), EColor.Yellow);
            EColor buildRunningColor = GetColor(context.GetValue(BuildRunningColor), EColor.Blue);

            switch (buildDetail.CompilationStatus)
            {
                    case BuildPhaseStatus.Failed:
                        return compilationFailedColor;

                    case BuildPhaseStatus.Succeeded:
                        switch (buildDetail.TestStatus)
                        {
                                case BuildPhaseStatus.Failed:
                                    return testsFailedColor;
                                case BuildPhaseStatus.Succeeded:
                                    return successColor;
                                case BuildPhaseStatus.Unknown: //tests are running
                                    return buildRunningColor;
                        }
                        break;    
                    case BuildPhaseStatus.Unknown:
                        return buildRunningColor;
            }
            return EColor.None;
        }

        [RequiredArgument]
        public InArgument<string> BuildOrbManagerFolder { get; set; }
        [RequiredArgument]
        public InArgument<string> TargetBuildDefinitionName { get; set; }

        public InArgument<List<string>> ServerIps { get; set; }
        public InArgument<string> CompilationFailedColor{ get; set; }        
        public InArgument<string> TestsFailedColor { get; set; }
        public InArgument<string> SuccessColor { get; set; }
        public InArgument<string> BuildRunningColor { get; set; }


    }
}
