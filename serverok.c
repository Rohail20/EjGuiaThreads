#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <mysql.h>
#include <pthread.h>

int contador;

//Estructura necessaria para acceso excluyente
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
void *AtenderCliente (void *socket)
{
	int sock_conn, sock_listen, ret, *s;
	s = (int *) socket;
	sock_conn = *s;
	char peticion[512];
	char respuesta[512];
	char respuesta1[512];
	
	int terminar = 0;
	while (terminar == 0)
	{//Ahora recibimos su nombre, que dejamos en el buf
		ret = read(sock_conn, peticion, sizeof(peticion));
		printf("Recibido\n");
		
		//Tenemos que aￃﾃￂﾱadir la marca de fin de string para que no escriba lo que hay en el buffer
		peticion[ret] = '\0';
		
		//Escribinos el nombre en consola
		
		printf("Peticion: %s\n", peticion);
		
		//Vamos a ver que nos pide la peticion
		char* p = strtok(peticion, "/");
		char codigo[10];
		char NOMBRE[50];
		char PASSWORD[15];
		if (codigo != 0)
		{
			p = strtok(NULL, "/");
			
			strcpy(NOMBRE, p);
			printf("Contrase￱a: %s, Nombre: %s\n", codigo, NOMBRE);
		}
		if (codigo == 0) //peticion de desconexion
		{
			terminar = 1;
		}
		else if (codigo == 1)
		{
			p = strtok(NULL, "/");
			strcpy(PASSWORD, p);
			printf("DNI: %s, Nombre: %s, Password: %s\n", codigo, NOMBRE, PASSWORD);
			
			MYSQL* conn;
			int err;
			
			MYSQL_RES* resultado;
			MYSQL_ROW row;
			
			char consulta[100];
			
			conn = mysql_init(NULL);
			if (conn == NULL) {				printf("Error al crear la conexion: %u %s\n",
												   mysql_errno(conn), mysql_error(conn));
			exit(1);
			}
			
			conn = mysql_real_connect(conn, "localhost", "root", "mysql", "JUEGO", 0, NULL, 0);
			if (conn == NULL) {
				printf("Error al inicializar la conexion: %u %s\n",
					   mysql_errno(conn), mysql_error(conn));
				exit(1);
			}
			
			sprintf(consulta, "SELECT Jugadores.Nombre, Jugadores.Password FROM Jugadores WHERE (Jugadores.Nombre='%s' AND Jugadores.Password='%s')", NOMBRE, PASSWORD);
			
			err = mysql_query(conn, consulta);
			if (err != 0) {
				printf("Error al consultar datos de la base %u %s\n",
					   mysql_errno(conn), mysql_error(conn));
				exit(1);
			}
			printf("aqui\n");
			resultado = mysql_store_result(conn);
			
			printf("aqui\n", resultado);
			
			row = mysql_fetch_row(resultado);
			printf(row[0]);
			if (row[0] == NULL)
			{
				printf("No se han obtenido datos en la consulta\n");
				strcpy(respuesta1, "NO");
			}
			
			else
			{
				strcpy(respuesta1, "SI");
			}
			
			write(sock_conn, respuesta1, strlen(respuesta1));
			
			mysql_close(conn);
		}
		else if (codigo == 2)
		{
			sprintf(respuesta, "%d", DamePartidasGanadas(NOMBRE));
			write(sock_conn, respuesta, strlen(respuesta));
		}
		else if (codigo == 3)
		{
			sprintf(respuesta, "%d", FechaPartidas(NOMBRE));
			write(sock_conn, respuesta, strlen(respuesta));
		}
		else if (codigo == 4)
		{
			sprintf(respuesta, "%d", DuracionPartidas(NOMBRE));
			write(sock_conn, respuesta, strlen(respuesta));
		}
		printf("Respuesta: %s\n", respuesta);
		//lo enviamos
		
		if ((codigo == 2) || (codigo == 3) || (codigo == 4))
		{
			pthread_mutex_lock(&mutex); //no interrumpas
			contador = contador + 1;
			pthread_mutex_unlock(&mutex); //ya puedes interrumpir
		}
	}
	close(sock_conn);
}
int main(int argc, char* argv[])
{
	int sock_conn, sock_listen;
	struct sockaddr_in serv_adr;
	int ret;
	char peticion[512];
	char respuesta1[512];
	char respuesta[512];
	//abrimos socket
	if ((sock_listen = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		printf("Error creant socket");
	int terminar = 0;
	while(terminar == 0)
	{
		//Ahora recibimos su nombre, que dejamos en el buf
		ret=read(sock_conn,peticion, sizeof(peticion));
		printf ("Recibido\n");
		
		//Tenemos que aￃﾃￂﾱadir la marca de fin de string para que no escriba lo que hay en el buffer
		peticion[ret]='\0';
		
		//Escribinos el nombre en consola
		
		printf("Peticion: %s\n", peticion);
		
		//Vamos a ver que nos pide la peticion
		char *p = strtok(peticion, "/");
		char codigo[10];
		char NOMBRE[50];
		char PASSWORD[15];
		if (codigo !=0)
		{
			p = strtok (NULL, "/");
			
			strcpy(NOMBRE, p);
			printf ("Codigo: %s, Nombre: %s\n", codigo, NOMBRE);
		}
		if (codigo ==0) //peticion de desconexion
		{
			terminar = 1;
		}
		else if(codigo == 1)
		{
			p = strtok (NULL, "/");
			strcpy(PASSWORD, p);
			printf ("Codigo: %s, Nombre: %s, Password: %s\n", codigo, NOMBRE, PASSWORD);
			
			MYSQL *conn;
			int err;
			
			MYSQL_RES *resultado;
			MYSQL_ROW row;
			
			char consulta [100];
			
			conn = mysql_init(NULL);
			if (conn==NULL) {
				printf ("Error al crear la conexion: %u %s\n", 
						mysql_errno(conn), mysql_error(conn));
				exit (1);
				
			}
			
			conn = mysql_real_connect (conn, "localhost","root", "mysql", "JUEGO",0, NULL, 0);
			if (conn==NULL) {
				printf ("Error al inicializar la conexion: %u %s\n", 
						mysql_errno(conn), mysql_error(conn));
				exit (1);
			}
			
			sprintf(consulta, "SELECT JUGADOR.NOMBRE, JUGADOR.PASSWORD FROM JUGADOR WHERE (JUGADOR.NOMBRE='%s' AND JUGADOR.PASSWORD='%s')", NOMBRE, PASSWORD);
			err=mysql_query (conn, consulta);
			if (err!=0) {
				printf ("Error al consultar datos de la base %u %s\n",
						mysql_errno(conn), mysql_error(conn));
				exit (1);
			}
			printf("aqui\n");
			resultado = mysql_store_result (conn);
			
			printf("aqui\n",resultado);
			
			row = mysql_fetch_row (resultado);
			printf(row[0]);
			if (row[0] == NULL)
			{
				printf ("No se han obtenido datos en la consulta\n");
				strcpy(respuesta1, "NO");
			}
			
			else
			{	
				strcpy(respuesta1, "SI");
			}
			
			write (sock_conn, respuesta1, strlen(respuesta1));
			
			mysql_close (conn);
		}
		else if(codigo == 2)
		{
			sprintf (respuesta,"%d",DamePartidasGanadas(NOMBRE));
			write (sock_conn, respuesta, strlen(respuesta));
		}
		else if(codigo == 3)
		{
			sprintf (respuesta,"%d",FechaPartidas(NOMBRE));
			write (sock_conn, respuesta, strlen(respuesta));
		}
		else if(codigo == 4)
		{
			sprintf (respuesta,"%d",DuracionPartidas(NOMBRE));
			write (sock_conn, respuesta, strlen(respuesta));
			printf ("Respuesta: %s\n", respuesta);
			//lo enviamos
			
			if ((codigo == 2)||(codigo == 3)||(codigo == 4))
			{
				pthread_mutex_lock( &mutex ); //no interrumpas
				contador =  contador+1;
				pthread_mutex_unlock( &mutex ); //ya puedes interrumpir
			}
		}
		close(sock_conn);
	}
		int main(int argc, char* argv[])
		{
			int sock_conn, sock_listen;
			struct sockaddr_in serv_adr;
			
			//abrimos socket
			if ((sock_listen = socket(AF_INET, SOCK_STREAM, 0)) < 0)
				printf("Error creant socket");
			
			//Fem el bind al port
			//Inicalitza a zero serv_adr
			memset(&serv_adr, 0, sizeof(serv_adr));
			serv_adr.sin_family = AF_INET;
			
			//Asocia el socket a qualsevol IP de la maquina
			//htonl formatea el numero que recibe al formato necesario
			serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
		}
		//escuchamos en el puerto 9050
		serv_adr.sin_port = htons(9050);
		
		if (bind(sock_listen, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) < 0)
			printf("Error en el bind");
		
		//Maximo de peticiones en la cola es de 3
		if (listen(sock_listen, 3) < 0)
			printf("Error en el Listen");
		contador = 0;
		int i;
		int sockets[100];
		
		pthread_t thread;
		i = 0;
		
		for (;;) {
			printf("Escuchando\n");
			
			sock_conn = accept(sock_listen, NULL, NULL);
			
			printf("He recibido conexion\n");
			
			sockets[i] = sock_conn;
			//Crear thread y decirle lo que tiene que hacer
			pthread_create(&thread, NULL, AtenderCliente, &sockets[i]);
			i = i + 1;
				
				//Asocia el socket a qualsevol IP de la maquina
				//htonl formatea el numero que recibe al formato necesario
				serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
			
			//escuchamos en el puerto 9050
			serv_adr.sin_port = htons(9050);
			if (bind(sock_listen, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) < 0)
				printf("Error en el bind");
			
			//Maximo de peticiones en la cola es de 3
			if (listen(sock_listen, 3) < 0)
				printf("Error en el Listen");
			
			contador = 0;
			int i;
			int sockets[100];
			pthread_t thread;
			i = 0;
			
			for (;;) {
				printf("Escuchando\n");
				
				sock_conn = accept(sock_listen, NULL, NULL);
				
				printf("He recibido conexion\n");
				
				sockets[i] = sock_conn;
				//Crear thread y decirle lo que tiene que hacer
				pthread_create(&thread, NULL, AtenderCliente, &sockets[i]);
				i = i + 1;
			}
		}
	}
	int FechaPartidas(char nombre1[60])
	{
		MYSQL *conn;
		int res;
		int err;
		// Estructura especial para almacenar resultados de consultas 
		MYSQL_RES *resultado;
		MYSQL_ROW row;
		char consulta[100];
		conn = mysql_init(NULL);
		if (conn==NULL)
		{
			printf ("Error al crear la conexion: %u %s\n",
					mysql_errno(conn), mysql_error(conn));
			exit (1);
		}
		//inicializar la conexion(Necesario para las consultas)
		conn = mysql_real_connect (conn, "localhost","root", "mysql", "JUEGO",0, NULL, 0);
		if (conn==NULL) {
			printf ("Error al inicializar la conexion: %u %s\n", 
					mysql_errno(conn), mysql_error(conn));
			exit (1);
		}
		//consulta SQL
		sprintf(consulta, "SELECT Partida.Fecha FROM JUGADOR WHERE Partida.Njugador1 = '%s'",nombre1,"OR Partida.Njugador2 = '%d'",nombre1);
		//Comprobamos consulta
		if (err!=0)
		{
			printf ("Error al consultar datos de la base %u %s\n", mysql_errno(conn), mysql_error(conn));
			exit (1);
		}
		//recogemos el resultado de la consulta
		resultado = mysql_store_result (conn);
		row = mysql_fetch_row (resultado);
		//Recogemos el resultado
		if (row == NULL)
			return -1;
		else
			res = strtok(row[0]," ");
		return res;
		
		mysql_close (conn);
		exit(0);
	}
	int DamePartidasGanadas(char nombre[20])
	{
		MYSQL *conn;
		int err;
		// Estructura especial para almacenar resultados de consultas 
		MYSQL_RES *resultado;
		MYSQL_ROW row;
		char consulta [100];
		//Creamos una conexion al servidor MYSQL 
		conn = mysql_init(NULL);
		if (conn==NULL) {
			printf ("Error al crear la conexion: %u %s\n", 
					mysql_errno(conn), mysql_error(conn));
			exit (1);
		}
		//inicializar la conexin
		conn = mysql_real_connect (conn, "localhost","root", "mysql", "JUEGO",0, NULL, 0);
		if (conn==NULL) {
			printf ("Error al inicializar la conexion: %u %s\n", 
					mysql_errno(conn), mysql_error(conn));
			exit (1);
		}
		//consulta SQL
		strcpy (consulta, "SELECT COUNT(*) FROM JUEGO WHERE Resultado.Ganador = '");
		strcat (consulta, nombre);
		strcat (consulta, "'");
		
		//Para comprobar errores en la consulta
		err=mysql_query (conn, consulta);
		if (err!=0) {
			printf ("Error al consultar datos de la base %u %s\n",
					mysql_errno(conn), mysql_error(conn));
			exit (1);
		}
		
		//recogemos el resultado de la consulta
		resultado = mysql_store_result (conn);
		row = mysql_fetch_row (resultado);
		
		//Recogemos el resultado
		
		if (row == NULL || atoi(row[0]) == 0)
			return -1;
		else
			return atoi(row[0]);
		
		mysql_close (conn);
		exit(0);
		
	}
	int DuracionPartidas(char nombre1[60])
	{
		MYSQL *conn;
		int err;
		int res;
		// Estructura especial para almacenar resultados de consultas 
		MYSQL_RES *resultado;
		MYSQL_ROW row;
		char consulta[100];
		char nombre2[60];
		conn = mysql_init(NULL);
		if (conn==NULL)
		{
			printf ("Error al crear la conexion: %u %s\n",
					mysql_errno(conn), mysql_error(conn));
			exit (1);
		}
		//inicializar la conexion(Necesario para las consultas)
		conn = mysql_real_connect (conn, "localhost","root", "mysql", "JUEGO",0, NULL, 0);
		if (conn==NULL) {
			printf ("Error al inicializar la conexion: %u %s\n", 
					mysql_errno(conn), mysql_error(conn));
			exit (1);
		}
		strcpy(nombre1, nombre2);
		//consulta SQL
		sprintf(consulta, "SELECT Partida.Duracion FROM JUGADOR WHERE Partida.Njugador1 = '%s'",nombre1,"OR Partida.Njugador2 = '%d'",nombre1);	
		//Comprobamos consulta
		if (err!=0)
		{
			printf ("Error al consultar datos de la base %u %s\n", mysql_errno(conn), mysql_error(conn));
			exit (1);
		}
		//recogemos el resultado de la consulta
		resultado = mysql_store_result (conn);
		row = mysql_fetch_row (resultado);
		//Recogemos el resultado
		if (row == NULL)
			return -1;
		else
			res = strtok(row[0]," ");
		return res;
		mysql_close (conn);
		exit(0);
	}
	
	void Login(char nombre[25], int DNI[25], char respuesta[512])
	{
		MYSQL *conn;
		int err;
		int login;
		char consulta[500];
		conn = mysql_init(NULL);
		if (conn == NULL) {
			printf("Error al crear la conexion1: %u %s\n",
				   mysql_errno(conn), mysql_error(conn));
			exit(1);
		}
		
		conn = mysql_real_connect(conn, "localhost", "root", "mysql", "JUEGO", 0, NULL, 0);
		if (conn == NULL)
		{
			printf("Error al inicializar la conexion2: %u %s\n",
				   mysql_errno(conn), mysql_error(conn));
			exit(1);
		}
		sprintf(consulta, "SELECT Jugador.Nombre, Jugador.DNI FROM Jugadores WHERE (Jugadores.Nombre='%s' AND Jugadores.DNI='%s')", nombre, DNI);
		err = mysql_query(conn, consulta);
		if (err != 0)
		{
			printf("Error al consultar datos de la base %u %s\n",
				   mysql_errno(conn), mysql_error(conn));
			exit(1);
			sprintf(respuesta, "0-Error-Al consultar la base de datos");
		}
		else
		{
			MYSQL_RES* resultado;
			resultado = mysql_store_result(conn);
			int num_filas = mysql_num_rows(resultado);
			mysql_free_result(resultado);
			
			if (num_filas > 0) {
				printf("Ha iniciado sesion el usuario con nombre: %s\n", nombre);
				sprintf(respuesta, "0-SI");
			}
			else {
				printf("No se encontro el usuario con nombre: %s\n", nombre);
				sprintf(respuesta, "0-Error-Usuario o contrasena mal introducida");
			}
		}
		mysql_close(conn);
	}
	void Registrar(char nombre[25], int DNI[25],float edad, char respuesta[512])
	{
		char consulta[500];
		int numJ;
		int err;
		MYSQL *conn;
		conn = mysql_init(NULL);
		if (conn == NULL) {
			printf("Error al crear la conexion1: %u %s\n",
				   mysql_errno(conn), mysql_error(conn));
			exit(1);
		}
		
		conn = mysql_real_connect(conn, "localhost", "root", "mysql", "JUEGO", 0, NULL, 0);
		if (conn == NULL)
		{
			printf("Error al inicializar la conexion2: %u %s\n",
				   mysql_errno(conn), mysql_error(conn));
			exit(1);
		}
		sprintf(consulta, "INSERT INTO JUGADOR VALUES ('%s', '%s', %s)", nombre, DNI, edad);
		err = mysql_query(conn, consulta);
		if (err != 0)
		{
			printf("Error al insertar datos en la base: %u %s\n", mysql_errno(conn), mysql_error(conn));
			exit(1);
			sprintf(respuesta, "1-Error");
		}
		
		else
		{
			sprintf(respuesta, "Se ha podido registrar correctamente");
		}
		mysql_close(conn);
	
	}


			
		
