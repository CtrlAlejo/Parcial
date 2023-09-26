// Definir los pines de control del 74HC595
const int pin_de_datos = 5;   // Pin de datos (SER)
const int pin_de_reloj = 6;  // Pin de reloj (SRCLK)
const int pin_de_almacenamiento = 7; //Pin de almacenamiento (RCLK)
int filas [8];
int columnas [8];
int seleccion = 0;
unsigned long col2[8];
unsigned long fil2[8];
void imagen(unsigned long (&col2)[8], unsigned long (&col)[8]);
void generacion_de_filas(char (&fila)[8][8], char columna[8][8]);
void patron1(unsigned long (&columnas2)[8], unsigned long (&filas2)[8]);
void patron2(unsigned long (&columnas2)[8], unsigned long (&filas2)[8]);
void patron3(unsigned long (&columnas2)[8], unsigned long (&filas2)[8]);
void patron4(unsigned long (&columnas2)[8], unsigned long (&filas2)[8]);
void generacion_de_filas_2(char **fila, char **columna);
void setup(){
  Serial.begin(9600);
  // Configurar los pines como salidas
  pinMode(pin_de_datos, OUTPUT);
  pinMode(pin_de_reloj, OUTPUT);
  pinMode(pin_de_almacenamiento, OUTPUT);
}

void loop()
{
  publik();
}

void verificacion()
{
  int ON_OFF, Secuencias;
  Serial.println("Ingrese el tiempo de encendido / apagado: ");
  while (Serial.available() == 0){}//Verifica que no hayan datos en el buffer
  ON_OFF = Serial.parseInt();
  ON_OFF *= 1000;
  Serial.println("Ingrese cuantas secuencias desea ver: ");
  while (Serial.available() == 0){}
  Secuencias = Serial.parseInt();
  for (int n = 0; n < Secuencias; n++)
  { //Apaga y prende los 64 leds hasta que "n" sea igual al numero de Secuencias
    matriz_completa(255,0);
    delay(ON_OFF);
    matriz_completa(0,0);
    delay(ON_OFF);
  }
}

void imagen(unsigned long (&col2)[8], unsigned long (&fil2)[8], unsigned long delay)
{ /*
	Argumentos --> col2 = Contiene el arreglo vacio de columnas
    			   fil2 = Contiene el arreglo vacio de filas
  */
  char signo, bits_fil[9], bits_col[9]; int cont = 0; char col1[8][8], fil1[8][8]; unsigned long tiempoInicial;
  Serial.println("Ingrese 1 para los leds encendidos y 0 para los leds apagados");
  do{
    for (int i = 0; i < 8; i++) //Se ejecuta hasta que el usuario ingrese todo el patron de la matriz
    {
      while (Serial.available() == 0){}
  	  signo = Serial.read();
      Serial.print(signo);
      *(*(col1+cont)+i) = signo;
      Serial.print(" ");
    }
    Serial.print("\n");
    cont++;
  } while (cont < 8);
  generacion_de_filas(fil1, col1);
  for (int n = 0; n < 8; n++)
  {
    for (int a = 0; a < 8; a++)//Toma los elementos de cada arreglo de la matriz
    {
   		bits_fil[a] = fil1[n][a];
        bits_col[a] = col1[n][a];
    }
    bits_fil[8] = '\0';
    bits_col[8] = '\0';
    *(fil2+n) = conversion_de_char_a_int (bits_fil);
    *(col2+n) = conversion_de_char_a_int (bits_col);
    *(fil2+n) = binarioADecimal(*(fil2+n));
    *(col2+n) = binarioADecimal(*(col2+n));
  }
  for (int b = 0; b < 500; b++){
    tiempoInicial = millis();
    while (millis() - tiempoInicial < delay) { // Continuar mientras no se haya alcanzado el tiempo máximo
       patron(fil2, col2);
    }
    tiempoInicial = millis();
    while (millis() - tiempoInicial < delay) { // Continuar mientras no se haya alcanzado el tiempo máximo
       matriz_completa(0, 0);
    }
  }
}

void matriz_completa(int columnas, int filas)
{ //Acciona todos los leds de la matriz
  digitalWrite(pin_de_reloj, LOW);
  shiftOut(pin_de_datos, pin_de_almacenamiento, LSBFIRST,columnas);
  shiftOut(pin_de_datos, pin_de_almacenamiento, LSBFIRST,filas);
  digitalWrite(pin_de_reloj, HIGH);
}



