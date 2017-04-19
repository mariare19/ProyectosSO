//MOUSE.CPP
#include <stdio.h>

#include <dos.h>

#include <conio.h>

#include <string.h>

#include <time.h>

#define INTR 0X1C    /* The clock tick interrupt */


#ifdef __cplusplus

    #define __CPPARGS ...

#else

    #define __CPPARGS

#endif

void interrupt (*oldhandler)(__CPPARGS);
void PrintPantalla ();
int BotonPulsado(void);

void Comandos_pantalla();
void Comando_Add(int proc);
void Comando_Pause(int proc);
void Comando_Clear(int proc);
void Comando_Quantum(int proc);
void Comando_Stats(int proc);
void Comando_Remove(int proc);
void Validar_Proc(int cmd, int proc);
void Guardar_PuntosPantalla();
void Validar_ComandosPantalla();
int ConvertirNum();
void Limpiar_Matriz();
int CorHTexto(void);
int CorVTexto(void);

#define X_in 8
#define Y_in 24
#define X_out 10
#define Y_out 26

union REGS reg;
int count=0;
int Quamtum = 1;
int x=0;
int y=0;
int matriz_x =0;
int matriz_y =0;
int indice = -1;
int pulsaciones = 0;
char Cadena[100];
int contador_Candena=0;
int recorrido_Cadena=0;
int Error=0;
clock_t T_total;


struct Coordenadas
    {
        int x;
        int y;
        int estatus;
        Coordenadas(){estatus = 0;}
    } ;

class PCB {
   public:
      int estado;
      int Quamtum_PCB;
      int id;
      int fila;
      int Columna;
      int opcion;
      long tiempo;
      Coordenadas Matriz_puntos[10][25];
      PCB () {
        Quamtum_PCB = 1;
        opcion = 1;
        estado = 0;
        fila = 0;
        Columna = 0;
        tiempo=0;
      }
      void GuardarPunto(int pox, int poy, int f1_f2, int proc);
      void PCB2()
      {
        Quamtum_PCB = 1;
        opcion = 1;
        estado = 0;
        fila = 0;
        Columna = 0;
        tiempo=0;
      }
};


void PCB::GuardarPunto(int pox, int poy, int f1_f2, int proc)
{
    //calular el indice x
    //si es fila superior
    if(f1_f2 == 1)
    {
      matriz_y = poy-1;
    }
    //Si es fila inferior
    else
    {
      matriz_y = poy -12;
    }

    //calular el indice y
    if(proc ==1 || proc ==4)
    {
      matriz_x = pox-1;
    }
    else if(proc ==2 || proc ==5) 
    {
      matriz_x = pox-27;
    }
    else if(proc==3|| proc ==6)
    {
      matriz_x = pox-53;
    }
    matriz_x = matriz_x-1;
    matriz_y = matriz_y-1;

    Matriz_puntos[matriz_y][matriz_x].x = pox;
    Matriz_puntos[matriz_y][matriz_x].y = poy;
    Matriz_puntos[matriz_y][matriz_x].estatus =1;
    
}

PCB VecPcb[6];
struct Coordenadas *puntero = NULL;

void Guardar_PuntosPantalla()
{
  
      //reg.x.ax=0x3;
      //int86(0x33, &reg, &reg);
      //if(reg.x.bx==1)
        {
                x =CorHTexto();
                y =CorVTexto();
                /*proceso1*/
            if (x <= 26 && x >= 2 && y <= 11 && y >=2)
            {
              if(VecPcb[0].estado ==1)
              {
                VecPcb[0].GuardarPunto(x,y,1,1);
              }
              
            }

            /*proceso2*/
            else if (x <= 52 && x >= 28 && y <= 11 && y >= 2)
            {
              if(VecPcb[1].estado ==1)
              {
                VecPcb[1].GuardarPunto(x,y,1,2);
              }
            }

            /*proceso3*/
            else if (x <= 78 && x >= 54 && y <= 11 && y>=2)
            {
              if(VecPcb[2].estado ==1)
              {
                VecPcb[2].GuardarPunto(x,y,1,3);
              }
            }

            /*proceso4*/
            else if ((x >= 2 && x <= 26) && (y >= 13 && y <= 22))
            {
              if(VecPcb[3].estado ==1)
              {
                VecPcb[3].GuardarPunto(x,y,2,4);
              }
            }

            /*proceso5*/
            else if ((x >= 28 && x <= 52) && (y >= 13 && y <= 22))
            {
              if(VecPcb[4].estado ==1)
              {
                VecPcb[4].GuardarPunto(x,y,2,5);
              }
            }

            /*proceso6*/
            else if ((x >= 54 && x <= 78) && (y >= 13 && y<=22))
            {
              if(VecPcb[5].estado ==1)
              {
                VecPcb[5].GuardarPunto(x,y,2,6);
              }
            }      
        }
    gotoxy(X_in+contador_Candena, Y_in);
}
void interrupt handler(__CPPARGS) /* if C++, need the the ellipsis */

