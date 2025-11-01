# 🎨 Juego Colorette

Una adaptación del clásico juego de cartas **Coloretto**, desarrollada en **C++** para consola.  
Compite con tus amigos localmente y demuestra tu estrategia para conseguir la mejor combinación de colores.  

Incluye **sistema de guardado y carga de partidas**, **administrador de partidas guardadas**, y un completo **menú interactivo** con reglas integradas.

Ideal para practicar **programación estructurada y orientada a objetos** en C++ y manejo de archivos.

---

## 🚀 Instalación

### 1️⃣ Clona o descarga este repositorio

```bash
git clone https://github.com/Santiagox01/estructura_2025
2️⃣ Compila el juego
bash
Copiar código
make
Nota: Asegúrate de que tu compilador soporte C++11 o superior.

🕹️ Uso
Ejecuta el programa compilado

bash
Copiar código
make run
o directamente:

bash
Copiar código
./Colorette
Menú Principal:

🎮 Nueva Partida → Inicia una nueva sesión local (3 a 5 jugadores humanos)

💾 Cargar Partida → Reanuda una partida guardada

🗂️ Administrar Partidas → Lista y elimina partidas guardadas

🚪 Salir → Cierra el juego

Nueva Partida:

Elige el número de jugadores (de 3 a 5)

Ingresa un nombre para la partida (sin espacios)

Se mostrarán las reglas completas del juego

Presiona Enter para comenzar

Durante la Partida:

Cada jugador puede robar una carta del mazo y colocarla en una fila

O bien, tomar una fila completa de cartas disponibles

Cuando todos los jugadores han tomado una fila, inicia una nueva ronda

La carta “Última Ronda” marca el final del juego

Puntuación Final:

Cada jugador elige 3 colores positivos, los demás se cuentan como negativos

Las cartas +2 suman puntos extra

El sistema aplica la tabla oficial de puntuación automáticamente

Guardado y Carga:

Las partidas se almacenan en la carpeta saves/

Puedes cargarlas o eliminarlas desde el menú principal

✨ Características
🃏 Modo Local Multijugador — Hasta 5 jugadores humanos en una sola consola
💾 Sistema de Guardado Completo — Guarda y reanuda partidas fácilmente
📚 Reglas Integradas — Explicación detallada antes de comenzar
🧠 Estrategia Pura — Elige bien tus colores positivos y negativos
🗂️ Gestor de Partidas — Lista, carga o elimina tus partidas guardadas

🧩 Arquitectura del Proyecto
🔹 Clases Principales
🟨 Juego
Responsabilidad: Controla la lógica general de las partidas.

Métodos principales:

inicializarMazo(int jugadores)

barajar()

crearJugadores(int jugadores)

repartirCartasIniciales()

ejecutarJuegoConGuardado(string nombrePartida)

cargarPartida(string ruta)

🟩 SaveManager
Responsabilidad: Gestiona el guardado y la eliminación de partidas.

Métodos principales:

listarPartidas()

eliminarPartida(string nombre)

guardarPartida(string nombre, Estado estado)

🟦 Jugador
Responsabilidad: Representar a un jugador humano.

Atributos:

nombre, cartas

Métodos:

tomarFila()

elegirColorPositivo()

🟥 Carta
Responsabilidad: Representa una carta individual.

Atributos:

color, valor

Ejemplo: cartas de color o carta especial “+2” o “Última Ronda”.

🟪 Mazo
Responsabilidad: Gestiona todas las cartas del juego.

Métodos principales:

barajar()

robarCarta()

🟫 FilaDeJuego
Responsabilidad: Representar las filas de cartas en la mesa.

Métodos principales:

agregarCarta(Carta c)

estaCompleta()

📂 Estructuras de Datos
EstadoPartida
Estructura que almacena el progreso completo del juego:

mazo: Cartas restantes

jugadores: Lista de jugadores y sus cartas

filas: Estado actual de las filas

rondaActual: Número de ronda

ultimaRonda: Indica si es la ronda final

🔁 Ejemplo de Flujo de Juego
Se eligen 4 jugadores y se asigna un nombre de partida

Se prepara el mazo según las reglas del número de jugadores

Cada jugador en su turno elige entre robar o tomar una fila

Se repite hasta que todos hayan tomado una fila

Al llegar la Última Ronda, se calcula la puntuación

Se guardan los resultados automáticamente

🤝 Contribuciones
Si deseas mejorar el proyecto, contáctanos:

@Santiagox01

@YForondaa

@jaiderehaco-eng

👨‍💻 Desarrollado por
Estudiantes del ITM:

Santiago Jaramillo Valencia

Yenifer Foronda Hernández

Jayder Alejandro Arias Arango

🎨 ¡Disfruta de Colorette y demuestra tu mejor estrategia de colores!

yaml
Copiar código
