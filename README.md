# TP Integrador — Buffet Albert Einstein

Trabajo Práctico grupal de **Algoritmos y Estructuras de Datos** (UTN FRBA) —
unidad de **archivos binarios en C/C++**.

> **Grupo:** 3
>
> **Integrantes:** 
>
> - Leandro Ricardo Albertoni Borghese - @Learicardo1
> - Tiago Labriola - @TiagoLab
> - Emiliano Gaggino - @Egaggino

## Estructura del repo

```
.
├── enunciado.pdf              # la consigna
├── datos/                     # archivos PROVISTOS por la cátedra (no se tocan)
│   ├── comandas_historicas.dat
│   ├── inventario.dat
│   └── dump_datos_de_prueba.txt   # los dos .dat en texto, para verificar la lectura
├── .gitignore
│
│   # --- estos los crean USTEDES (no vienen en el repo base) ---
├── normalizacion.cpp          # cada uno con su propio main; NO hay main.cpp
├── ventas.cpp
├── cierre.cpp
└── resumen.cpp
```


Compilar y correr cada programa (cada uno tiene su propio `main`, no hay `main.cpp`):

## Comentarios de compilacion:
Los programas .cpp deben estar dentro de la carpeta datos, ya que desde ahi buscan los archivos que necesitan

## Archivo: normalizacion.cpp
Alumno: Tiago Labriola
Normalizacion es el primer programa en ejecutarse. Este consume los archivos:
comandas_historicas e inventarios.
A partir de la lectura de comandas se crea el archivos mozos.dat Luego se recorre de nuevo comandas_historicas para crear los archivos de comandas diarias, y modificar el inventario en cuestión.
Resolucion de partes clave: 
Las contraseñas se someten a una encriptacion que avanza 5 caracteres el escrito por el usuario. Y asi se guarda en el archivo de mozos. Se utiliza un recorrido de la cadena previo a guardar en el archivo.
La creacion de los archivos se dan a partir de las lecturas de "ComandasHistoricas", de ellas se extraen los datos necesarios para la generación de nuevas estructuras, tanto "Mozo" como "Comandas", que son las que se guardan en los dos archivos creados por el modulo.

## Archivo: ventas.cpp
Alumno: Gaggino Emiliano
El programa utiliza el archivo inventario.dat, mozos.dat y comandas_dd-mm-aaaa.dat
El programa pide ingresar la fecha y abre/crea un archivo.
Hecho esto, se crea un nodo en el cual se guardarán las comandas en forma ordenada para evitar posibles corrupciones en el archivo cuando se guarda.
Se pide ID y password del mozo y se comprueba que sean correctas. El password se modifica a nivel ASCII para obtener la clave creada por normalización (Se suma 5 al carácter).
En caso de que el ID y el password sean incorrectos el programa se cierra.
Si es correcto se pide el código de producto. Se verifica que el idProducto exista en el inventario. Si existe se pide la cantidad. 
A la hora de cargar las comandas se toma el idMozo, el idProducto, la cantidad y en base al precio del producto (el cual está en inventario.dat) se crea calcula la comisión, bajando el stock del producto correspondiente.
Luego de esto se cargan en el nodo las comandas existentes (si hubiese alguna en el archivo) y se agrega de forma ordenada a la lista la nueva.
Paso siguiente, la lista es cargada de forma ordenada en el archivo, borrando el contenido previo.
Finalmente, se notifica al usuario que la carga se realizó correctamente y se da la opción de reingresar con el usuario para cargar otro producto.

## Archivo: cierre.cpp
Alumno: Albertoni Borghese, Leandro Ricardo
El programa cierre junta las planillas diarias de una semana (comandas_dd-mm-aaaa.dat) en una sola planilla semanal (comandas_semana_sX-mm.dat), esta planilla semanal tambien va a estar ordenada por idMozo.
Al correr el programa, el usuario carga manualmente el numero de semana y los dias y este le devuelve la planilla semanal.
Resolucion de partes clave: El apareo fue la parte clave de este archivo. Primero se abren los archivos de la semana y se obtienen los primeros registros de cada dia, se comparan estos registros y se elige el que tenga menor idMozo para guardar en la nueva planilla, Es como un apareo entre 2 pero compara con los registros de cada archivo abierto.

## Archivo: resumen.cpp
Alumnos:
Albertoni Borghese, Leandro Ricardo
Gaggino Emiliano
Tiago Labriola
El usuario dicta un numero de semana y un numero de mes, el programa busca una coincidencia con las semanas historicas de normalizacion asi como tambien con las ventas propias del programa de ventas. De hallar una coincidencia reporta por pantalla el total de ventas del mozo, la comision, y el total de ventas del buffet.

```bash
g++ -O2 -o normalizacion normalizacion.cpp
g++ -O2 -o ventas        ventas.cpp
g++ -O2 -o cierre        cierre.cpp
g++ -O2 -o resumen       resumen.cpp
```

Orden de ejecución (cada programa deja los archivos que usa el siguiente):

```
./normalizacion     # historicas + inventario  ->  mozos.dat, comandas_dd-mm-aaaa.dat (varios), inventario actualizado
./ventas            # carga interactiva de la semana  ->  agrega a las planillas del día
./cierre            # junta los días de la semana      ->  comandas_semana_sX-mm.dat
./resumen           # imprime el resumen por mozo + total del buffet
```

## Verificar que leen bien el binario

`datos/dump_datos_de_prueba.txt` tiene el contenido de los dos archivos provistos
en texto legible. Si tu programa imprime otra cosa al leerlos, revisá los `struct`
(orden de campos / tamaños): tienen que dar `sizeof(ComandaHistorica)=76` y
`sizeof(Producto)=64`.

## Qué se entrega

- Este mismo repo (uno por grupo), con los **4 programas** `.cpp` en la raíz
  (cada uno con su propio `main`, no hay `main.cpp`).
- El **README** con el grupo, los integrantes (nombre + usuario de GitHub) y, si
  hace falta, cualquier aclaración de cómo correrlo.
- Los `.dat` de `datos/` son los **provistos por la cátedra** y ya vienen en el
  repo: no los borren ni agreguen otros. Cualquier `.dat` que ustedes copien a la
  raíz o generen al correr **no** se versiona (el `.gitignore` ya los deja afuera).
  La cátedra corre sus programas sobre su propio dataset.
- **Commits repartidos:** cada integrante tiene que tener commits propios a lo
  largo del trabajo. El historial es parte de lo que se mira para la defensa
  individual (no vale un único commit final ni que suba todo una sola persona).
