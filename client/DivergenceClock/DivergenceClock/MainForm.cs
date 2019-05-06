using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO.Ports;
using System.IO;
using System.Threading;

namespace DivergenceClock
{
    public partial class MainForm : Form
    {
        delegate void Action();
        SerialPort sp;
        byte[] buffer = new byte[100];

        public MainForm()
        {
            InitializeComponent();

            return;
            for (int i = 1; i < 30; i++)
            {
                try
                {
                    sp = new SerialPort("COM" + i, 19200, Parity.None, 8, StopBits.One);
                    sp.Open(); // IOException
                    sp.ReadTimeout = 200;
                    break;
                }
                catch (IOException)
                { continue; }
            }

        }

        void sp_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            try
            {
                int type = 0;
                do
                {
                    type = sp.ReadByte();
                } while (type == 0);
                int size = sp.ReadByte();
                int len = 0;
                while (len != size)
                    len += sp.Read(buffer, len, size);
                Action a;
                switch (type)
                {
                    case 0x43:
                        int light = buffer[0];
                        a = () => trackBar1.Value = light;
                        this.BeginInvoke(a);
                        break;
                    case 0x44:
                        int fade = buffer[0];
                        a = () => trackBar2.Value = fade;
                        this.BeginInvoke(a);
                        break;
                    case 0x45:
                        var alarm = new
                        {
                            enable = buffer[0] != 0,
                            min = buffer[1],
                            hour = buffer[2]
                        };
                        a = () =>
                        {
                            label1.Text = string.Format("{0} {1:X}:{2:X}", alarm.enable, alarm.hour, alarm.min);
                        };
                        this.BeginInvoke(a);
                        break;
                    case 0x46:
                        var sleepTime = new
                        {
                            enable = buffer[0] != 0,
                            startMin = buffer[1],
                            StartHour = buffer[2],
                            endMin = buffer[3],
                            endHour = buffer[4]
                        };
                        a = () =>
                        {
                            label2.Text = string.Format("{0} {1:X}:{2:X} to {3:X}:{4:X}", sleepTime.enable, sleepTime.StartHour, sleepTime.startMin, sleepTime.endHour, sleepTime.endMin);
                        };
                        this.BeginInvoke(a);
                        break;
                    default:
                        break;
                }
            }
            catch { }
        }

        private void btnAutoSetTime_Click(object sender, EventArgs e)
        {
            DateTime date = DateTime.Now.AddSeconds(1.0);
            buffer[0] = 0x01;
            buffer[1] = 7;
            buffer[2] = (byte)((date.Second / 10 << 4) | date.Second % 10);
            buffer[3] = (byte)((date.Minute / 10 << 4) | date.Minute % 10);
            buffer[4] = (byte)((date.Hour / 10 << 4) | date.Hour % 10);
            buffer[5] = (byte)(date.DayOfWeek == 0 ? 7 : (int)date.DayOfWeek);
            buffer[6] = (byte)((date.Day / 10 << 4) | date.Day % 10);
            buffer[7] = (byte)((date.Month / 10 << 4) | date.Month % 10);
            int year = date.Year % 100;
            buffer[8] = (byte)((year / 10 << 4) | year % 10);

            sp.Write(buffer, 0, 9);
        }

        private void btnTime_Click(object sender, EventArgs e)
        {
            buffer[0] = 0x20;
            buffer[1] = 0;
            sp.Write(buffer, 0, 2);
        }

        private void btnDate_Click(object sender, EventArgs e)
        {
            buffer[0] = 0x21;
            buffer[1] = 0;
            sp.Write(buffer, 0, 2);
        }

        private void btnStopwatch_Click(object sender, EventArgs e)
        {
            buffer[0] = 0x22;
            buffer[1] = 0;
            sp.Write(buffer, 0, 2);
        }

        private void btnWorldLine_Click(object sender, EventArgs e)
        {
            buffer[0] = 0x23;
            buffer[1] = 0;
            sp.Write(buffer, 0, 2);
        }

        private void btnMarquee_Click(object sender, EventArgs e)
        {
            buffer[0] = 0x24;
            buffer[1] = 0;
            sp.Write(buffer, 0, 2);
        }

