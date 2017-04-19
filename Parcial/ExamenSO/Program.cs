using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.Remoting.Channels.Tcp;
using System.Runtime.Remoting.Channels;
using System.Runtime.Remoting;
using System.IO;
using Mensaje;
using System.Threading;
using System.Net;

namespace ExamenSO
{

    class Program
    {
        static int puerto = 0;
        public static Mail remoteObject;
        public static Object lockObj;
        public static string mensagens;
        static void Main(string[] args)
        {
            bool correcto = false;
            lockObj = new object();
            do
            {
                try
                {
                    IniciarServer(ref correcto);
                    Console.WriteLine("Servidor Activo");
                }
                catch (Exception)
                {
                    Console.WriteLine("Error");
                }
            } while (correcto == false);
            IniciarCliente();
            Environment.Exit(0);

        }

        static void IniciarServer(ref bool bandera)
        {
            Console.WriteLine("Ingrese el numero de puerto");
            puerto = Convert.ToInt32(Console.ReadLine());

            TcpChannel ServerChannel = new TcpChannel(puerto);
            ChannelServices.RegisterChannel(ServerChannel,false);
            RemotingConfiguration.RegisterWellKnownServiceType(typeof(Mail), "clientServer", WellKnownObjectMode.Singleton);
            bandera = true;
        }