void patron(unsigned long filas[], unsigned long columnas[])
{ //Muestra un patron de leds especifico
  /*
  Argumentos --> filas: Contiene el arreglo con los numeros que corresponden a las filas de la matriz
                 columnas: Contiene el arreglo con los numeros que corresponden a las columnas de la matriz
  */
  for (int i = 0; i < 8; i++)
  {
    digitalWrite(pin_de_reloj, LOW);
    shiftOut(pin_de_datos, pin_de_almacenamiento, LSBFIRST,columnas[i]);
    shiftOut(pin_de_datos, pin_de_almacenamiento, LSBFIRST,filas[i]);
    digitalWrite(pin_de_reloj, HIGH);
  }
}

void patron1(unsigned long (&columnas2)[8], unsigned long (&filas2)[8], unsigned long delay){
    char **columnas1, **filas1; char bits_col[9], bits_fil[9]; int cont = 3, cont2 = 5; unsigned long tiempoInicial;
    columnas1 = crear_matriz(8, 8);
    filas1 = crear_matriz(8, 8);
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            if (i < 4){ //Condicional de la primera mitad
                if (j < cont){
                    *(*(columnas1 + i)+ j) = '0';
                } else if (j < cont2){
                    *(*(columnas1 + i)+ j) = '1';
                } else {
                    *(*(columnas1 + i)+ j) = '0';
                }
            }
            else if (i >= 4){//Condicional de la segunda mitad
                if (j < cont){
                    *(*(columnas1 + i)+ j) = '0';
                } else if (j < cont2){
                    *(*(columnas1 + i)+ j) = '1';
                } else {
                    *(*(columnas1 + i)+ j) = '0';
                }
            }
        }
        if (i < 3){
            cont -= 1;
            cont2 += 1;
        } else if (i >= 4){
            cont += 1;
            cont2 -= 1;
        }
    }
    generacion_de_filas_2(filas1, columnas1);
    for (int n = 0; n < 8; n++)
    {
        for (int a = 0; a < 8; a++)//Toma los elementos de cada arreglo de la matriz
        {
            bits_fil[a] = filas1[n][a];
            bits_col[a] = columnas1[n][a];
        }
        bits_fil[8] = '\0';
        bits_col[8] = '\0';
        *(filas2+n) = conversion_de_char_a_int (bits_fil);
        *(columnas2+n) = conversion_de_char_a_int (bits_col);
        *(filas2+n) = binarioADecimal(*(filas2+n));
        *(columnas2+n) = binarioADecimal(*(columnas2+n));
    }
  tiempoInicial = millis();
  while (millis() - tiempoInicial < delay) { // Continuar mientras no se haya alcanzado el tiempo máximo
    patron(fil2, col2);
  }
  matriz_completa(0, 0);
  eliminacion_de_matriz(columnas1, 8);
  eliminacion_de_matriz(filas1, 8);
}

void patron2(unsigned long (&columnas2)[8], unsigned long (&filas2)[8], unsigned long delay) {
    char **filas1, **columnas1, bits_col[9], bits_fil[9]; unsigned long tiempoInicial;
    columnas1 = crear_matriz(8, 8);
    filas1 = crear_matriz(8, 8);
    int cont = 0;
    int cont2 = 7;

    // Configurar las matrices de columnas para formar una "X"
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (j == cont || j == cont2) {
                columnas1[i][j] = '1';
            } else {
                columnas1[i][j] = '0';
            }
        }
        cont++;
        cont2--;
    }

    // Generar las matrices de filas basadas en las columnas
    generacion_de_filas_2(filas1, columnas1);

    // Convertir las matrices de caracteres en valores enteros
    for (int n = 0; n < 8; n++) {
        for (int a = 0; a < 8; a++) {
            bits_fil[a] = filas1[n][a];
            bits_col[a] = columnas1[n][a];
        }
        bits_fil[8] = '\0';
        bits_col[8] = '\0';
        *(filas2 + n) = conversion_de_char_a_int(bits_fil);
        *(columnas2 + n) = conversion_de_char_a_int(bits_col);
        *(filas2 + n) = binarioADecimal(*(filas2 + n));
        *(columnas2 + n) = binarioADecimal(*(columnas2 + n));
    }

    // Mostrar la "X" en la matriz de LEDs
  tiempoInicial = millis();
  while (millis() - tiempoInicial < delay) { // Continuar mientras no se haya alcanzado el tiempo máximo
    patron(fil2, col2);
  }
  matriz_completa(0, 0);
  eliminacion_de_matriz(columnas1, 8);
  eliminacion_de_matriz(filas1, 8);
}

