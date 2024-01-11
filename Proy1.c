/*Universidad Católica Andrés Bello
Algoritmos y estructuras de datos
Aliendre Ignacio - Palma Alfonso */

/*Redistribución Ordenada del Acceso a Determinados Sitios o R.O.A.D.S BD*/

#include <stdio.h>
#include <stdlib.h>
#include "Grafos.h"

void main (){


//CARGA INICIAL LEYENDO UN ARCHIVO Y SIN LEER ORIGEN/DESTINO/DISTANCIA/TIEMPO

//MOSTRAR TODAS LAS RUTAS

//MOOSTRAR RUTA MÁS OPTIMA

int MatAd[10][10],i=0,j=0;

for (i=0; i < 10; i++)
{
        for (i=0; j < 10; j++)
        {
        MatAd[i][j]=0;    
    }
}

for (i=0; i < 10; i++)
{
        for (i=0; j < 10; j++)
        {
        printf("%d",MatAd[i][j]);    
    }
}

}