{

/* disable interrupts during the handling of the interrupt */

   disable();
/* increase the global counter */
  if(indice != -1)
  {
    Quamtum--;
  }
   
/* reenable interrupts at the end of the handler */
   enable();
/* call the old routine */
   oldhandler();

}


void Validar_ComandosPantalla()
{
  gotoxy(X_in+contador_Candena, Y_in);
  if(kbhit())
   {
      char ch = getch();
      if(ch == 13)
      {
        Comandos_pantalla();
      }
      else if(ch ==8)
      {
        if(contador_Candena >0)
        {
          printf("%s", "\b \b" );
          contador_Candena--;
          Cadena[contador_Candena] = '\0';
        }
      }
      else
      {
        if (contador_Candena<41)
        {
          Cadena[contador_Candena]= ch;
          printf("%c", ch);
          contador_Candena++;
        }
          
      }
    }
  gotoxy(X_in+contador_Candena, Y_in);
}


void Comandos_pantalla()
{
    recorrido_Cadena=0;
    gotoxy(X_out,Y_out);
    printf("%s", "                                                           ");
    gotoxy(X_out,Y_out);
    if(strncmp(Cadena, "add ", 4)==0)
    {
      recorrido_Cadena = 4;
      Validar_Proc(1, Cadena[recorrido_Cadena]-48);

    }
    else if (strncmp(Cadena, "pause ", 6)==0)
    {
      recorrido_Cadena = 6;
      Validar_Proc(2,  Cadena[recorrido_Cadena]-48);
    }
    else if (strncmp(Cadena, "remove ", 7)==0)
    {
      recorrido_Cadena = 7;
      Validar_Proc(3,  Cadena[recorrido_Cadena]-48);
    }
    else if (strncmp(Cadena, "clear ", 6)==0)
    {
      recorrido_Cadena = 6;
      Validar_Proc(4,  Cadena[recorrido_Cadena]-48);
    }
    else if (strncmp(Cadena, "quantum ", 8)==0)
    {
      recorrido_Cadena = 8;
      Validar_Proc(5, Cadena[recorrido_Cadena]-48);
    }
    else if (strncmp(Cadena, "stats ", 6)==0)
    {
      recorrido_Cadena = 6;
      Validar_Proc(6,  Cadena[recorrido_Cadena]-48);
    }
    else
    {
      printf("%s", "Ingrese un comando valido" );
      Error=1;
    }

    if(Error==0)
    {
      gotoxy(X_out, Y_out);
      printf("%s","Comando ejecutado con exito" );
      gotoxy(X_in,Y_in); 
      printf("%s","                                                " );
      gotoxy(X_in,Y_in); 
      contador_Candena=0;
      memset(Cadena, '\0', 100);
    }
    else
    {
      gotoxy(strlen(Cadena)+X_in, Y_in);
    }

    
    Error=0;
    
}

