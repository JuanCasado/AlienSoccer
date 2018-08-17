void comunica_xbee(void);

int mv=0;

void comunica_xbee(void)
{
  //Borra terminal VT100
  Serial3.print(byte(0x1B));
  Serial3.print("[2J");
  
  if(robot == ROBOT_NEXUS_9_2)
    Serial3.println("Nexus 9.2 - Command Mode");

  else
    Serial3.println("Nexus 9.1 - Command Mode");

  Serial3.print("> ");

  while(true)
  {    
    if(Serial3.available() > 0)
    {
      switch(Serial3.read())
      {
      case 'B':
        Serial3.println();
        Serial3.print("> ");
        Serial3.print("Bat Electro: ");
        Serial3.print((promedia_ai(BAT_ELECTRO,16) * 59.0)/2500.0);
        Serial3.print("V");
        Serial3.print(" Bat Pot: ");
        Serial3.print((promedia_ai(BAT_POT,16) * 59.0)/2500.0);
        Serial3.println("V");
        Serial3.print("> ");
        break;

      case 'c':
        Serial3.println();
        Serial3.print("> ");
        Serial3.print("Posicion Brujula: ");
        Serial3.print(varia_posicion_brujula(brujula_inicial));
        Serial3.println();
        Serial3.print("> ");
        break;  

      case 'C':
        chuta();
        Serial3.println();
        Serial3.print("> ");
        break;


      case 'h':
      case 'H':
        Serial3.println();
        Serial3.println();
        Serial3.println("Ayuda");
        Serial3.println("-----");
        Serial3.println();
        Serial3.println("B   -> Tension Baterias");
        Serial3.println("c   -> Brujula");
        Serial3.println("C   -> Chuta");
        Serial3.println("I   -> Identificacion");
        Serial3.println("M   -> Mueve Robot");
        Serial3.println("P,p -> Para Robot");
        Serial3.println("S   -> Sensores Bola");
        Serial3.println("U   -> Sensores Ultrasonidos");
        Serial3.println("V   -> Ajusta Velocidad");
        Serial3.println();
        Serial3.print("> ");
        break;

      case 'I':
        beep();
        if(robot == ROBOT_NEXUS_9_2)
          Serial3.println(" Nexus 9.2");

        else
          Serial3.println(" Nexus 9.1");
        Serial3.print("> ");
        break;

      case 'M':
        while(Serial3.available() < 2);
        mv=((Serial3.read()-0x30)*10)+(Serial3.read()-0x30);
        if((mv>=0) && (mv<48))
        {
          mueve_robot(mv);
        }
        else
        {
          Serial3.println("Error");
        }
        Serial3.println();
        Serial3.print("> ");
        break;

      case 'p':
      case 'P':
        para_robot();
        Serial3.println();
        Serial3.print("> ");
        break;

      case 'S':
        actualiza_lecturas_arduball();

        Serial3.println();
        Serial3.print("> ");
        Serial3.print("Sensor Max: ");
        Serial3.print(sensor_maximo);
        Serial3.print("Distancia: ");
        Serial3.println(distancia_bola);
        Serial3.print("> ");
        break;

      case 'U':
        actualiza_lecturas_ardusonic();

        Serial3.println();
        Serial3.print("> ");
        Serial3.print("US Derecho: ");
        Serial3.print(us_derecho);
        Serial3.print(" US Izquierdo: ");
        Serial3.print(us_izquierdo);
        Serial3.print(" US Trasero: ");
        Serial3.println(us_trasero);
        Serial3.print("> ");
        break;

      case 'V':
        while(Serial3.available() < 3);
        mv=((Serial3.read()-0x30)*100)+((Serial3.read()-0x30)*10)+(Serial3.read()-0x30);
        if((mv>=0) && (mv<101))
        {
          vel_max_robot(mv);
        }
        else
        {
          Serial3.println("Error");
        }
        Serial3.println();
        Serial3.print("> ");
        break;

      default:
        Serial3.println(" what?");
        Serial3.print("> ");
        break;
      }
    }
  }
}


