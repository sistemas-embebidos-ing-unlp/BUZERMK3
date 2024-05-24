/*
 * funciones.c
 *
 *  Created on: Apr 15, 2024
 *      Author: Dell
 */
// DE ACA PARA ABAJO DEFINO LOS "DEFINES"
#include "main.h"
#include "funciones.h"

typedef enum {sleep,largada,juego}state_t;
typedef enum {null,boton,gana,pierde,inicio}event_t;

uint32_t ganadores[10]={0};
uint32_t tiempo=0;
state_t estado = sleep;
event_t evento = null;
uint32_t i=0;

#define Gancho GPIOA, GPIO_PIN_1
#define Alambre GPIOA, GPIO_PIN_2
#define LedR GPIOA, GPIO_PIN_3
#define Boton GPIOA, GPIO_PIN_4
#define Arandela GPIOA, GPIO_PIN_5
#define LedV GPIOA, GPIO_PIN_6
#define LedA GPIOA, GPIO_PIN_7
#define ArandelaInicio GPIOB, GPIO_PIN_0
#define Buzzer GPIOB, GPIO_PIN_1
//_______________________________________________________________________________________________________________
//DE ACA PARA ABAJO ARRANCO A DEFINIR FUNCIONES
//-------------------------------------------------------------------------------------------------------------
void PrenderGancho();
void ApagarGancho();
void MensajePerdiste();
void MensajeGanaste();
void Mensajeinicio();
void MensajeinicioMAL();
void LedRojo();
void LedVerde();
void LedAzul();
void Top10();


// Función para configurar el pin 4 como output y ponerlo en alto
void PrenderGancho(void) {
	GPIO_InitTypeDef GPIO_InitStruct;
	// Habilitar el reloj GPIO para el puerto A
	__HAL_RCC_GPIOA_CLK_ENABLE();

	// Configurar el pin A4 como salida
	GPIO_InitStruct.Pin = GPIO_PIN_1;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	// Poner el pin A4 en alto
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);
}