void Validar_Proc( int cmd, int proc)
{
  
  char temp[100];
  strcpy(temp,Cadena+recorrido_Cadena);
  if(proc >= 1 && proc <=6)
  {
    

    switch(cmd)
    {
      case 1:
        recorrido_Cadena++;
        //strcpy(temp,Cadena+4);
        if(recorrido_Cadena != strlen(Cadena))
        {
          Error = 1;
          printf("%s", "Proceso invalido: " );
          printf("%s", temp);
        }
        else
        {
          Comando_Add(proc);
        }
      break;
      case 2:
        recorrido_Cadena++;
        //strcpy(temp,Cadena+6);
        if(recorrido_Cadena != strlen(Cadena))
        {
          Error = 1;
          printf("%s", "Proceso invalido:  " );
          printf("%s", temp);
        }
        else
        {
          Comando_Pause(proc);
        }
        break;
      case 3:
        recorrido_Cadena++;
        //strcpy(temp,Cadena+7);
        if(recorrido_Cadena != strlen(Cadena))
        {
          Error = 1;
          printf("%s", "Proceso invalido: " );
          printf("%s", temp);
        }
        else
        {
          Comando_Remove(proc);
        }
        break;
      case 4:
        recorrido_Cadena++;
        //strcpy(temp,Cadena+6);
        if(recorrido_Cadena != strlen(Cadena))
        {
          Error = 1;
          printf("%s", "Proceso invalido: " );
          printf("%s", temp);
        }
        else
        {
          Comando_Clear(proc);
        }
        break;
      case 5:
          //strcpy(temp,Cadena+8);
          if(Cadena[9] != 32)
          {
            Error = 1;
            printf("%s", "Proceso invalido: " );
            printf("%s", temp);
          }
          else
          {
            Comando_Quantum(proc);
          }
          

        break;
      case 6:
          //strcpy(temp,Cadena+6);
          recorrido_Cadena++;
          if(strlen(Cadena) != recorrido_Cadena)
          {
              Error = 1;
              printf("%s", "Proceso invalido: " );
              printf("%s", temp);
          }
          else
          {
              Comando_Stats(proc);
          }

        break;
      default:
        break;
    }
  }
  else if(proc == -3)
  {
    //strcpy(temp,Cadena+recorrido_Cadena);
    if(cmd == 6)
    {
      if(Cadena[7]!= 49)
      {
        Error = 1;
        printf("%s", "Proceso invalido: " );
        printf("%s", temp);
      }
      else
      {
        recorrido_Cadena= recorrido_Cadena+2;
        if(strlen(Cadena) != recorrido_Cadena)
        {
            Error = 1;
            printf("%s", "Proceso invalido: " );
            printf("%s", temp);
        }
        else
        {
            Comando_Stats(7);
        }
      }
    }
    else
    {
      Error = 1;
      printf("%s", "Proceso invalido: " );
      printf("%s", temp);
    }
    
  }
  else if(proc == 0)
  {
    //strcpy(temp,Cadena+recorrido_Cadena);
    if(cmd == 6)
    {
      recorrido_Cadena++;
      if(strlen(Cadena) != recorrido_Cadena)
      {
          Error = 1;
          printf("%s", "Proceso invalido: " );
          printf("%s", temp);
      }
      else
      {
          Comando_Stats(0);
      }
    }
    else
    {
      Error = 1;
      printf("%s", "Proceso invalido: " );
      printf("%s", temp);
    }
  }
  else
  {
    Error=1;
    strcpy(temp,Cadena+recorrido_Cadena);
    printf("%s", "Procedimineto invalido: ");
    printf("%s", temp);
        
  }
}


void Comando_Add(int proc)
{
  switch(proc)
  {
    case 1:
          if(VecPcb[0].estado == 1)
          {
            Error=1;
            printf("%s","El proceso 1 ya esta activo" );
          }
          else
          {
            VecPcb[0].estado =1;
          }
      break;
    case 2:
          if(VecPcb[1].estado == 1)
          {
            Error=1;
            printf("%s","El proceso 2 ya esta activo" );
          }
          else
          {
            VecPcb[1].estado =1;
          }
      break;
    case 3:
          if(VecPcb[2].estado == 1)
          {
            Error=1;
            printf("%s","El proceso 3 ya esta activo" );
          }
          else
          {
            VecPcb[2].estado =1;
          }
      break;
    case 4:
          if(VecPcb[3].estado == 1)
          {
            Error=1;
            printf("%s","El proceso 4 ya esta activo" );
          }
          else
          {
            VecPcb[3].estado =1;
          }
      break;
    case 5:
        if(VecPcb[4].estado == 1)
          {
            Error=1;
            printf("%s","El proceso 5 ya esta activo" );
          }
          else
          {
            VecPcb[4].estado =1;
          }
      break;
    case 6:
        if(VecPcb[5].estado == 1)
          {
            Error=1;
            printf("%s","El proceso 6 ya esta activo" );
          }
          else
          {
            VecPcb[5].estado =1;
          }
      break;
    default:
      break;
  }
}