void patron3(unsigned long (&columnas2)[8], unsigned long (&filas2)[8], unsigned long delay){
    char **filas1, **columnas1, bits_col[9], bits_fil[9]; int cont = 0, cont2 = 0; unsigned long tiempoInicial;
    columnas1 = crear_matriz(8, 8);
    filas1 = crear_matriz(8, 8);
    for (int i = 0; i < 8; i++){
        for (int j = 0; j < 8; j++){
            if (cont2 < 2){
                if (cont < 2){
                    *(*(columnas1 + i)+j) = '1';
                    cont++;
                } else {
                    *(*(columnas1 + i)+j) = '0';
                    cont = 0;
                }
            } else if (cont2 >= 2) {
                if (cont % 3 == 0){
                    *(*(columnas1 + i)+j) = '0';
                    cont++;
                } else {
                    *(*(columnas1 + i)+j) = '1';
                    cont++;
                }
            }
        }
        cont2++;
        cont = 0;
        if (cont2 == 4){
            cont2 = 0;
        }
    }
    generacion_de_filas_2(filas1, columnas1);
    for (int n = 0; n < 8; n++)
    {
        for (int a = 0; a < 8; a++)//Toma los elementos de cada arreglo de la matriz
        {
            bits_fil[a] = filas1[n][a];
            bits_col[a] = columnas1[n][a];
        }
        bits_fil[8] = '\0';
        bits_col[8] = '\0';
        *(filas2+n) = conversion_de_char_a_int (bits_fil);
        *(columnas2+n) = conversion_de_char_a_int (bits_col);
        *(filas2+n) = binarioADecimal(*(filas2+n));
        *(columnas2+n) = binarioADecimal(*(columnas2+n));
    }
  tiempoInicial = millis();
  while (millis() - tiempoInicial < delay) { // Continuar mientras no se haya alcanzado el tiempo máximo
    patron(fil2, col2);
  }
  matriz_completa(0, 0);
  eliminacion_de_matriz(columnas1, 8);
  eliminacion_de_matriz(filas1, 8);
}

void patron4(unsigned long (&columnas2)[8], unsigned long (&filas2)[8], unsigned long delay){
    char **filas1, **columnas1, bits_col[9], bits_fil[9]; unsigned long tiempoInicial;
    int cont = 0;
    int cont2 = 4;
    columnas1 = crear_matriz(8, 8);
    filas1 = crear_matriz(8, 8);
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if(i < 4){
                if (j < cont) {
                    columnas1[i][j] = '0';
                } else if (j < cont2) {
                    columnas1[i][j] = '1';
                } else {
                    columnas1[i][j] = '0';
                }
            }
            else if (i >= 4){
                columnas1[i][j] = columnas1[7 - i][j];
            }

        }
        cont++;
        cont2++;
    }

    generacion_de_filas_2(filas1, columnas1);

    for (int n = 0; n < 8; n++) {
        for (int a = 0; a < 8; a++) {
            bits_fil[a] = filas1[n][a];
            bits_col[a] = columnas1[n][a];
        }
        bits_fil[8] = '\0';
        bits_col[8] = '\0';
        *(filas2 + n) = conversion_de_char_a_int(bits_fil);
        *(columnas2 + n) = conversion_de_char_a_int(bits_col);
        *(filas2 + n) = binarioADecimal(*(filas2 + n));
        *(columnas2 + n) = binarioADecimal(*(columnas2 + n));
    }
  tiempoInicial = millis();
  while (millis() - tiempoInicial < delay) { // Continuar mientras no se haya alcanzado el tiempo máximo
    patron(fil2, col2);
  }
  matriz_completa(0, 0);
  eliminacion_de_matriz(columnas1, 8);
  eliminacion_de_matriz(filas1, 8);
}

char** crear_matriz(int filas, int columnas){
    char **matriz_dinamica = new char *[filas];
    for (int i = 0; i < filas; i++) { //Crea los arreglos internos dinamicos
        matriz_dinamica[i] = new char[columnas];
    }
    return matriz_dinamica;
}

