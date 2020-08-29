/***********************************************************************
*                                                                      *
*                           VISCOSÍMETRO                               *
*                     Lael Ramos | Sizenando Braga                     *
*                                                                      *
*                                                                      *
***********************************************************************/


// =============================================================================================================
// --- Bibliotecas Auxiliares ---
#include "DHT.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);


// =============================================================================================================
// --- Mapeamento de Hardware ---
#define bt_e 5
#define bt_d 4
#define bt_v 3
#define bt_mn 2

#define start_ A2
#define stop_ A3

#define DHTPIN 6

// =============================================================================================================
// --- Constantes e Objetos ---
byte p20[8] = {
  B10000,
  B10000,
  B10000,
  B10000,
  B10000,
  B10000,
  B10000,
};
byte p40[8] = {
  B11000,
  B11000,
  B11000,
  B11000,
  B11000,
  B11000,
  B11000,
};
byte p60[8] = {
  B11100,
  B11100,
  B11100,
  B11100,
  B11100,
  B11100,
  B11100,
};
byte p80[8] = {
  B11110,
  B11110,
  B11110,
  B11110,
  B11110,
  B11110,
  B11110,
};
byte p100[8] = {
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
};


// =============================================================================================================
// --- Variáveis Auxiliares ---
#define   menu_max   2  //número máximo de menus existentes

#define DHTTYPE DHT11     // DHT 11
//#define DHTTYPE DHT22   // DHT 22
DHT dht(DHTPIN, DHTTYPE);


// =============================================================================================================
// --- Variáveis Menu ---
int menu_init = 1, menu_num = 1, sub_menu = 1, sub_menu1 = 1;

// --- Variáveis Tempo ---
double c = 0, a = millis(), b ;

// --- Variáveis densidade --- 
int    x               = 100;
int   flag             = 0;
float incremento       = 0.1;
int   flag1            = 0;
int   x1               = 100;
float incremento1      = 0.1;
float DensidadeOleo    = 871.0;
// =============================================================================================================
// --- Variáveis angulo --- 
int    Angulo      = 80;
float  AnguloRad   = (Angulo * (3.14/180));
float  cosseno     = cos(AnguloRad);

// --- Variáveis Cálculo ---
float resultado;
float m;

float vg32;
float vg46;
float vg68;
float vg100;

float visc_32;
float visc_46;
float visc_68;
float visc_100;

float menor;

float q;
float r;
float s;
float t;
float z;
float j;
float w;
float p;

float Distancia      = 0.150;
float Gravidade      = 9.806;
float DiametroEsfera = 0.005;
float DensidadeAco   = 7850.;

int IndiceCorrecao = 10000;
float DiametroTubo   = 0.012;

float elemento32     = 56.9;
float elemento46     = 65.0;
float elemento68     = 74.4;
float elemento100    = 84.3;
float Fator          = 2.0144;





// =============================================================================================================
// --- Funções ---
void teclado();
void menu();
void medir();
void submenu();
void densidade();
void angulo();
void menu_var();
void timer();
void calculo();
void distancia_x();
void d_esfera();
void d_tubo();
void rugosidade();
void correcao();

// =============================================================================================================
// --- Configurações Iniciais ---
void setup() 
{
  lcd.init();
  //lcd.begin();
  lcd.backlight();
  dht.begin();
  Serial.begin (9600);

  pinMode(bt_e, INPUT_PULLUP);
  pinMode(bt_d, INPUT_PULLUP);
  pinMode(bt_v, INPUT_PULLUP);
  pinMode(bt_mn, INPUT_PULLUP);

  pinMode(start_, INPUT_PULLUP);
  pinMode(stop_, INPUT_PULLUP);

// =============================================================================================================
// --- Mensagem Inicial ---
  lcd.setCursor (0, 0); lcd.print("  VISCOSIMETRO  ");
  lcd.createChar(0, p20);
  lcd.createChar(1, p40);
  lcd.createChar(2, p60);
  lcd.createChar(3, p80);
  lcd.createChar(4, p100);

  lcd.setCursor(0, 1);
  lcd.print("                ");
  

  for (int a1 = 0; a1 < 16; a1++)
      { for (int a2 = 0; a2 < 5; a2++)
           { lcd.setCursor(a1, 1);
             lcd.write(a2);
             delay(15);
           }
      }
  lcd.clear();
}//end setup