void Comando_Pause(int proc)
{
  switch(proc)
  {
    case 1:

        if(VecPcb[0].estado == 2)
        {
          Error=1;
          printf("%s","El proceso 1 ya esta en pausa" );
        }
        else if(VecPcb[0].estado == 0)
        {
          Error=1;
          printf("%s","El proceso 1 no esta activo" );
        }
        else
        {
          VecPcb[0].estado =2;
        }
      break;
    case 2:
        if(VecPcb[1].estado == 2)
        {
          Error=1;
          printf("%s","El proceso 2 ya esta en pausa" );
        }
        else if(VecPcb[1].estado == 0)
        {
          Error=1;
          printf("%s","El proceso 2 no esta activo" );
        }
        else
        {
          VecPcb[1].estado =2;
        }
      break;
    case 3:
        if(VecPcb[2].estado == 2)
        {
          Error=1;
          printf("%s","El proceso 3 ya esta en pausa" );
        }
        else if(VecPcb[2].estado == 0)
        {
          Error=1;
          printf("%s","El proceso 3 no esta activo" );
        }
        else
        {
          VecPcb[2].estado =2;
        }
        //Proceso 3 Validar
      break;
    case 4:
        if(VecPcb[3].estado == 2)
        {
          Error=1;
          printf("%s","El proceso 4 ya esta en pausa" );
        }
        else if(VecPcb[3].estado == 0)
        {
          Error=1;
          printf("%s","El proceso 4 no esta activo" );
        }
        else
        {
          VecPcb[3].estado =2;
        }
        //Proceso 4 Validar
      break;
    case 5:
        if(VecPcb[4].estado == 2)
        {
          Error=1;
          printf("%s","El proceso 5 ya esta en pausa" );
        }
        else if(VecPcb[4].estado == 0)
        {
          Error=1;
          printf("%s","El proceso 5 no esta activo" );
        }
        else
        {
          VecPcb[4].estado =2;
        }
        //Proceso 5 Validar
      break;
    case 6:
        if(VecPcb[5].estado == 2)
        {
          Error=1;
          printf("%s","El proceso 6 ya esta en pausa" );
        }
        else if(VecPcb[5].estado == 0)
        {
          Error=1;
          printf("%s","El proceso 6 no esta activo" );
        }
        else
        {
          VecPcb[5].estado =2;
        }
        //Proceso 6 Validar
      break;
    default:
      break;
  }
}


void Comando_Stats(int proc)
{
  clock_t tiempito = clock();
  double resultado = 0;
  Error=1;
  switch(proc)
  {
    case 0:
          for (int j = 0; j < 6; j++) 
          {
            if(VecPcb[j].estado == 1)
              {
                  resultado = (((double)VecPcb[j].tiempo)/((double)tiempito))*100;
                  printf("%i",(j+1) );
                  printf ("/%%%.1f",resultado);
                  printf("%s"," , " );
              } 
              
          }
      break;
    case 1:
         resultado = (((double)VecPcb[0].tiempo)/((double)tiempito))*100;
         printf ("1/%%%.1f",resultado); 
      break;
    case 2:
        resultado = (((double)VecPcb[1].tiempo)/((double)tiempito))*100;
         printf ("2/%%%.1f",resultado);
      break;
    case 3:
        resultado = (((double)VecPcb[2].tiempo)/((double)tiempito))*100;
         printf ("3/%%%.1f",resultado);
        //Proceso 3 Validar
      break;
    case 4:
        resultado = (((double)VecPcb[3].tiempo)/((double)tiempito))*100;
         printf ("4/%%%.1f",resultado);
        //Proceso 4 Validar
      break;
    case 5:
        resultado = (((double)VecPcb[4].tiempo)/((double)tiempito))*100;
         printf ("5/%%%.1f",resultado);
      break;
    case 6:
        resultado = (((double)VecPcb[5].tiempo)/((double)tiempito))*100;
         printf ("6/%%%.1f",resultado);
      break;
    case 7:
        resultado = (((double)tiempito-T_total)/((double)tiempito))*100;
         printf ("-1/%%%.1f",resultado);
      break;
    default:
      break;
  }
}