        static void IniciarCliente()
        {
            Type requiredType = typeof(Mail);
            new Thread(refreshMessages).Start();
            Console.WriteLine("Ingrese un comando para el chat");
            bool bandera = false;
            while (bandera == false)
            {
                try
                {
                    string comando = "";
                    comando = Console.ReadLine();
                    string[] datos = comando.Split(' ');
                    switch (datos[0].ToLower())
                    {
                        case "connect":
                            if (datos[1].Contains(":"))
                            {
                                string[] value = datos[1].Split(':');
                                try
                                {
                                    remoteObject = (Mail)Activator.GetObject(requiredType, "tcp://"+value[0]+":"+value[1]+"/clientServer");
                                    if (remoteObject == null)
                                    {
                                        Console.WriteLine("No se pudo localizar el servidor");
                                    }
                                    else
                                    {
                                        if (!Mail.sender.ContainsKey(datos[2].ToLower()))
                                        {
                                            switch (remoteObject.Agregarsuscriptor(datos[2].ToLower(), LocalIPAddress(), puerto.ToString()))
                                            {
                                                case 0:
                                                    Console.WriteLine("No se puede suscribir mas de una vez al mismo servidor");
                                                    break;
                                                case 1:
                                                    Mail.sender.Add(datos[2].ToLower(), value[0] + "," + value[1]);
                                                    Console.WriteLine("Cliente conectado con exito");
                                                    break;
                                                case 2:
                                                    Console.WriteLine("Ya existe un usuario con el mismo nombre en el servidor");
                                                    break;
                                            }
                                        }
                                        else
                                        {
                                            Console.WriteLine("Ya se ha conectado a un servidor con el mismo nombre");
                                        }
                                    }
                                }
                                catch (Exception)
                                {
                                    Console.WriteLine("Error al conectarse al servidor");
                                }
                            }
                            else
                            {
                                Console.WriteLine("Error se esperaba ':'");
                            }

                            break;
                        case "list":
                            if (datos[1].Equals("connected"))
                            {

                                if (Mensaje.Mail.Connected.Count != 0)
                                {
                                    foreach (KeyValuePair<string, string> item in Mensaje.Mail.Connected)
                                    {
                                        string[] value = item.Value.Split(',');
                                        Console.WriteLine(item.Key + ":" + value[0] + ":" + value[1]);
                                    }
                                }
                                else
                                {
                                    Console.WriteLine("Clientes conectados al chat: 0");
                                }

                            }
                            else if (datos[1].Equals("sender"))
                            {
                                if (Mensaje.Mail.sender.Count != 0)
                                {
                                    foreach (KeyValuePair<string, string> item in Mensaje.Mail.sender)
                                    {
                                        string[] value = item.Value.Split(',');
                                        Console.WriteLine(item.Key + ":" + value[0] + ":" + value[1]);
                                    }
                                }
                                else
                                {
                                    Console.WriteLine("Maquinas a las que estoy conectado: 0");
                                }
                            }
                            else if (datos[1].Equals("dir"))
                            {
                                if (Mensaje.Mail.sender.Count != 0)
                                {
                                    foreach (KeyValuePair<string, string> item in Mensaje.Mail.sender)
                                    {
                                        string[] value = item.Value.Split(',');
                                        try
                                        {

                                            remoteObject = (Mail)Activator.GetObject(requiredType, "tcp://"+value[0]+":"+value[1]+"/clientServer");
                                            if (remoteObject == null)
                                            {
                                                Console.WriteLine("Error al enviar el mensaje al cliente:" + value[0]);
                                            }
                                            else
                                            {
                                                Console.WriteLine(item.Key + ":");
                                                Console.WriteLine(remoteObject.Directorios(datos[2]));
                                            }
                                        }
                                        catch (Exception)
                                        {
                                            Console.WriteLine("Error con el servidor:" + value[0]);
                                        }
                                    }
                                }
                                else
                                {
                                    Console.WriteLine("Maquinas a las que estoy conectado: 0");
                                }
                            }
                            else
                            {
                                Console.WriteLine("Error en el comando");
                            }
                            break;
                        case "kill":
                            if (datos[1].ToLower().Equals("user"))
                            {
                                if (Mensaje.Mail.Connected.ContainsKey(datos[2]))
                                {
                                    string[] value = Mensaje.Mail.Connected[datos[2]].Split(',');
                                    Mensaje.Mail.Connected.Remove(datos[2]);
                                    try
                                    {
                                        remoteObject = (Mail)Activator.GetObject(requiredType, "tcp://"+value[0]+":"+value[1]+"/clientServer");
                                        remoteObject.EliminarSuscriptor(datos[2]);
                                        remoteObject.SendMesaje("El servidor " + LocalIPAddress() + "->" + puerto.ToString(), ":te ha eliminado y no podras envarile mas mensajes");
                                        Console.WriteLine("Se desconecto al cliente con exito");
                                    }
                                    catch (Exception)
                                    {
                                        Console.WriteLine("Error con el cliente: no se pudo eliminar al usuario ");
                                    }
                                }
                                else
                                {
                                    Console.WriteLine("Error: no se encontro al usuario");
                                }

                            }
                            else
                            {
                                Console.WriteLine("Error: se esperaba user");
                            }
                            break;
                        case "send":
                            if (Mensaje.Mail.sender.Count != 0)
                            {
                                comando = comando.Remove(0, 5);
                                foreach (KeyValuePair<string, string> item in Mensaje.Mail.sender)
                                {
                                    string[] value = item.Value.Split(',');
                                    try
                                    {
                                        remoteObject = (Mail)Activator.GetObject(requiredType, "tcp://"+value[0]+":"+value[1]+"/clientServer");
                                        remoteObject.SendMesaje(item.Key, comando);
                                        Console.WriteLine("Mensaje enviado a:" + value[0]);
                                    }
                                    catch (Exception)
                                    {
                                        Console.WriteLine("Error con el cliente:" + value[0]);
                                    }
                                }
                            }
                            else
                            {
                                Console.WriteLine("No hay servidores al que este conectado");
                            }

                            break;
                        case "exit":
                            foreach (KeyValuePair<string, string> item in Mensaje.Mail.Connected)
                            {
                                string[] value = item.Value.Split(',');
                                try
                                {
                                    remoteObject = (Mail)Activator.GetObject(requiredType, "tcp://"+value[0] +":"+value[1]+"/clientServer");
                                    remoteObject.EliminarSuscriptor(item.Key);
                                    remoteObject.SendMesaje("El servidor: " + LocalIPAddress() + "->" + puerto.ToString(), "Se ha desconectado");
                                }
                                catch (Exception)
                                {
                                    Console.WriteLine("Error con el cliente:" + value[0]);
                                }
                            }
                            foreach (KeyValuePair<string, string> item in Mensaje.Mail.sender)
                            {
                                string[] value = item.Value.Split(',');
                                try
                                {
                                    remoteObject = (Mail)Activator.GetObject(requiredType, "tcp://"+value[0]+":"+value[1]+"/clientServer");
                                    remoteObject.EliminarConnected(item.Key);
                                }
                                catch (Exception)
                                {
                                    Console.WriteLine("Error con el cliente:" + value[0]);
                                }
                            }
                            bandera = true;
                            break;
                        default:
                            Console.WriteLine("Error en el comando");
                            break;
                    }
                }
                catch (Exception)
                {
                    Console.WriteLine("Error en el comando");
                }

            }

        }
        static void refreshMessages()
        {
            while (true)
            {
                
                if (Mail.mensajesRecibidos.Count != 0)
                {
                    for (int i = 0; i < Mail.mensajesRecibidos.Count; i++)
                    {
                        Console.WriteLine(Mail.mensajesRecibidos[i]);
                    }
                    Mail.mensajesRecibidos.Clear();
                }
            }
        }
        public static string LocalIPAddress()
        {
            IPHostEntry host;
            string localIP = "";
            host = Dns.GetHostEntry(Dns.GetHostName());
            foreach (IPAddress ip in host.AddressList)
            {
                if (ip.AddressFamily.ToString() == "InterNetwork")
                {
                    localIP = ip.ToString();
                }
            }
            return localIP;
        }

    }
}
