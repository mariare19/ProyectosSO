using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Newtonsoft.Json;
namespace Mensaje
{
    public class Mail : MarshalByRefObject
    {
        public static Dictionary<string, string> sender = new Dictionary<string, string>();
        public static Dictionary<string, string> Connected = new Dictionary<string, string>();
        public  static List<string> mensajesRecibidos = new List<string>();
        public string Directorios(string path)
        {
            string mensaje = "";
            if (Directory.Exists(path))
            {
                DirectoryInfo directory = new DirectoryInfo(path);
                DirectoryInfo[] directories = directory.GetDirectories();
                for (int i = 0; i < directories.Length; i++)
                {
                    mensaje += (i+1).ToString()+"-"+ directories[i] + "\n";
                }
            }
            else
            {
                mensaje = "error en el path";
            }

            return mensaje;
        }

        public int Agregarsuscriptor(string usuario, string ip, string puerto)
        {
            bool agregar = true;
            int numero = 0;
            if (!Connected.ContainsKey(usuario))
            {
                foreach  (KeyValuePair<string, string> item in Connected)
                {
                    string[] values = item.Value.Split(',');
                    if (values[0].Equals(ip))
                    {
                        agregar = false;
                        numero = 0;
                        break;
                    }
                }
                if (agregar == true )
                {
                    Connected.Add(usuario, ip + "," + puerto);
                    numero = 1;
                }
            }
            else
            {
                numero = 2;
            }
            return numero;
        }
        //Elimina al cliente que estan conectados a mi chat
        public void EliminarSuscriptor(string usuario)
        {
            sender.Remove(usuario);
            //bool mensaje = false;
            //if (Connected.ContainsKey(usuario))
            //{
            //    sender.Remove(usuario);
            //    mensaje = true;
            //}
            //else
            //{
            //    mensaje = false;
            //}

           // return mensaje;
        }
        public void SendMesaje(string usuario, string message)
        {
            mensajesRecibidos.Add(usuario+":"+message);
        }

        public void EliminarConnected(string value)
        {
            Connected.Remove(value);

        }
    }
}