// =============================================================================================================
// --- Configurações Iniciais ---
void loop() 
{  
  teclado();
  while (menu_init == 1)
        {teclado();
         menu();
        }

  while (menu_init == 2)
        {teclado();
         medir();
        }
}


// =============================================================================================================
// --- Função teclado ---
void teclado() 
{         
          if (!digitalRead(bt_d) && menu_num == 1)   ///
             { while(!digitalRead(bt_d));            ///
               delay(50);                            ////// NUMERO DE MENUS
               if (menu_init <= 2) menu_init++;      ///
               if (menu_init > 2) menu_init = 1;     ///
             }                                       ///
    
          if (!digitalRead(bt_d) && menu_num == 2)   ///
             { while(!digitalRead(bt_d));            ///
               delay(50);                            ////// NUMERO DE SUB MENUS
               if (sub_menu <= 8) sub_menu++;        ///
               if (sub_menu  > 8) sub_menu = 1;      ///
             } //end bt_d                            /// 


  if (!digitalRead(bt_e) && menu_num == 1)           ///
     { while(!digitalRead(bt_e));                    ///
       delay(50);                                    ////// NUMERO DE MENUS
       if (menu_init > 0) menu_init--;               ///
       if (menu_init == 0) menu_init = 2;            ///
     }                                               ///
  
  if (!digitalRead(bt_e) && menu_num == 2)           ///
     { while(!digitalRead(bt_e));                    ///
       delay(50);                                    ////// NUMERO DE SUB MENUS
       if (sub_menu > 0) sub_menu--;                 ///
       if (sub_menu == 0) sub_menu = 8;              ///
  }//end bt_ e                                       ///

          
          if (!digitalRead(bt_mn) && menu_num == 2)
             { delay(150);
               while(!digitalRead(bt_mn));
               if (sub_menu1 <= 2) sub_menu1++;
               if (sub_menu1 > 2) sub_menu1 = 2;
             } 

          if (!digitalRead(bt_mn))
             { delay(150);
               while(!digitalRead(bt_mn));
               if (menu_num <= 2) menu_num++;
               if (menu_num > 2) menu_num = 2;
          }//end bt_mn



  if (!digitalRead(bt_v))
     { while(!digitalRead(bt_v));
       delay(150);
       if (menu_num > 0) menu_num--;
       if (menu_num < 1) menu_num = 1;
  }
  
  if (!digitalRead(bt_v))
     { delay(150);
       while(!digitalRead(bt_v));
       if (sub_menu1 > 1) sub_menu1--;
       if (sub_menu1 < 1) sub_menu1 = 1; 
     } //end bt_v
} //end teclado


// =============================================================================================================
// --- Função menu ---
void menu() 
{
  /* Serial.print("menu_init: "); 
     Serial.print(menu_init); 
     Serial.print("  menu_num: ");  
     Serial.print(menu_num); 
     Serial.print("  sub_menu: ");  
     Serial.print(sub_menu); 
     Serial.print("  sub_menu1: ");
     Serial.println(sub_menu1); */
  switch (menu_num)
         { case 1:
           lcd.setCursor(0, 0); lcd.print("  VISCOSIMETRO  ");
           lcd.setCursor(0, 1); lcd.print(">Menu      Medir");
           break;
           case 2:
           submenu();
           break;
         }
} //end menu


// =============================================================================================================
// --- Função medir ---
void medir() 
{
  switch (menu_num)
         { case 1:
           lcd.setCursor(0, 0); lcd.print("  VISCOSIMETRO  ");
           lcd.setCursor(0, 1); lcd.print(" Menu     >Medir");
           break;
           case 2:
           timer();
           break;
         }
} //end medir