void Comando_Remove(int proc)
{
  switch(proc)
  {
    case 1:
          if(VecPcb[0].estado == 0)
          {
            Error=1;
            printf("%s","El proceso 1 no esta activo" );
          }
          else
          {
            puntero = &VecPcb[0].Matriz_puntos[0][0];
            Limpiar_Matriz();
            VecPcb[0].PCB2();
          }
      break;
    case 2:
          if(VecPcb[1].estado == 0)
          {
            Error=1;
            printf("%s","El proceso 2 no esta activo" );
          }
          else
          {
            puntero = &VecPcb[1].Matriz_puntos[0][0];
            Limpiar_Matriz();
            VecPcb[1].PCB2();
          }
      break;
    case 3:
          if(VecPcb[2].estado == 0)
          {
            Error=1;
            printf("%s","El proceso 3 no esta activo" );
          }
          else
          {
            puntero = &VecPcb[2].Matriz_puntos[0][0];
            Limpiar_Matriz();
            VecPcb[2].PCB2();
          }
      break;
    case 4:
          if(VecPcb[3].estado == 0)
          {
            Error=1;
            printf("%s","El proceso 4 no esta activo" );
          }
          else
          {
            puntero = &VecPcb[3].Matriz_puntos[0][0];
            Limpiar_Matriz();
            VecPcb[3].PCB2();
          }
      break;
    case 5:
        if(VecPcb[4].estado == 0)
          {
            Error=1;
            printf("%s","El proceso 5 no esta activo" );
          }
          else
          {
            puntero = &VecPcb[4].Matriz_puntos[0][0];
            Limpiar_Matriz();
            VecPcb[4].PCB2();
          }
      break;
    case 6:
        if(VecPcb[5].estado == 0)
          {
            Error=1;
            printf("%s","El proceso 6 no esta activo" );
          }
          else
          {
            puntero = &VecPcb[5].Matriz_puntos[0][0];
            Limpiar_Matriz();
            VecPcb[5].PCB2();
          }
      break;
    default:
      break;
  }
}

void Comando_Quantum(int proc)
{
  int num = ConvertirNum();
  printf("%i", num);
  switch(proc)
  {
    case 1:
          if(VecPcb[0].estado == 0)
          {
            Error=1;
            printf("%s","El proceso 1 no esta activo" );
          }
          else
          {
            VecPcb[0].Quamtum_PCB = num;
          }
      break;
    case 2:
          if(VecPcb[1].estado == 0)
          {
            Error=1;
            printf("%s","El proceso 2 no esta activo" );
          }
          else
          {
            VecPcb[1].Quamtum_PCB =num;
          }
      break;
    case 3:
          if(VecPcb[2].estado == 0)
          {
            Error=1;
            printf("%s","El proceso 3 no esta activo" );
          }
          else
          {
            VecPcb[2].Quamtum_PCB =num;
          }
      break;
    case 4:
          if(VecPcb[3].estado == 0)
          {
            Error=1;
            printf("%s","El proceso 4 no esta activo" );
          }
          else
          {
            VecPcb[3].Quamtum_PCB =num;
          }
      break;
    case 5:
        if(VecPcb[4].estado == 0)
          {
            Error=1;
            printf("%s","El proceso 5 no esta activo" );
          }
          else
          {
            VecPcb[4].Quamtum_PCB =num;
          }
      break;
    case 6:
        if(VecPcb[5].estado == 0)
          {
            Error=1;
            printf("%s","El proceso 6 no esta activo" );
          }
          else
          {
            VecPcb[5].Quamtum_PCB = num;
          }
      break;
    default:
      break;
  }
}

