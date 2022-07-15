//MENU 4 SERVOS BY DAVWOLF
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <Servo.h>

/*LIBRERIA KEYPAD PARA 2X3*/
const byte ROWS = 2; //four rows
const byte COLS = 3; //four columns
//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},

};
byte rowPins[ROWS] = {3,4}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {5,6,7}; //connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 




/*LIBRERIA LCD*/
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display


/*VARIABLES SERVOMOTOR*/
Servo motorX;
Servo motorY;
Servo motorZ;
Servo motorH;
Servo S[4] = {motorX,motorY,motorZ,motorH};     /*Arreglo que guarda la variable se configuracion de los Servos */

char M[6] = {'X', 'Y', 'Z', 'H'};         /*Arreglo para el nombre del servo que se muestra en la LCD */
int X,Y,Z,H = 0;
int MV[6] = {X,Y,Z,H};                    /*Arreglo que guarda las variables de los grados de los servos */

byte lobo[] = {
  B00000,
  B10001,
  B11111,
  B11111,
  B10101,
  B11111,
  B01010,
  B00100
};

byte skull[] = {
  B00000,
  B01110,
  B11111,
  B10101,
  B11011,
  B11111,
  B01010,
  B00000
};


/*VARIABLES DE TECLADO */
int i = 0;                              /* variable i para la poscion de la flecha */
int j = 0;                              /*variable que guarda valor de i para saber en que servo esta apuntando */

int col_p[6] = {0,0,8,8};               /* Arreglo pára las posiciones de las columnas */
int row_p[6] = {0,1,0,1};               /* Arreglo pára las posiciones de los renglones */
int col_ant = 0;                        /* guarda posicion anterior de la columna de la flecha para poder borrarla */
int rel_ant = 0;                        /* guarda posicion anterior del renglon de la flecha para poder borrarla */


/*VARIABLES DE CONTROL */
char f[6] = {'1','2','4','5'};          /*Arreglo que guarda el valor de la tecla oprimida (1 = UP, 2= DOWN, 3= LEFT, 4 = RIGHT=) */
int x;
int k;

void setup()
{

 lcd.init();                      // initialize the lcd 
  // Print a message to the LCD.
  lcd.backlight();                                                                /* Prepara el LCD */

  motorX.attach(9,650,2500);                                                      /*PREPARA SERVOMOTORES*/
  motorY.attach(10,650,2500);
  motorZ.attach(11,650,2500);
  motorH.attach(12,650,2500);

  motorX.write(0);                                                                /*Inicializa Servomotores 0° */
  motorY.write(0);
  motorZ.write(0);
  motorH.write(0);

  Menu();                                                                       /*Muestra el MENU PRINCIPAL */

  lcd.createChar(0, lobo);                        /*CARACTER 1*/
  
  lcd.createChar(1, skull);                        /* CARACTER 2*/
  
  lcd.home();
  lcd.write(0);
  
  Serial.begin(9600);                                                           /*Inicializa conexion Serial */
  Serial.println("PROGRAMA BY DAVWOLF");
}

void loop()
{

    char customKey = customKeypad.getKey();                                        /* Llama a la funcion "Teclado()" que sirve para leer la tecla oprimida*/

//Serial.print("LA TECLA ES: "); Serial.println(customKey);                           /* Valor prueba */
      switch(customKey)                                                            /* Switch case, que realiza accion dependiendo de la tecla oprimida */
  {
    case '1': if(i>0){ Serial.print("DOWN: "); Serial.println(customKey); Borrar_Flecha(); i--; Arrow(i); break;} else {break;}   /*Baja flecha */
    case '2': if(i<3){ Serial.print("UP: "); Serial.println(customKey); Borrar_Flecha(); i++; Arrow(i); break;} else{ break;}     /*Sube flecha */
    case '3': j = i; MenuDatos(j);                                            /*Caso de ser 3 (ENTER) abre el menu dependiendo de la variable j (valor de posicion) */
    default: break;                                                           /* En caso de no oprimir ninguna tecla valida, no realiza ninguna accion */
  } 
}




