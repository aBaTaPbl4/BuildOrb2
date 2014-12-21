using System.Runtime.InteropServices;
using System;

class call_led {
	
	[DllImport("led.dll")]
	private static extern int led_get();
	[DllImport("led.dll")]
	private static extern void led_set( int isOn );
	
	public static void Main()
	{
		led_set(0);
		Console.WriteLine("led is {0}", led_get());
	}
}