void Comando_Clear(int proc)
{
  switch(proc)
  {
    case 1:
          if(VecPcb[0].estado == 0)
          {
            Error=1;
            printf("%s","El proceso 1 no esta activo" );
          }
          else
          {
            puntero = &VecPcb[0].Matriz_puntos[0][0];
            Limpiar_Matriz();
            VecPcb[0].fila =0;
            VecPcb[0].Columna=0;
          }
      break;
    case 2:
          if(VecPcb[1].estado == 0)
          {
            Error=1;
            printf("%s","El proceso 2 no esta activo" );
          }
          else
          {
            puntero = &VecPcb[1].Matriz_puntos[0][0];
            Limpiar_Matriz();
            VecPcb[1].fila =0;
            VecPcb[1].Columna=0;
          }
      break;
    case 3:
          if(VecPcb[2].estado == 0)
          {
            Error=1;
            printf("%s","El proceso 3 no esta activo" );
          }
          else
          {
            puntero = &VecPcb[2].Matriz_puntos[0][0];
            Limpiar_Matriz();
            VecPcb[2].fila =0;
            VecPcb[2].Columna=0;
          }
      break;
    case 4:
          if(VecPcb[3].estado == 0)
          {
            Error=1;
            printf("%s","El proceso 4 no esta activo" );
          }
          else
          {
            puntero = &VecPcb[3].Matriz_puntos[0][0];
            Limpiar_Matriz();
            VecPcb[3].fila =0;
            VecPcb[3].Columna=0;
          }
      break;
    case 5:
        if(VecPcb[4].estado == 0)
          {
            Error=1;
            printf("%s","El proceso 5 no esta activo" );
          }
          else
          {
            puntero = &VecPcb[4].Matriz_puntos[0][0];
            Limpiar_Matriz();
            VecPcb[4].fila =0;
            VecPcb[4].Columna=0;
          }
      break;
    case 6:
        if(VecPcb[5].estado == 0)
          {
            Error=1;
            printf("%s","El proceso 6 no esta activo" );
          }
          else
          {
            puntero = &VecPcb[5].Matriz_puntos[0][0];
            Limpiar_Matriz();
            VecPcb[5].fila =0;
            VecPcb[5].Columna=0;
          }
      break;
    default:
      break;
  }
}

int ConvertirNum()
{
  char num[100];
  int num2=0;
  int num3=0;
  int contador =0;
  //int leer=0;
  strcpy(num,Cadena+10);
  int error_ciclo=0;
  while(error_ciclo ==0)
  {
    num3 = num[contador]-48;
    if(num3 == -48)
    {
      error_ciclo =1;
      if(contador==0)
      {
        Error =1;
        printf("%s", "Se esperaba un numero" );
      }
    }
    else
    {
        if(num3 >= 0 && num3 <= 9)
        {
          num2= num2 + num3;
          if((num[contador+1]-48) >= 0 && (num[contador+1]-48) <= 9)
          {
            num2 = num2*10;

          }
            
        }
        else
        {
   

          Error =1;
          error_ciclo=1;
          printf("%s", "Se esperaba un numero: " );
          printf("%s", num);
          
        }
    }
    
    contador++;   
  }
  return num2;
}

void Planificador()
{
    int bandera = 0;
    if (indice != -1)
    {
        if (Quamtum <= 0)
        { 
          do
          {
            if(indice > 5)
            {
              indice = -1;
              bandera = 1;
            }
            else
            {
              indice++;
              if(VecPcb[indice].estado == 1)
              {
                Quamtum = VecPcb[indice].Quamtum_PCB;
                bandera = 1;
              }
              
            }
          }while(bandera == 0);
        }
    }
    else
    {
         do
          {
            if(indice > 5)
            {
              indice = -1;
              bandera  = 1;
            }
            else
            {
              indice++;
              if(VecPcb[indice].estado == 1)
              {
                Quamtum = VecPcb[indice].Quamtum_PCB;
                bandera = 1;
              } 
            }
          }while(bandera == 0);
    }
    
  }
void pintar_Matriz()
{
   clock_t inicio;
   clock_t fin;
   clock_t resul;
  _setcursortype(_NOCURSOR);
    if(indice != -1)
    {
        inicio = clock();
        if (VecPcb[indice].fila < 10)
        {
          if(VecPcb[indice].Columna < 25)
          {
              if(VecPcb[indice].Matriz_puntos[VecPcb[indice].fila][VecPcb[indice].Columna].estatus == 1)
                {
                    gotoxy(VecPcb[indice].Matriz_puntos[VecPcb[indice].fila][VecPcb[indice].Columna].x,VecPcb[indice].Matriz_puntos[VecPcb[indice].fila][VecPcb[indice].Columna].y);
                    if(VecPcb[indice].opcion == 1)
                    {
                      printf("%s", ".");
                    }
                    else
                    {
                      printf("%s", " ");
                    }
                    
                }
             VecPcb[indice].Columna++; 
          }
          else
          {
            VecPcb[indice].Columna = 0;
            VecPcb[indice].fila++;
          }
          
        }
        else
        {
            VecPcb[indice].fila = 0;
            if(VecPcb[indice].opcion == 0)
            {
                VecPcb[indice].opcion = 1;
            }
            else
            {
              VecPcb[indice].opcion = 0;
            }
        }
        fin = clock();
        resul = fin-inicio;
        VecPcb[indice].tiempo += resul;
        T_total += resul;
    }
    _setcursortype(_NORMALCURSOR);
}

