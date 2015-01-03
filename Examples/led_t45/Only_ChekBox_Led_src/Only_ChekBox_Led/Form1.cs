using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;

namespace WindowsFormsApplication1
{
    public partial class Form1 : Form
    {
        int led_is_set;

        [DllImport("led.dll")]
        private static extern int led_get();
        [DllImport("led.dll")]
        private static extern void led_set(int isOn);

        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            led_is_set = led_get();
            if (led_is_set == 0 || led_is_set == 1 )
            {
                checkBoxLed.Enabled = true;
                if (led_is_set == 0)
                {
                    checkBoxLed.Text = "Led is off";
                    checkBoxLed.Checked = false;
                }
                if (led_is_set == 1)
                {
                    checkBoxLed.Text = "Led is on";
                    checkBoxLed.Checked = true;
                }
            }
            else
            {
                checkBoxLed.Text = "No Connect";
                checkBoxLed.Enabled = false;
            }
        }

        private void checkBoxLed_CheckedChanged(object sender, EventArgs e)
        {
            led_is_set = led_get();
            if (led_is_set == 0 || led_is_set == 1)
            {
                if (!checkBoxLed.Enabled) checkBoxLed.Enabled = true;
                if (checkBoxLed.Checked)
                {
                    checkBoxLed.Text = "Led is on";
                    checkBoxLed.Checked = true;
                    led_set(1);
                }
                else
                {
                    checkBoxLed.Text = "Led is off";
                    checkBoxLed.Checked = false;
                    led_set(0);
                }
            }
            else
            {
                checkBoxLed.Text = "No Connect";
                checkBoxLed.Enabled = false;
            }
        }
    }
}