////////////////////////////////////////Sub menu///////////////////////////////////////////
void submenu() 
{ 
  switch(sub_menu)
        { case 1: menu1(); break; //Dens. Fluido
          case 2: menu2(); break; //Angulo
          case 3: menu3(); break; //Temperatura
          case 4: menu4(); break; //Distância
          case 5: menu5(); break; //Diam. Esfera
          case 6: menu6(); break; //Diam. Tubo
          case 7: menu7(); break; //Rugosidade
          case 8: menu8(); break; //Ind. correção
        }
} //end submenu


////////////////////////////////////////MENU 1///////////////////////////////////////////
void menu1() 
{
  switch(sub_menu1)
        { case 1:
          lcd.setCursor(0,0); lcd.print("< Dens. Fluido >");
          lcd.setCursor(0,1); lcd.print("                ");
          break;
          case 2:
          densidade();
          break;
         }
} //end menu1


////////////////////////////////////////MENU 2///////////////////////////////////////////
void menu2()
{ 
  switch(sub_menu1)
        { case 1:
          lcd.setCursor(0,0); lcd.print("<    Angulo    >");
          lcd.setCursor(0,1); lcd.print("                ");
          break;
          case 2:
          angulo();
          break;
        } 
} //end menu2


////////////////////////////////////////MENU 3///////////////////////////////////////////
void menu3()
{ 
  float t = dht.readTemperature();            //Celsius
  
  lcd.setCursor(0,0); lcd.print("< Temperatura  >");
  lcd.setCursor(0,1); lcd.print(t,1); 
  lcd.setCursor(4,1); lcd.print(" C "); lcd.print("         ");
  if(!digitalRead(bt_mn)) {delay(150); while(!digitalRead(bt_mn)); sub_menu1 = 1;}
  if(!digitalRead(bt_v))  {delay(150); while(!digitalRead(bt_v));   menu_num = 1;}
} //end menu3


////////////////////////////////////////MENU 4///////////////////////////////////////////
void menu4()
{ 
  switch(sub_menu1)
        { case 1:
          lcd.setCursor(0,0); lcd.print("<  Distancia   >");
          lcd.setCursor(0,1); lcd.print("                ");
          break;
          case 2:
          distancia_x();
          break;
        } 
} //end menu4


////////////////////////////////////////MENU 5///////////////////////////////////////////
void menu5()
{ 
  switch(sub_menu1)
        { case 1:
          lcd.setCursor(0,0); lcd.print("< Diam. Esfera >");
          lcd.setCursor(0,1); lcd.print("                ");
          break;
          case 2:
          d_esfera();
          break;
        } 
} //end menu5


////////////////////////////////////////MENU 6///////////////////////////////////////////
void menu6()
{ 
  switch(sub_menu1)
        { case 1:
          lcd.setCursor(0,0); lcd.print("<  Diam. Tubo  >");
          lcd.setCursor(0,1); lcd.print("                ");
          break;
          case 2:
          d_tubo();
          break;
        } 
} //end menu6


////////////////////////////////////////MENU 7///////////////////////////////////////////
void menu7()
{ 
  switch(sub_menu1)
        { case 1:
          lcd.setCursor(0,0); lcd.print("<  Rugosidade  >");
          lcd.setCursor(0,1); lcd.print("                ");
          break;
          case 2:
          rugosidade();
          break;
        } 
} //end menu7

////////////////////////////////////////MENU 8///////////////////////////////////////////
void menu8()
{ 
  switch(sub_menu1)
        { case 1:
          lcd.setCursor(0,0); lcd.print("<   Correcao   >");
          lcd.setCursor(0,1); lcd.print("                ");
          break;
          case 2:
          correcao();
          break;
        } 
} //end menu8