/*FUNCION DE LA FLECHA*/
void Arrow(int LED)                                                         /* Funcion para posicionar e imprimir la flecha en el LCD */
{

Serial.print ("          RENGLON: "); Serial.print(row_p[LED]); Serial.print ("          COLUMNA: "); Serial.println(col_p[LED]);     /*VALOR DE PRUEBA */

 lcd.setCursor(col_p[LED],row_p[LED]);                                      /*Coloca el cursor segun la i recibida desde el switch case */
 lcd.write(byte(0));                             /*CARACTER 1 SE IMPRIME*/
 
 lcd.write (byte(1));                           /* CARACTER 2 SE IMPRIME                Imprime la flecha en la poscion i */

    
   col_ant = col_p[LED];                                                    /*Guarda la posicion columna en una variable para poder se borrada posteriormente */
 rel_ant = row_p[LED];                                                      /*Guarda la posicion renglon en una variable para poder ser borrada posteriormente */

}

void Borrar_Flecha()                                                        /*Funcion para borrar la flecha */
{
   lcd.setCursor(col_ant,rel_ant);                                          /*Utiliza la posicion anterior guardada */
   lcd.print("  ");                                                         /*Imprime espacios en blanco que simula un 'borrado' */
}





/*MENU PRINCIPAL*/
void Menu()
{

  lcd.setCursor(2,0);                                                             /* Empieza a imprimir el texto fijo en el LCD */ 
  lcd.print("MotorX");
  lcd.setCursor(0,1);
  lcd.print("  MotorY");
  lcd.setCursor(8,0);
  lcd.print("  MotorZ");
  lcd.setCursor(8,1);
  lcd.print("  MotorH");
  Arrow(i);                                                                     /*Posiciona flecha en valor i */
}


/*CONTROL DE GRADO DE SERVOS X,Y,Z,H*/
void ControlMot(int j, int k)
{
                       if(MV[j] < 180)                                              /*Verifica que los grados sean menos a 180 para que pueda aumentarlos */
                     {
                            if(customKeypad.getState() == HOLD && x    ==  f[k])    /*Si se mantiene UP o LEFT (dependiendo del servo 'k') apretada realiza: */
                            {
                                MV[j]++;                                          /*Aumenta el valor de la variable de grados */
                                lcd.setCursor(9,1); lcd.print(MV[j]);  lcd.print("   "); /*Lo muestra en la LCD */
                                S[j].write(MV[j]);                              /*Y manda esos grados al Servo */
                                delay(100);
                            }
                     }
                     if(MV[j] >0)                                                /*Verifica que los grados sea mayor a 0 para poder disminuirlos */
                     {
                        if(customKeypad.getState() == HOLD && x    == f[k+1])      /*Si se mantiene DOWN O RIGHT (dependiendo del servo 'k')......... */
                        {
                            MV[j]--;
                            lcd.setCursor(9,1); lcd.print(MV[j]); lcd.print("   ");
                            S[j].write(MV[j]);
                            delay(100);
                        }
                     }
}


/*MENU SECUNDARIO*/
void MenuDatos(int j)
{   
/*Motor X,Y,Z,H*/ 
            lcd.setCursor(0,0);                                                     /*Dibuja el esqueleto del MotorX */
            lcd.print(" MOTOR "); lcd.print(M[j]); lcd.print("        ");
            lcd.setCursor(0,1); 
            lcd.print(" Grados: "); lcd.print(MV[j]); lcd.print("      ");         /*Imprime la variable de grados */
            while (j != 8)                                                         /*Mientras sea motorX (j = 0) */
            {
                  char customKey = customKeypad.getKey();                         /*Busca si se oprime tecla */
                  
                    if (customKey){                                               /*Si encuentra tecla oprimida: */
                    Serial.println(customKey);
                    x = customKey;
                    }

                    
                    if(j == 0 || j== 2)                                         /*Si el servo es X (0) o Z (2) entonces: */
                    {
                      k = 0;                                                   /*K = 0 para poder mover el servo SOLO con UP y DOWN */
                      ControlMot(j,k);
                    }
                    else {k = 2; ControlMot(j, k);}                           /*Si el servo es Y (1) o H (3) entonces: */ /*K = 2 para poder mover el servo SOLO con LEFT y RIGHT */

                  
                  if(customKey == '3') {j = 8;}                     /*Si se presiona ENTER (3) reinicia valor j por lo que sale del ciclo */  
            }          
  
  i = 0;                            /*Al salirse del ciclo y del case.... reinicia la posicion de la flecha */
    Menu();                         /*Vuelve al MENU PRINCIPAL */
  
}
