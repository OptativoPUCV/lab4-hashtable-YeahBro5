#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"


typedef struct HashMap HashMap;
int enlarge_called=0;

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}


void insertMap(HashMap * map, char * key, void * value) {
  if (map == NULL || key ==  NULL)
  {
    return;
  }
  
  long posicion = hash(key, map->capacity);
  
  Pair *newPair = createPair(key, value);
  if (newPair == NULL) return;

  if (map->buckets[posicion] == NULL)
  {
    map->buckets[posicion] = newPair;
  }
  else
  {
    while (map->buckets[posicion] != NULL)
      {
        posicion++;
        if (posicion == map->capacity) posicion = 0;
      }
    
    map->buckets[posicion] = newPair;
  }
  
  map->size += 1;
  map->current = posicion;

  return;

}

void enlarge(HashMap * map) {
    enlarge_called = 1; //no borrar (testing purposes)


}


HashMap * createMap(long capacity) {
    HashMap * map = (HashMap *)malloc(sizeof(HashMap));
    if (map == NULL)
    {
      printf("No se pudo reservar memoria para el mapa\n");
      return NULL;
    }
    else
    {
      map->size = 0;
      map->capacity = capacity;
      map->current = -1;
    }
  
    map->buckets = (Pair **)calloc(map->capacity, sizeof(Pair *));
    if (map->buckets == NULL)
    {
      printf("No se pudo reservar memoria para los buckets del mapa\n");
      return NULL;
    }
  
    return map;
}

void eraseMap(HashMap * map,  char * key) {
  if (map == NULL || key  ==  NULL) return;
  Pair *PairAEliminar = searchMap(map, key);
  if (PairAEliminar == NULL)
  {
    return;
  }

  PairAEliminar->key = NULL;
  map->size -= 1;


}

Pair * searchMap(HashMap * map,  char * key)
{
  
  long posicion =  hash(key, map->capacity);

  Pair *newPair =  map->buckets[posicion];

  if (is_equal(newPair->key, key))
  {
    map->current = posicion;
    return newPair;
  }
  else
  {
    while (map->buckets[posicion] != NULL)
    {
      newPair =  map->buckets[posicion];
      if (is_equal(newPair->key, key))
      {
        map->current = posicion;
        return newPair;
      }
      
      posicion++;
      if (posicion == map->capacity) posicion = 0;
    }
    
  }

  return NULL;

}


Pair * firstMap(HashMap * map) {
  if (map == NULL || map->size < 0) return NULL; 
  for (long i = 0; i <= map->capacity;i++)
    {
      if (map->buckets[i] != NULL && map->buckets[i]->key != NULL)
      {
        map->current = i;
        return map->buckets[i];
      }
    }
    return NULL;
}

Pair * nextMap(HashMap * map) {
  if (map == NULL) return NULL;
  long posicion =  map->current;
  Pair *newPair = map->buckets[posicion];

  for (long i = posicion + 1; i < map->capacity ; i++)
    {
      if (map->buckets[i] != NULL && map->buckets[i]->key != NULL) 
      {
        map->current = i;
        return map->buckets[i];
      }
    }
  
  

    return NULL;
}