void Limpiar_Matriz()
{
  _setcursortype(_NOCURSOR);
  for (int k = 0; k < 10; k++) 
    {
        for (int b = 0; b < 25; b++) 
        {
            if(puntero[(k*25) + b].estatus == 1)
            {
                gotoxy(puntero[(k*25) + b].x,puntero[(k*25) + b].y);
                printf("%s", " ");   
                puntero[(k*25) + b].estatus =0;         
            }  
        }
    }
    
  _setcursortype(_NORMALCURSOR);
}

void ValidacionMouse()
{
    switch(BotonPulsado())
			{
          case 0:
              //cuando no pasa nada
              pulsaciones = 0;            
            break;
          case 1:
            
                if(pulsaciones == 0)
                {
                    pulsaciones = 1;
                    Guardar_PuntosPantalla();
                    
                }
                
            break;
            
          case 2:
                pulsaciones = 0; 
                //boton derecho pulsadp
            break;
			}
}

int main(void)

{
    
/* save the old interrupt vector */

   oldhandler = getvect(INTR);

/* install the new interrupt handler */

   setvect(INTR, handler);

/* loop until the counter exceeds 20 */

  PrintPantalla();
  //punteroPcb = VecPcb[0];
  int bandera = 0;
  while (bandera == 0) 
  {
    gotoxy(X_in+contador_Candena, Y_in);
    pintar_Matriz();
    ValidacionMouse();
    Validar_ComandosPantalla();  
    Planificador();
  }
      



/* reset the old interrupt handler */

   setvect(INTR, oldhandler);
   return 0;

}

int BotonPulsado()
	{
	//asm mov ax, 03h
	//asm int 33h
    reg.x.ax=0x3;
    int86(0x33, &reg, &reg);
	//return _BX;
    return reg.x.bx;
	}
int CorHTexto(void)
	{
	//asm mov ax, 03h
	//asm int 33h
	//return (_CX/8)+1;
    return (reg.x.cx/8)+1;
	}

int CorVTexto(void)
	{
	//asm mov ax, 03h
	//asm int 33h
	//return (_DX/8)+1;
    return (reg.x.dx/8)+1;
	}
void PrintPantalla()
{
  clrscr();
  printf("%s\n","|======= Proceso 1 =======|======= Proceso 2 =======|======= Proceso 3 =======|");
  printf("%s\n","|                         |                         |                         |");
  printf("%s\n","|                         |                         |                         |");
  printf("%s\n","|                         |                         |                         |");
  printf("%s\n","|                         |                         |                         |");
  printf("%s\n","|                         |                         |                         |");
  printf("%s\n","|                         |                         |                         |");
  printf("%s\n","|                         |                         |                         |");
  printf("%s\n","|                         |                         |                         |");
  printf("%s\n","|                         |                         |                         |");
  printf("%s\n","|                         |                         |                         |");
  printf("%s\n","|======= Proceso 4 =======|======= Proceso 5 =======|======= Proceso 6 =======|");
  printf("%s\n","|                         |                         |                         |");
  printf("%s\n","|                         |                         |                         |");
  printf("%s\n","|                         |                         |                         |");
  printf("%s\n","|                         |                         |                         |");
  printf("%s\n","|                         |                         |                         |");
  printf("%s\n","|                         |                         |                         |");
  printf("%s\n","|                         |                         |                         |");
  printf("%s\n","|                         |                         |                         |");
  printf("%s\n","|                         |                         |                         |");
  printf("%s\n","|                         |                         |                         |");
  printf("%s\n","|=============================================================================|" );
  printf("%s\n","|Input:                                                                       |" );
  printf("%s\n","|=============================================================================|" );
  printf("%s\n","|Output:                                                                      |" );
  printf("%s\n","|=============================================================================|" );
  gotoxy(8,24);
}