/////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////SUB-MENU DENSIDADE///////////////////////////////////////////
void densidade()
{ 
  while (sub_menu1 == 2)
        { lcd.setCursor(0,0); lcd.print("  Dens. Fluido  ");
          lcd.setCursor(0,1); lcd.print(DensidadeOleo,1); lcd.print(" "); lcd.print("kg/m3"); lcd.print(" ");
          if (!digitalRead(bt_d) && flag==1) 
            {if (x!=1) {x -= 9;} 
             else {incremento = 1;} delay(x); DensidadeOleo+=incremento;} 
          else {flag=0; x = 100; incremento = 0.1; }
          if (!digitalRead(bt_d) && flag==0) { flag = 1; delay(x); DensidadeOleo+=incremento; }  //end incremento
     
          if (!digitalRead(bt_e) && flag1==1) 
             {if(x1!=1) { x1 -= 9;} 
              else {incremento1 = 1;} delay(x1); DensidadeOleo-=incremento1;} 
              else {flag1=0; x1 = 100; incremento1 = 0.1;}
          if(!digitalRead(bt_e) && flag1==0) {flag1 = 1; delay(x1); DensidadeOleo-=incremento1;}
          if(!digitalRead(bt_v)) {delay(150); while(!digitalRead(bt_v)); sub_menu1 = 1; }      //end decremento
        }
} //end densidade


/////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////SUB-MENU ANGULO /////////////////////////////////////////////
void angulo()
{
  while(sub_menu1 == 2)
       { lcd.setCursor(0,0); lcd.print("     Angulo     ");
         lcd.setCursor(0,1); lcd.print(Angulo); lcd.print("   ");
         if(!digitalRead(bt_d)) {delay(x); Angulo++;}
         if(!digitalRead(bt_e)) {delay(x); Angulo--;}
         if(Angulo < 0)    {Angulo = 180;}
         if(Angulo > 180)  {Angulo = 0;}  
     
         if(!digitalRead(bt_v)) {delay(150); while(!digitalRead(bt_v)); sub_menu1 = 1;}
       }
} //end angulo


/////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////SUB-MENU DISTANCIA /////////////////////////////////////////////
void distancia_x()
{
  while(sub_menu1 == 2)
       { lcd.setCursor(0,0); lcd.print("   Distancia    ");
         lcd.setCursor(0,1); lcd.print(Distancia,3); lcd.print("  m  ");
         if(!digitalRead(bt_d)) {delay(x); Distancia = Distancia + 0.001;}
         if(!digitalRead(bt_e)) {delay(x); Distancia = Distancia - 0.001;}
     
         if(!digitalRead(bt_v)) {delay(150); while(!digitalRead(bt_v)); sub_menu1 = 1;}
       }
} //end distancia_x


/////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////SUB-MENU DIAMETRO ESFERA /////////////////////////////////////////////
void d_esfera()
{
  while(sub_menu1 == 2)
       { lcd.setCursor(0,0); lcd.print("  Diam. Esfera  ");
         lcd.setCursor(0,1); lcd.print(DiametroEsfera,3); lcd.print("  m  ");
         if(!digitalRead(bt_d)) {delay(x); DiametroEsfera = DiametroEsfera + 0.001;}
         if(!digitalRead(bt_e)) {delay(x); DiametroEsfera = DiametroEsfera - 0.001;}
     
         if(!digitalRead(bt_v)) {delay(150); while(!digitalRead(bt_v)); sub_menu1 = 1;}
       }
} //d_esfera


/////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////SUB-MENU DIAMETRO TUBO /////////////////////////////////////////////
void d_tubo()
{
  while(sub_menu1 == 2)
       { lcd.setCursor(0,0); lcd.print("   Diam. Tubo   ");
         lcd.setCursor(0,1); lcd.print(DiametroTubo,3); lcd.print("  m  ");
         if(!digitalRead(bt_d)) {delay(x); DiametroTubo = DiametroTubo + 0.001;}
         if(!digitalRead(bt_e)) {delay(x); DiametroTubo = DiametroTubo - 0.001;}
     
         if(!digitalRead(bt_v)) {delay(150); while(!digitalRead(bt_v)); sub_menu1 = 1;}
       }
} //d_tubo


/////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////SUB-MENU RUGOSIDADE /////////////////////////////////////////////
void rugosidade()
{
  while(sub_menu1 == 2)
       { lcd.setCursor(0,0); lcd.print("   Rugosidade   ");
         lcd.setCursor(0,1); lcd.print(Fator,4); lcd.print("   ");
         if(!digitalRead(bt_d)) {delay(x); Fator = Fator + 0.0001;}
         if(!digitalRead(bt_e)) {delay(x); Fator = Fator - 0.0001;}
     
         if(!digitalRead(bt_v)) {delay(150); while(!digitalRead(bt_v)); sub_menu1 = 1;}
       }
} //Fator


