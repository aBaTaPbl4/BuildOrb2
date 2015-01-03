using System.Runtime.InteropServices;
using System;

class call_led {
	
	[DllImport("led.dll")]
	private static extern int led_get();
	[DllImport("led.dll")]
	private static extern void led_set( int isOn );
	
	public static void Main(string[] args)
	{
	    if (args.Length == 0)
	    {
	        Console.WriteLine("No arguments passed");
	        return;
	    }
	    Int32 isLedOn = Int32.Parse(args[0]);
		led_set(isLedOn);
		Console.WriteLine("led is {0}", led_get());
	}
}