void eliminacion_de_matriz(char** matriz, int filas){
    //Recibe la matriz dinamica y la elimina de memoria
    for (int i = 0; i < filas; i++) {
        delete[] matriz[i];
    }
    delete[] matriz;
}

void generacion_de_filas_2(char **fila, char **columna)
{ //Genera la matriz de filas de la matriz de LEDs a partir de la la matriz de columnas.
    int cont = 0;
    for (int b = 0; b < 8; b++)
    {
        while (b > cont){
            *(*(fila+b)+cont++) = '1';
        }
        for (int c = 0, d = cont; c < 8; c++, d++){//Transforma el conjunto de bits si en un arreglo de columnas esta el '1'
            if (*(*(columna+b)+c) == '1'){
                *(*(fila+b)+cont++) = '0';
                break;
            }
        }
        while (8 > cont){
            *(*(fila+b)+cont++) = '1';
        }
        cont = 0;
    }
}

unsigned long conversion_de_char_a_int(const char *cadena)
{
    //Recibe una cadena de caracteres numericos y la convierte a enteros
    unsigned long numero = 0, i = 0;
    if (cadena[0] == '-') {
        i = 1;
    }
    while(cadena[i] != '\0'){
        if(cadena[i] >= '0' && cadena[i] <= '9'){
            numero = numero * 10 + (cadena[i] - '0');
        }
        else{
            break;
        }
        i++;
    }
    return numero;
}

unsigned long binarioADecimal(long binario)
{ //Convierte un numero binario a decimal
    //Argumentos --> binario: Es el numero binario que sera convertido a decimal
    unsigned long decimal = 0;
    int base = 1;  // Inicializar la base en 2^0 (1)

    while (binario > 0) {
        int digito = binario % 10;   // Obten el último dígito del número binario
        decimal = decimal + digito * base;
        binario = binario / 10;       // Elimina el último dígito del número binario
        base = base * 2;             // Incrementa la base a la siguiente potencia de 2
    }

    return decimal;
}

void patrones(int retardo){
  patron1(col2, fil2, retardo);
  delay(350);
  patron2(col2, fil2, retardo);
  delay(350);
  patron3(col2, fil2, retardo);
  delay(350);
  patron4(col2, fil2, retardo);
}

void publik(){
  int seleccion, retardo; unsigned long delay;
  Serial.println("Ingrese el modo que desea ejecutar:");
  Serial.println("1. Comprobar el funcionamiento de los LEDs de la matriz 8x8");
  Serial.println("2. Ingresar un patron para mostrar en la matriz de LEDs");
  Serial.println("3. Mostrar la secuencia de 4 patrones");
  while (Serial.available() == 0){}
  seleccion = Serial.parseInt();
  switch (seleccion){
  	case 1:
  	  verificacion();
      break;
    case 2:
      Serial.println("Ingrese el tiempo de encendido/apagado del patron:");
      while (Serial.available() == 0){}
      retardo = Serial.parseInt();
      delay = retardo * 1000;
      imagen(fil2, col2, delay);
      break;
  	case 3:
      Serial.println("Ingrese el tiempo de retardo entre visualizaciones:");
      while (Serial.available() == 0){}
      retardo = Serial.parseInt();
      delay = retardo * 1000;
      patrones(delay);
      break;
  	default:
  	  break;
  }
}

void eliminacion_de_matriz(int** matriz, int filas){
    //Recibe la matriz dinamica y la elimina de memoria
    for (int i = 0; i < filas; i++) {
        delete[] matriz[i];
    }
    delete[] matriz;
}

void generacion_de_filas(char (&fila)[8][8], char columna[8][8])
{ //Genera la matriz de filas de la matriz de LEDs a partir de la la matriz de columnas.
  int cont = 0;
  for (int b = 0; b < 8; b++)
  {
    while (b > cont){
      *(*(fila+b)+cont++) = '1'; 
    }
    for (int c = 0, d = cont; c < 8; c++, d++){//Transforma el conjunto de bits si en un arreglo de columnas esta el '1'
      if (*(*(columna+b)+c) == '1'){
        *(*(fila+b)+cont++) = '0';
        break;
      }
    }
    while (8 > cont){
      *(*(fila+b)+cont++) = '1';
    }
    cont = 0;
  }
}