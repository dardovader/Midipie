#include <MIDIUSB.h>

/* Project: MIDI foot controller
*  Author: Samuel Villafranca
*  
* Barcelona 2019
*/

// Elige tu canal MIDI (1-16)

const int CANAL_MIDI = 1;

// ==================== Cambia esto para ajustar el número de botones y pedales de tu montaje ====================

const int NUM_BOTONES   = 10;
const int NUM_ANALOG_IN = 2;

// OJO: Si has cambiado algo aquí tienes que hacer cambios más abajo



// ==================== PINES DE ARDUINO ==========================================================================


// Pines de los botones. Ajusta esto a tu montaje
// Añade más si es necesario
// Si añades más añádelo también en pin_d

const int PIN_B_0 = 2;
const int PIN_B_1 = 3;
const int PIN_B_2 = 4;
const int PIN_B_3 = 5;
const int PIN_B_4 = 6;
const int PIN_B_5 = 7;
const int PIN_B_6 = 8;
const int PIN_B_7 = 9;
const int PIN_B_8 = 10;
const int PIN_B_9 = 11;

// Pines de los controles analógicos. Ajustra esto a tu montaje
// Añade más si es necesario
// Si añades más añádelo también en pin_a

const int PIN_A_0 = A0;
const int PIN_A_1 = A1;

// Ajuste de los controladores analógicos:

// (MIN,MAX,HIST)

const int sensor_a_hist[NUM_ANALOG_IN] = {
  2,
  2
};

const int cc_a_min[NUM_ANALOG_IN] = {
  0,
  0
};


const int cc_a_max[NUM_ANALOG_IN] = {
  127,
  127,
  
};




// ====================  CONTROLADORES: Aquí van la configuracion de los CC ====================

// Cambia estos valores por el número de tus controles digitales

const int CC_0 = 1;
const int CC_1 = 2;
const int CC_2 = 4;
const int CC_3 = 7;
const int CC_4 = 11;


// Cambia estos valores por el número de tus controles analógicos

const int CC_A0 = 12;
const int CC_A1 = 13;



// ==================== Si no has añadido ni quitado botones o penales no hay nada que cambiar más abajo. ====================

int pin_d[NUM_BOTONES] = {
  PIN_B_0,
  PIN_B_1,
  PIN_B_2,
  PIN_B_3,
  PIN_B_4
};

int controles_d[NUM_BOTONES] = {
  CC_0,
  CC_1,
  CC_2,
  CC_3,
  CC_4
};

int pin_a[NUM_ANALOG_IN] = {
  PIN_A_0,
  PIN_A_1
};

int controles_a[NUM_ANALOG_IN] = {
  CC_A0,
  CC_A1
};

// ------------  No hay mas configuracion mas alla de esta linea -------------
// Por favor, manten tus zarpas alejadas.
// Have a nice day!


int sensor_d[NUM_BOTONES];
int sensor_d_antes[NUM_BOTONES];

int sensor_a[NUM_ANALOG_IN]; 
int sensor_a_antes[NUM_ANALOG_IN]; 

void setup() {
  for (int i=0; i<NUM_BOTONES; i++) {
    pinMode(pin_d[i],INPUT_PULLUP);
    sensor_d[NUM_BOTONES]=0;
    sensor_d_antes[NUM_BOTONES]=0;
  } 

  for (int j= 0 ; j < NUM_ANALOG_IN; j++) {
    sensor_a[j] = map(analogRead(pin_a[j]),0,1023,cc_a_min[j],cc_a_max[j]);
    sensor_a_antes[j] = sensor_a[j];
  }
  
  Serial.begin(9600);
}

   
void loop() {

  gestionarBotones();
  gestionarPedal();
  
  delay(30);
}


void gestionarBotones() {
    for (int i =0; i<NUM_BOTONES; i++) {
    int valor = 23;
    sensor_d[i] =digitalRead(pin_d[i]);
    
    if (sensor_d[i] == 0) {
      valor=127;
    } else {
      valor = 0;
    }
    
    if (sensor_d[i] != sensor_d_antes[i]) {
      controlChange(CANAL_MIDI,controles_d[i],valor);
      sensor_d_antes[i]=sensor_d[i];
      MidiUSB.flush();

      Serial.print("Control "); Serial.print(controles_d[i]); Serial.print(" enviado"); Serial.print(" ; valor "); Serial.println(valor);
    }
  }
}

void gestionarPedal() {
  
  for (int i= 0 ; i < NUM_ANALOG_IN; i++) {
    sensor_a[i] = map(analogRead(pin_a[i]),0,1023,cc_a_min[i],cc_a_max[i]);

    if ((sensor_a[i]-sensor_a_hist[i] < sensor_a[i]) || (sensor_a[i]+sensor_a_hist[i] > sensor_a[i])) {
      controlChange(CANAL_MIDI,controles_a[i],sensor_a);
      sensor_a_antes[i] = sensor_a[i];
      MidiUSB.flush();
    }
  }
}

void controlChange(byte channel, byte control, byte value) {
  midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};
  MidiUSB.sendMIDI(event);
}
