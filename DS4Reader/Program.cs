using System;
using System.IO.Ports;
using SharpDX.XInput;

namespace DS4Reader
{
    class Program
    {
        static void Main(string[] args)
        {
            Controller ds4 = new Controller(UserIndex.One);

            SerialPort serialPort = new SerialPort("COM4", 9600); // com4 = serial port where arduino is connected
            serialPort.Open();

            while (true)
            {
                State state = ds4.GetState();
                byte leftStickX =(byte) state.Gamepad.LeftThumbX;
                byte leftStickY =(byte) state.Gamepad.LeftThumbY;

                byte[] byteArray = { leftStickX,leftStickY};
                serialPort.Write(byteArray, 0, 2);

                Console.WriteLine(leftStickX);
                Console.WriteLine(leftStickY);
            }
        }
    }
}