/////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////SUB-MENU COREÇÃO /////////////////////////////////////////////
void correcao()
{
  while(sub_menu1 == 2)
       { lcd.setCursor(0,0); lcd.print("    Correcao    ");
         lcd.setCursor(0,1); lcd.print(IndiceCorrecao); lcd.print("   ");
         if(!digitalRead(bt_d)) {delay(x); IndiceCorrecao = IndiceCorrecao + 1000;}
         if(!digitalRead(bt_e)) {delay(x); IndiceCorrecao = IndiceCorrecao - 1000;}
     
         if(!digitalRead(bt_v)) {delay(150); while(!digitalRead(bt_v)); sub_menu1 = 1;}
       }
} //Fator


/////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////TIMER /////////////////////////////////////////////
void timer()
{   
  lcd.setCursor(0,0); lcd.print("Aguardando...   "); 
  lcd.setCursor(0,1); lcd.print("                ");

  if(digitalRead(start_) == LOW)
    { lcd.clear();
      a = millis();
      while(digitalRead(stop_) == HIGH)
           { b = millis();
             c = (b - a) / 1000;
             lcd.print(c);
             lcd.setCursor(7,0);
             lcd.print("Seg's.");
             lcd.setCursor(0,0);
             delay(100);             
           }
      delay(1300);
      calculo(); 
    }
}


////////////////////////////////////////////////////////////////////////////////////////////////////       
/////////////////////////////////////////////RESULTADO////////////////////////////////////////////// 
void calculo() 
{ 
  int n = 0;                     
  lcd.setCursor(0,0); lcd.print("Calculando.     "); 
  lcd.setCursor(0,1); lcd.print("                ");
  delay(600);
  lcd.setCursor(0,0); lcd.print("Calculando..    "); 
  lcd.setCursor(0,1); lcd.print("                ");
  delay(600);
  lcd.setCursor(0,0); lcd.print("Calculando...   "); 
  lcd.setCursor(0,1); lcd.print("                ");
  delay(600);
  lcd.clear();
 
  float Velocidade  = (Distancia / c);
  float temperatura = dht.readTemperature();

  m = (log10(36) - log10(16)) / -20;
 
  w = Gravidade*((DiametroEsfera,6)*(DiametroEsfera,6))*(DensidadeAco-DensidadeOleo)*cosseno;
    
  p = 18*(1+Fator*(DiametroEsfera/DiametroTubo))* Velocidade * IndiceCorrecao;

  resultado = w/p;

  vg32  = m * (temperatura - elemento32)  + log10(16);
  vg46  = m * (temperatura - elemento46)  + log10(16);
  vg68  = m * (temperatura - elemento68)  + log10(16);
  vg100 = m * (temperatura - elemento100) + log10(16);

  visc_32  = pow(10,  vg32);
  visc_46  = pow(10,  vg46);
  visc_68  = pow(10,  vg68);
  visc_100 = pow(10, vg100);

  q = (resultado - visc_32);
  r = (resultado - visc_46);
  s = (resultado - visc_68);
  t = (resultado - visc_100);

  q = abs(q);
  r = abs(r);
  s = abs(s);
  t = abs(t);
 
  z = min (q,r); 
  j = min (s,t);
  menor = min (z,j); 
  n++;     
         
  while(n > 0) 
       { lcd.setCursor(0,0);   lcd.print("Viscos. : "); 
         lcd.setCursor(10,0);  lcd.print(resultado);
         if (menor == q)
            {lcd.setCursor(0,1); lcd.print("Grau ISO: vg 32 ");} 
         if (menor == r)
            {lcd.setCursor(0,1); lcd.print("Grau ISO: vg 46 ");}
         if (menor == s)
            {lcd.setCursor(0,1); lcd.print("Grau ISO: vg 68 ");}
         if (menor == t)
            {lcd.setCursor(0,1); lcd.print("Grau ISO: vg 100");} 
                
         if(!digitalRead(bt_v)) {delay(150); while(!digitalRead(bt_v)); lcd.clear(); n=0; }
       }     
} //end calculo
