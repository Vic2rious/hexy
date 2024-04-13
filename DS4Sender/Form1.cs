using System;
using System.IO.Ports;
using SharpDX.XInput;
using System.Runtime.InteropServices;
using System.Security.Cryptography;
using System.Diagnostics;

namespace DS4Sender
{
    public partial class Form1 : Form
    {
        bool isTracking = false;
        Controller ds4 = new Controller(UserIndex.One);
        SerialPort serialPort = new SerialPort("COM4", 9600); // com4 = serial port where arduino is connected
        
      

        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            serialPort.Open();

            if (ds4.IsConnected)
            {
                label1.Text = "Controller is connected.";
                label1.BackColor = Color.FromArgb(128, 255, 128);

            }
            else
            {
                label1.Text = "No controller detected";
                label1.BackColor = Color.FromArgb(255, 128, 128);
            }


        }

        private void startButton_Click(object sender, EventArgs e)
        {
            if (label1.Text == "No controller detected")
            {
                MessageBox.Show("Connect a controller first!!", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            if (!isTracking)
            {
                isTracking = true;
                startButton.Text = "Stop tracking / Hold options on DS4";
                while (isTracking)
                {

                    SharpDX.XInput.State state = ds4.GetState();

                    if (state.Gamepad.Buttons == GamepadButtonFlags.Start)
                    {
                        startButton.Text = "Start Joystick Tracking";
                        isTracking = false;
                    }


                    float leftStickX = state.Gamepad.LeftThumbX;
                    float leftStickY = state.Gamepad.LeftThumbY;

                    float normalizedX = leftStickX / 32767f;
                    float normalizedY = leftStickY / 32767f;

                    double xPos = Math.Round(-normalizedX * 50, MidpointRounding.AwayFromZero);
                    double yPos = Math.Round(normalizedY * 50, MidpointRounding.AwayFromZero);
                    //Y = forward / backward
                    // X = left / right
                    // Z does not exist here, since we do not controll the height of the hexapod. (For now, at least)f


                    Debug.WriteLine(yPos);
                    //label2.Text = xPos.ToString();
                    serialPort.WriteLine(yPos.ToString());
                    Thread.Sleep(100);
                }
            }

        }

        private void controllerCheck_Click(object sender, EventArgs e)
        {
            Controller ds4 = new Controller(UserIndex.One);
            if (ds4.IsConnected)
            {
                label1.Text = "Controller is connected.";
                label1.BackColor = Color.FromArgb(128, 255, 128);

            }
            else
            {
                label1.Text = "No controller detected";
                label1.BackColor = Color.FromArgb(255, 128, 128);
            }
        }
    }
}