        private void btnTemperature_Click(object sender, EventArgs e)
        {
            buffer[0] = 0x25;
            buffer[1] = 0;
            sp.Write(buffer, 0, 2);
        }

        private void btnButton1_Click(object sender, EventArgs e)
        {
            buffer[0] = 0x11;
            buffer[1] = 0;
            sp.Write(buffer, 0, 2);
        }

        private void btnButton2_Click(object sender, EventArgs e)
        {
            buffer[0] = 0x12;
            buffer[1] = 0;
            sp.Write(buffer, 0, 2);
        }

        private void btnButton3_Click(object sender, EventArgs e)
        {
            buffer[0] = 0x13;
            buffer[1] = 0;
            sp.Write(buffer, 0, 2);
        }

        private void trackBar1_ValueChanged(object sender, EventArgs e)
        {
            buffer[0] = 0x03;
            buffer[1] = 1;
            buffer[2] = (byte)trackBar1.Value;
            sp.Write(buffer, 0, 3);
        }

        private void trackBar2_ValueChanged(object sender, EventArgs e)
        {
            buffer[0] = 0x04;
            buffer[1] = 1;
            buffer[2] = (byte)trackBar2.Value;
            sp.Write(buffer, 0, 3);
        }

        private void button1_Click(object sender, EventArgs e)
        {
            new Thread(new ThreadStart(() =>
            {
                int startTime ,sleepTime, oldTime;
                startTime = oldTime = Environment.TickCount;
                for (int i = 0; ; i++)
                {
                    buffer[0] = 0x02;
                    buffer[1] = 16;
                    int v = i;
                    for (int j = 2; j < 10; j++)
                    {
                        buffer[j] = (byte)(v % 10);
                        v /= 10;
                    }
                    byte light = (byte)(i % 100);
                    if ((buffer[4] & 1) == 1) light = (byte)(100 - light);
                    for (int j = 10; j < 18; j++)
                        buffer[j] = light;
                    sp.Write(buffer, 0, 18);
                    sleepTime = (int)(40 - (Environment.TickCount - oldTime));
                    if (sleepTime > 0)
                        Thread.Sleep(sleepTime);
                    oldTime = startTime + i * 40;
                }
            })).Start();

        }

        private void button2_Click(object sender, EventArgs e)
        {
            sp = new SerialPort("COM" + textBox1.Text, 9600, Parity.None, 8, StopBits.One);
            sp.ReadTimeout = 200;
            sp.WriteTimeout = 200;
            sp.Open(); // IOException
            sp.DataReceived += new SerialDataReceivedEventHandler(sp_DataReceived);

            for (int i = 0; i < 6; i++)
                buffer[i] = 0;
            sp.Write(buffer, 0, 6);
            Thread.Sleep(100);

            buffer[0] = 0x43;
            buffer[1] = 0;
            sp.Write(buffer, 0, 2);
            Thread.Sleep(100);

            buffer[0] = 0x44;
            buffer[1] = 0;
            sp.Write(buffer, 0, 2);
            Thread.Sleep(100);

            buffer[0] = 0x45;
            buffer[1] = 0;
            sp.Write(buffer, 0, 2);
            Thread.Sleep(100);

            buffer[0] = 0x46;
            buffer[1] = 0;
            sp.Write(buffer, 0, 2);
        }

        private void button3_Click(object sender, EventArgs e)
        {
            buffer[0] = 0x06;
            buffer[1] = 5;
            buffer[2] = 1;
            buffer[3] = 0x00;
            buffer[4] = 0x09;
            buffer[5] = 0x00;
            buffer[6] = 0x16;

            sp.Write(buffer, 0, 7);
            Thread.Sleep(100);

            buffer[0] = 0x46;
            buffer[1] = 0;
            sp.Write(buffer, 0, 2);
            Thread.Sleep(100);


            buffer[0] = 0x05;
            buffer[1] = 3;
            buffer[2] = 1;
            buffer[3] = 0x30;
            buffer[4] = 0x06;

            sp.Write(buffer, 0, 5);
            Thread.Sleep(100);

            buffer[0] = 0x45;
            buffer[1] = 0;
            sp.Write(buffer, 0, 2);
        }

        private void timerAutoSetTime_Tick(object sender, EventArgs e)
        {
            btnAutoSetTime_Click(sender, e);
        }
    }
}