// Función para configurar el pin 4 como input
void ApagarGancho(void) {
	GPIO_InitTypeDef GPIO_InitStruct;
	// Habilitar el reloj GPIO para el puerto A
	__HAL_RCC_GPIOA_CLK_ENABLE();

	// Configurar el pin A4 como entrada
	GPIO_InitStruct.Pin = GPIO_PIN_1;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

//funcion con mensaje de perdiste
void MensajePerdiste(uint32_t tiempo) {
	HAL_GPIO_WritePin(Buzzer, GPIO_PIN_SET);
	char texto[100]; // Definir un buffer para almacenar la cadena

	// limpiamos la huart de forma artificial
	HAL_UART_Transmit(&huart1, "\n \n \n \n \n \n \n \n \n \n \n \n \n \n \n \n \n \n \n \n", strlen("\n \n \n \n \n \n \n \n \n \n \n \n \n \n \n \n \n \n \n \n"), HAL_MAX_DELAY);

	// Manda a la UART el siguiente mensaje
	HAL_UART_Transmit(&huart1, "PERDISTE, tardaste:\n", strlen("PERDISTE, tardaste:\n"), HAL_MAX_DELAY);

	// Manda a la UART el tiempo (en segundos)
	sprintf(texto, "%d.%d segundos \n",tiempo/1000,tiempo % 1000); // Convertir el int a una cadena de caracteres
	HAL_UART_Transmit(&huart1, (uint8_t*)texto, strlen(texto), HAL_MAX_DELAY);


	// limpiamos la huart de forma artificial
	HAL_UART_Transmit(&huart1, "\n \n \n \n \n \n \n \n \n \n \n \n \n \n \n \n", strlen("\n \n \n \n \n \n \n \n \n \n \n \n \n \n \n \n"), HAL_MAX_DELAY);
	HAL_Delay(2000);
	HAL_GPIO_WritePin(Buzzer, GPIO_PIN_RESET);

}

//funcion con mensaje de Ganaste
void MensajeGanaste(uint32_t tiempo) {
	char texto[100];
	// limpiamos la huart de forma artificial / crota jejeje
	HAL_UART_Transmit(&huart1, "\n \n \n \n \n \n \n \n \n \n", strlen("\n \n \n \n \n \n \n \n \n \n"), HAL_MAX_DELAY);

	// Manda a la UART la cadena de caracteres de antes
	HAL_UART_Transmit(&huart1, "GANASTE CHAMPION, tardaste: \n", strlen("GANASTE CHAMPION, tardaste: \n"), HAL_MAX_DELAY);
	sprintf(texto, "%d.%d segundos \n",tiempo/1000,tiempo % 1000); // Convertir el int a una cadena de caracteres
	HAL_UART_Transmit(&huart1, (uint8_t*)texto, strlen(texto), HAL_MAX_DELAY);
}

//funcion con mensaje de inicio
void Mensajeinicio(void) {
HAL_UART_Transmit(&huart1, "\n \n \n \n \n \n \n \n \n \n", strlen("\n \n \n \n \n \n \n \n \n \n"), HAL_MAX_DELAY);

HAL_UART_Transmit(&huart1, "tenes 5 segundos para tocar la arandela de inicio", strlen("tenes 5 segundos para tocar la arandela de inicio"), HAL_MAX_DELAY);

HAL_UART_Transmit(&huart1, "\n \n \n \n \n \n \n \n \n \n", strlen("\n \n \n \n \n \n \n \n \n \n"), HAL_MAX_DELAY);
}

//funcion con mensaje de inicio
void MensajeinicioMAL(void) {
HAL_UART_Transmit(&huart1, "\n \n \n \n \n \n \n \n \n \n", strlen("\n \n \n \n \n \n \n \n \n \n"), HAL_MAX_DELAY);

HAL_UART_Transmit(&huart1, "no tocaste la arandela, volve a iniciar el juego", strlen("no tocaste la arandela, volve a iniciar el juego"), HAL_MAX_DELAY);

HAL_UART_Transmit(&huart1, "\n \n \n \n \n \n \n \n \n \n", strlen("\n \n \n \n \n \n \n \n \n \n"), HAL_MAX_DELAY);
}



void LedRojo(int p) {
	if (p == 1) HAL_GPIO_WritePin(LedR, GPIO_PIN_SET); //prendo el led rojo
	if (p == 0) HAL_GPIO_WritePin(LedR, GPIO_PIN_RESET); //apago el led rojo
}

void LedVerde(int p) {
	if (p == 1) HAL_GPIO_WritePin(LedV, GPIO_PIN_SET); //prendo el led verde
	if (p == 0) HAL_GPIO_WritePin(LedV, GPIO_PIN_RESET); //apago el led verde
}

void LedAzul(int p) {
	if (p == 1) HAL_GPIO_WritePin(LedA, GPIO_PIN_SET); //prendo el led azul
	if (p == 0) HAL_GPIO_WritePin(LedA, GPIO_PIN_RESET); //apago el led azul
}

//funcion de top 10
void Top10(uint32_t tiempo,uint32_t ganadores[]){
	char texto[100];
	int j=0;
	int aux=0;
	int var=0;

	HAL_UART_Transmit(&huart1, "LOS MEJORES PUNTAJES FUERON: \n", strlen("LOS MEJORES PUNTAJES FUERON: \n"), HAL_MAX_DELAY);

	while(var<10){
		if (tiempo>ganadores[var] && ganadores[var]!=0) var++;
		else if(ganadores[var]==0){
			ganadores[var]=tiempo;
			var=0;
			break;
		}
		else if(tiempo<ganadores[var]){
			for (aux=0;aux<9-var;aux++){
				ganadores[9-aux]=ganadores[9-aux-1];
			}
			ganadores[var]=tiempo;
			var=0;
			break;
		}
	}

	for (j = 0; j < 10; ++j) {
		// Convertir el int a una cadena de caracteres
		sprintf(texto, "%d.%d segundos \n",ganadores[j]/1000,ganadores[j] % 1000); // Convertir el int a una cadena de caracteres
		HAL_UART_Transmit(&huart1, (uint8_t*)texto, strlen(texto), HAL_MAX_DELAY);
	}
	// limpiamos la huart de forma artificial / crota jejeje
	HAL_UART_Transmit(&huart1, "\n \n \n \n \n", strlen("\n \n \n \n \n"), HAL_MAX_DELAY);


}

//______________________________________________________________________________________________________________

//DE ACA PARA ARRIBA SON FUNCIONES INTERNAS DEL FUNCIONAMIENTO DEL JUEGO, EL USUSARIO NO LAS VE NUNCA

//DE ACA PARA ABAJO SON LAS FUNCIONES QUE LLAMA EL USUARIO
//_____________________________________________________________________________________________________________


void FuncionInit(){
	estado = sleep;
	evento = null;
}

void FuncionBuzzer(){
	  evento=null;

	  if (HAL_GPIO_ReadPin(Boton) == 1 && estado==sleep) evento=boton;

	  // si el jugador toca la arandela del inicio:
	  if (HAL_GPIO_ReadPin(ArandelaInicio) == 1 && estado==largada) evento=inicio;

	  // si el jugador toca el alambre:
	  if (HAL_GPIO_ReadPin(Alambre) == 1) evento=pierde;

	  // si el jugador gana el juego:
	  if (HAL_GPIO_ReadPin(Arandela) == 1) evento=gana;

		  switch(estado){
		  	  case sleep:
		  		switch(evento){

		  			case boton:
		  				estado=largada;
		  				i= HAL_GetTick();
		  				Mensajeinicio();
		  				PrenderGancho();
		  				HAL_Delay(5000);



		  				break;

		  			default:

		  		}
		  		break;

		  	  case largada:

		  		switch(evento){

		  			  			case inicio:
		  			  				estado=juego;
		  			  				i= HAL_GetTick();
		  			  				LedAzul(1);

		  			  				break;

		  			  			default:
		  			  				MensajeinicioMAL();
		  			  				estado=sleep;
		  			  				LedRojo(1);
		  			  				HAL_Delay(2000);
		  			  				LedRojo(0);
		  			  				break;

		  			  		}
		  		  break;


		  	  case juego:

		  		switch(evento){

		  			case pierde:
		  				tiempo= HAL_GetTick()-i;

		  				LedRojo(1);
		  				LedAzul(0);
		  				MensajePerdiste(tiempo);
		  				HAL_Delay(2000);
		  				ApagarGancho();
		  				LedRojo(0);

		  				i=0; //Reinicio i y tiempo para que se elimine el valor del tiempo guardado
		  				tiempo=0;

		  				estado=sleep;
		  				break;


		  			case gana:
		  				 tiempo= HAL_GetTick()-i;

		  				 ApagarGancho();
		  				 LedVerde(1);
		  				 LedAzul(0);
		  				 MensajeGanaste(tiempo);
		  				 Top10 (tiempo, ganadores);
		  				 HAL_Delay(2000); // un delay para que el led este unos segundos prendidos
		  				 LedVerde(0);

		  				 i=0; //Reinicio i y tiempo para que se elimine el valor del tiempo guardado
		  				 tiempo=0;

		  				 estado=sleep;
		  				 break;

		  			default:
		  		}
		  		break;
			  }
		  }


