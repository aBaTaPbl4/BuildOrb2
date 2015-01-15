using System;
using System.Collections.Generic;
using System.Activities;
using System.IO;
using Microsoft.TeamFoundation.Build.Client;

namespace BuildOrbManagerTask.Activities
{

    enum EColor
    {
        Red,
        Green,
        Yellow,
        Blue,        
        White,
        Unknown
    }

     [BuildActivity(HostEnvironmentOption.All)]  
    public sealed class BuildOrbManager : CodeActivity
    {
        
        protected override void Execute(CodeActivityContext context)
        {
            IBuildDefinition buildDefinition = context.GetExtension<IBuildDefinition>();
            string targetBuildDefinitionName = context.GetValue(this.TargetBuildDefinitionName);

            if (!string.Equals(buildDefinition.Name, targetBuildDefinitionName, StringComparison.CurrentCultureIgnoreCase))
            {
                return;
            }

            var curColor = DetectCurrentBuildColor(context);
            if (curColor == EColor.Unknown)
            {
                return;
            }
            
            List<string> serverIps = context.GetValue(this.ServerIps);

            if (serverIps == null || serverIps.Count == 0)
            {
                return;
            }

            string orbManagerToolPath = Path.Combine(context.GetValue(this.BuildOrbManagerFolder), @"OrbManager.exe");

            foreach (var serverIp in serverIps)
            {
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
                context.SetValue<string>(OrbClientOutput, orbClientProcess.Output);
            }
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
            return EColor.Unknown;
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
        public OutArgument<string> OrbClientOutput { get; set; }


    }
}
