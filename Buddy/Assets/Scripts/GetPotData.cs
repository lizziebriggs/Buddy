using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.IO.Ports;
using System.Threading;
using UnityEngine.UI;

public class GetPotData : MonoBehaviour
{
    public static SerialPort serialPort = new SerialPort("COM12", 9600);
    Thread readThread = new Thread(ReadData);
    static bool checking = true;

    public static int lightVal, moistVal, tempVal;

    // Start is called before the first frame update
    void Start()
    {
        OpenConnection();
        readThread.Start();
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    private void OnApplicationQuit()
    {
        checking = false;
    }

    void OpenConnection()
    {
        if(serialPort != null)
        {
            if(serialPort.IsOpen)
                serialPort.Close();

            else
            {
                serialPort.Open();
                serialPort.ReadTimeout = 16;
            }
        }
    }

    public static void ReadData()
    {
        serialPort.ReadLine();

        while (checking)
        {
            try
            {
                string data = serialPort.ReadLine();

                char[] seperator = { ',', ' ' };
                string[] values = data.Split(seperator, 3);

                lightVal = int.Parse(values[0]);
                moistVal = int.Parse(values[1]);
                tempVal = int.Parse(values[2]);
            }

            catch (System.TimeoutException) { }
        }
    }

    public void SendData(string data)
    {
        serialPort.Write(data);
    }

}
