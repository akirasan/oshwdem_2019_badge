/*

  #OSHWDem 19 - OSHWi
  Código para demo badged de la OSHWDem 2019
  Adaptado para emitir sonido con un buzzer
  by @akirasan

  Para programar el ATTiny85:
  - Arduno UNO con firmware cargado "ArduinoISP" (se encuentra en el IDE de Arduino como ejemplo)
  - Package Board: ATTinyCore Spence Konde http://drazzy.com/package_drazzy.com_index.json
  - Seleccionar la placa ATTiny85 (Procesador ATTiny85 / 16Mhz (PLL)
  - Opcional/Recomendable: La primera vez quemar bootloader antes del programa
  - Programador "Arduino as ISP"

*/

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif


#define PIN            2
#define PIN_BOTON      3
#define NUMPIXELS      2
#define BUZZER         1


/* TinyTone for ATtiny85 */

// Notes
const int Note_C  = 239;
const int Note_CS = 225;
const int Note_D  = 213;
const int Note_DS = 201;
const int Note_E  = 190;
const int Note_F  = 179;
const int Note_FS = 169;
const int Note_G  = 159;
const int Note_GS = 150;
const int Note_A  = 142;
const int Note_AS = 134;
const int Note_B  = 127;



Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

// G R B

#define naranja {57,255,0}
#define blanco {180,180,180}
#define amarillo {180,180,0}
#define celeste {128,0,124}
#define rojo {0,255,0}
#define azul {0,0,255}
#define verde {255,0,0}
#define violeta {0, 155, 155}
// --- Lista de colores a configurar

byte colores[][3] = {verde, rojo, azul, violeta, naranja, blanco, amarillo, celeste };
byte total_colores = (sizeof(colores) / 3) - 1;
byte color1;
byte color2;

byte click_boton = 0;
boolean nuevo_color = true;
unsigned int estado_boton;

boolean demo_mode = false;
boolean sound = true;


void setup() {
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif

  pinMode(PIN, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pixels.begin();
  randomSeed(analogRead(0));


  // MODO OCULTO DEMO
  estado_boton = analogRead(PIN_BOTON);
  delay(100);
  if (estado_boton == 1023 ) {
    demo_mode = true;
    for (byte a = 0; a < 50; a++) {
      efecto_leds(1, 2, 0);
    }
    delay(250);
  }

  playTuneStart();
}


boolean ajustar_RGB(byte *ci, byte *cf) {
  boolean ajuste = true;
  byte color_ini, color_fin;
  color_ini = *ci;
  color_fin = *cf;

  if (color_ini == color_fin) {
    ajuste = false;
  }
  else {
    if (color_ini < color_fin) {
      color_ini++;
    }
    else {
      color_ini--;
    }
  }
  *ci = color_ini;
  *cf = color_fin;

  return ajuste;
}


void fundir_a_color(byte icolor) {
  boolean r = true, g = true , b = true;
  byte gf = colores[icolor][0], rf = colores[icolor][1], bf = colores[icolor][2];
  byte ri, gi, bi;

  // comparamos el color actual con el nuevo para ver si tiene sentido el fundido.
  uint32_t color_actual = pixels.getPixelColor(1);
  uint32_t color_solicitado = pixels.Color(gf, rf, bf);

  if (color_actual != color_solicitado) {
    if (icolor == 0) {
      gi = colores[total_colores][0];
      ri = colores[total_colores][1];
      bi = colores[total_colores][2];

    }
    else {
      gi = colores[icolor - 1][0];
      ri = colores[icolor - 1][1];
      bi = colores[icolor - 1][2];

    }

    while (r || g || b) {
      if (r) {
        r = ajustar_RGB(&ri, &rf);
      }
      if (g) {
        g = ajustar_RGB(&gi, &gf);
      }
      if (b) {
        b = ajustar_RGB(&bi, &bf);
      }
      pixels.setPixelColor(0, pixels.Color(gi, ri, bi));
      pixels.setPixelColor(1, pixels.Color(gi, ri, bi));
      pixels.show();
      delay(1);

    }
  }
}


// efecto N, con el color N
void efecto_leds(byte iefecto, byte color1 , byte color2) {
  byte gf = colores[color1][0], rf = colores[color1][1], bf = colores[color1][2];

  if (iefecto == 1) {
    //    for (int i = 0; i < 10; i++) {
    pixels.setPixelColor(0, pixels.Color(gf, rf, bf));
    pixels.setPixelColor(1, pixels.Color(gf, rf, bf));
    pixels.show();
    delay(20);
    pixels.setPixelColor(0, pixels.Color(0, 0, 0));
    pixels.setPixelColor(1, pixels.Color(0, 0, 0));
    pixels.show();
    delay(20);
    //    }
  }
  if (iefecto == 2) {
    //    for (int i = 0; i < 10; i++) {
    pixels.setPixelColor(0, pixels.Color(gf, rf, bf));
    pixels.setPixelColor(1, pixels.Color(0, 0, 0));
    pixels.show();
    delay(50);
    pixels.setPixelColor(0, pixels.Color(0, 0, 0));
    pixels.setPixelColor(1, pixels.Color(gf, rf, bf));
    pixels.show();
    delay(50);
    //    }
    pixels.setPixelColor(0, pixels.Color(0, 0, 0));
    pixels.setPixelColor(1, pixels.Color(0, 0, 0));
    pixels.show();
  }
  /*
    if (iefecto == 3) {
    // selecciona el color1 y color2 de la lista
    byte ga = colores[1][0], ra = colores[1][1], ba = colores[1][2];
    byte gb = colores[2][0], rb = colores[2][1], bb = colores[2][2];

    pixels.setPixelColor(0, pixels.Color(ga, ra, ba));
    pixels.setPixelColor(1, pixels.Color(0, 0, 0));
    pixels.show();
    delay(50);
    pixels.setPixelColor(0, pixels.Color(0, 0, 0));
    pixels.setPixelColor(1, pixels.Color(gb, rb, bb));
    pixels.show();
    delay(50);
    pixels.setPixelColor(0, pixels.Color(0, 0, 0));
    pixels.setPixelColor(1, pixels.Color(0, 0, 0));
    pixels.show();
    }
  */
  if (iefecto == 4) {
    byte ga = colores[color1][0], ra = colores[color1][1], ba = colores[color1][2];
    byte gb = colores[color2][0], rb = colores[color2][1], bb = colores[color2][2];

    pixels.setPixelColor(0, pixels.Color(ga, ra, ba));
    pixels.setPixelColor(1, pixels.Color(gb, rb, bb));
    pixels.show();
    delay(50);
    pixels.setPixelColor(0, pixels.Color(gb, rb, bb));
    pixels.setPixelColor(1, pixels.Color(ga, ra, ba));
    pixels.show();
    delay(50);
  }

}


// Colores GRB
void loop() {

  if (!demo_mode) {

    estado_boton = analogRead(PIN_BOTON);

    // click boton
    if (estado_boton == 1023 ) {
      click_boton++;
      sound = true;
    }
  }
  else
  {
    click_boton++;
  }


  switch (click_boton) {
    case 1:
      if (sound) {
        TinyTone(Note_A, 4, 100);
        TinyTone(Note_A, 4, 100);
        sound = false;
      }
      fundir_a_color(0);
      break;
    case 2:
      if (sound) {
        TinyTone(Note_B, 4, 100);
        TinyTone(Note_B, 4, 100);
        sound = false;
      }
      fundir_a_color(1);
      break;
    case 3:
      if (sound) {
        TinyTone(Note_C, 4, 100);
        TinyTone(Note_C, 4, 100);
        sound = false;
      }
      fundir_a_color(2);
      break;
    case 4:
      if (sound) {
        TinyTone(Note_D, 4, 100);
        TinyTone(Note_D, 4, 100);
        sound = false;
      }
      fundir_a_color(3);
      break;
    case 5:
      if (sound) {
        TinyTone(Note_E, 4, 100);
        TinyTone(Note_E, 4, 100);
        sound = false;
      }
      fundir_a_color(4);
      break;
    case 6:
      if (sound) {
        TinyTone(Note_F, 4, 100);
        TinyTone(Note_F, 4, 100);
        sound = false;
      }
      fundir_a_color(5);
      break;
    case 7:
      if (sound) {
        TinyTone(Note_G, 4, 100);
        TinyTone(Note_G, 4, 100);
        sound = false;
      }
      fundir_a_color(6);
      break;
    case 8:
      if (sound) {
        TinyTone(Note_A, 4, 50);
        TinyTone(Note_A, 4, 50);
        TinyTone(Note_B, 4, 50);
        sound = false;
      }
      fundir_a_color(7);
      nuevo_color = true;
      break;
    case 9:
      if (nuevo_color) {
        color1 = random(0, total_colores);
        color2 = random(0, total_colores);
        nuevo_color = false;
      }
      if (sound) {
        TinyTone(Note_C, 4, 50);
        TinyTone(Note_C, 4, 50);
        TinyTone(Note_B, 4, 50);
        sound = false;
      }
      efecto_leds(4, color1, color2);
      break;
    case 10:
      if (sound) {
        TinyTone(Note_D, 4, 50);
        TinyTone(Note_D, 4, 50);
        TinyTone(Note_B, 4, 50);
        sound = false;
      }
      efecto_leds(1, 0, 0);
      break;
    case 11:
      if (sound) {
        TinyTone(Note_E, 4, 50);
        TinyTone(Note_E, 4, 50);
        TinyTone(Note_B, 4, 50);
        sound = false;
      }
      efecto_leds(1, 1, 0);
      break;
    case 12:
      if (sound) {
        TinyTone(Note_F, 4, 50);
        TinyTone(Note_F, 4, 50);
        TinyTone(Note_B, 4, 50);
        sound = false;
      }
      efecto_leds(1, 2, 0);
      break;
    case 13:
      if (sound) {
        TinyTone(Note_G, 4, 50);
        TinyTone(Note_G, 4, 50);
        TinyTone(Note_B, 4, 50);
        sound = false;
      }
      efecto_leds(1, 3, 0);
      break;
    case 14:
      if (sound) {
        TinyTone(Note_A, 4, 50);
        TinyTone(Note_B, 4, 50);
        TinyTone(Note_C, 4, 50);
        sound = false;
      }
      efecto_leds(1, 4, 0);
      break;
    case 15:
      if (sound) {
        TinyTone(Note_D, 4, 50);
        TinyTone(Note_E, 4, 50);
        TinyTone(Note_F, 4, 50);
        sound = false;
      }
      efecto_leds(1, 5, 0);
      break;
    case 16:
      if (sound) {
        TinyTone(Note_G, 4, 50);
        TinyTone(Note_A, 4, 50);
        TinyTone(Note_B, 4, 50);
        sound = false;
      }
      efecto_leds(1, 6, 0);
      break;
    case 17:
      if (sound) {
        TinyTone(Note_B, 4, 50);
        TinyTone(Note_B, 4, 50);
        TinyTone(Note_D, 4, 50);
        sound = false;
      }
      efecto_leds(1, 7, 0);
      break;
    case 18:
      if (sound) {
        TinyTone(Note_C, 4, 50);
        TinyTone(Note_C, 4, 50);
        TinyTone(Note_D, 4, 50);
        sound = false;
      }
      efecto_leds(2, 0, 0);
      break;
    case 19:
      if (sound) {
        TinyTone(Note_D, 4, 50);
        TinyTone(Note_D, 4, 50);
        TinyTone(Note_G, 4, 50);
        sound = false;
      }
      efecto_leds(2, 1, 0);
      break;
    case 20:
      if (sound) {
        TinyTone(Note_F, 4, 50);
        TinyTone(Note_F, 4, 50);
        TinyTone(Note_A, 4, 50);
        sound = false;
      }
      efecto_leds(2, 2, 0);
      break;
    case 21:
      if (sound) {
        TinyTone(Note_A, 4, 50);
        TinyTone(Note_G, 4, 50);
        TinyTone(Note_A, 4, 50);
        sound = false;
      }
      efecto_leds(2, 3, 0);
      break;
    case 22:
      if (sound) {
        TinyTone(Note_A, 4, 50);
        TinyTone(Note_F, 4, 50);
        TinyTone(Note_A, 4, 50);
        sound = false;
      }
      efecto_leds(2, 4, 0);
      break;
    case 23:
      if (sound) {
        TinyTone(Note_E, 4, 50);
        TinyTone(Note_F, 4, 50);
        TinyTone(Note_E, 4, 50);
        sound = false;
      }
      efecto_leds(2, 5, 0);
      break;
    case 24:
      if (sound) {
        TinyTone(Note_D, 4, 50);
        TinyTone(Note_C, 4, 50);
        TinyTone(Note_D, 4, 50);
        sound = false;
      }
      efecto_leds(2, 6, 0);
      break;
    case 25:
      if (sound) {
        TinyTone(Note_F, 4, 50);
        TinyTone(Note_F, 4, 50);
        TinyTone(Note_E, 4, 50);
        TinyTone(Note_A, 4, 150);
        sound = false;
      }
      efecto_leds(2, 7, 0);
      break;
    default:
      click_boton = 1;
      break;
  }


}



void TinyTone(unsigned char divisor, unsigned char octave, unsigned long duration)
{
  TCCR1 = 0x90 | (8 - octave); // for 1MHz clock
  // TCCR1 = 0x90 | (11-octave); // for 8MHz clock
  OCR1C = divisor - 1;       // set the OCR
  delay(duration);
  TCCR1 = 0x90;              // stop the counter
}

// Play a scale
void playTuneStart(void)
{
  TinyTone(Note_C, 4, 100);
  TinyTone(Note_D, 4, 100);
  TinyTone(Note_E, 4, 100);
  TinyTone(Note_F, 4, 100);
  TinyTone(Note_G, 4, 100);
  TinyTone(Note_A, 4, 100);
  TinyTone(Note_B, 4, 100);
  TinyTone(Note_C, 5, 100);
}
