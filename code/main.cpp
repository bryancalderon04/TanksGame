#include "Shell.cpp"

//declaramos las funciones y la pantalla de nuestro proyecto
Window ventana("Proyecto Shell",420,240);
void borraTanque(int x, int y);
void dibujacaja(COLORREF***cajas,int*x,int*y,int n);
int chocaCaja(int x,int y, int*xCoord,int*yCoord);
int disparaBala(int x,int y, int posicion,COLORREF***imagen,COLORREF***cajas,int*xCoord,int*yCoord,int xEnemigo,int yEnemigo);
int destruyeCaja(COLORREF***cajas,int*xCoord,int*yCoord,int x,int y);
int destruyeTanque(int xr,int yr,int x,int y);
int*coordenadas(int n,int k);
COLORREF*** matrizCajas(int n,int*x,int*y);
COLORREF***balasMatriz();

int main(){
	//plantamos nuestra semilla la cual me servira mas adelante
	srand(time(NULL));
	int i,j,n;	
	//declaramos el color rojo, que es mas naranja y sera el fondo utilizado en el juego
	COLORREF rojo = RGB(255,127,39);
	int nr,nc;
	//la variable imagen sera un triple apuntador en el cual guardaremos cuatro matrices con las imagenes del primer tanque
	COLORREF*** imagen;
	imagen = (COLORREF***)malloc(4*sizeof(COLORREF**));
	DimensionesBmp("tankdown.bmp",&nr,&nc);
	for(i=0;i<4;i++)
		imagen[i] = (COLORREF**)malloc(nr*sizeof(COLORREF*));
	for(i=0;i<4;i++){
		for(j=0;j<nr;j++)
			imagen[i][j] = (COLORREF*)malloc(nc*sizeof(COLORREF));	
	}
	//leemos las 4 imagenes 
	LeeBmpColor(imagen[0],"tankdown.bmp");
	LeeBmpColor(imagen[1],"tankl.bmp");
	LeeBmpColor(imagen[2],"tankup.bmp");
	LeeBmpColor(imagen[3],"tankr.bmp");
	/************************************************************************/
	//hacemos lo mismo pero con el segundo tanque
	COLORREF*** imagen2;
	imagen2 = (COLORREF***)malloc(4*sizeof(COLORREF**));
	DimensionesBmp("tankdown.bmp",&nr,&nc);
	for(i=0;i<4;i++)
		imagen2[i] = (COLORREF**)malloc(nr*sizeof(COLORREF*));
	for(i=0;i<4;i++){
		for(j=0;j<nr;j++)
			imagen2[i][j] = (COLORREF*)malloc(nc*sizeof(COLORREF));	
	}
	LeeBmpColor(imagen2[0],"trojodown.bmp");
	LeeBmpColor(imagen2[1],"trojol.bmp");
	LeeBmpColor(imagen2[2],"trojoup.bmp");
	LeeBmpColor(imagen2[3],"trojor.bmp");
	/**********************************************************************/
	//pintamos la pantalla de color naranja, pintando pixel por pixel
	for(i=0;i<420;i++){
		for(j=0;j<240;j++)
			ventana.PutPixel(i,j,3,rojo);	
	}
	//declaramos las siguientes variables, la variable key indicara el numero de tecla ingresado por el teclado
	unsigned int key;
	//x y y son las coordenadas de el primer tanque, posicion es el sentido en el que esta volteando el tanque
	//si posicion es 1 el tanque esta a la izquierda, 2 hacia abajo, 3 a la derecha y 4 hacia arriba
	int x = 0, y = 0;
	int posicion = 0;
	//imprimimos el primer tanque
	ventana.PlotCBitmap(imagen[1],nr,nc,x,y,1.0);
	//hacemos lo mismo con el segundo tanque, pero le anadimos una r al final a las variables
	int xr = 390, yr = 210;
	int posicionr = 0;
	ventana.PlotCBitmap(imagen2[1],nr,nc,xr,yr,1.0);
	/*********************************************************///dibuja caja
	//xCoord y yCoord seran las coordenadas de las cajas dentro del juego
	int*xCoord = (int*)malloc(30*sizeof(int));
	int*yCoord = (int*)malloc(30*sizeof(int));
	//le damos valor a estos mediante una funcion
	xCoord=coordenadas(30,14);
	yCoord=coordenadas(30,8);
	//creamos las cajas y despues les imprimamos en pantalla, usamos funciones
	COLORREF***cajas =matrizCajas(30,xCoord,yCoord); 
	dibujacaja(cajas,xCoord,yCoord,30);
	/****************************************************************************************/
	//creamos las balas a disparar por los tanques
	COLORREF***balas = balasMatriz();
	//definimos dos variables, gameOver cambiara a 0 cuando una bala impacte con un tanque
	int gameOver = 1;
	int choqueCaja = 1;
	while(gameOver){
		//cada vez que se inicialice el while key sera igual a 0 para que no mueva los tanques, recibe su valor de la tecla tocada
		key = 0;
		int resultado = (int)Kbhit(&key);
		getchar();
		//hacemos un switch para mover a uno de los tanques o disparar una bala segun la tecla presionada
		switch(key){
			//al presionar la tecla A
			case 65: 
				//primero comprobamos si adelante del tanque hay una caja
				choqueCaja = chocaCaja(x-30,y,xCoord,yCoord);
				//si la hay la funcion regresa un 1
				if(choqueCaja){
					//borramos el tanque en la posicion actual
					borraTanque(x,y);
					//despues lo movemos 15 dos veces en la coordenada x o y segun la tecla presionada
					x -= 15;
					ventana.PlotCBitmap(imagen[1],nr,nc,x,y,1.0); 
					//ponemos Sleep para que el movimiento no sea tan rapido
					Sleep(50);
					borraTanque(x,y);
					x -= 15;	
				}
				//indicamos una posicion segun a donde el tanque este apuntando
				posicion = 1;
				//si adelante del tanque habia una caja entonces el tanque solo apuntara a la direccion de la caja pero no se movera
				ventana.PlotCBitmap(imagen[1],nr,nc,x,y,1.0); 
				Sleep(50);
				//con las siguientes teclas se repiten los mismos pasos pero avanzando a diferentes direcciones
				break;
			//Al presionar la tecla S, el tanque apuntaria hacia abajo, se repiten los pasos	
			case 83: 
				choqueCaja = chocaCaja(x,y+30,xCoord,yCoord);
				if(choqueCaja){
					borraTanque(x,y);
					y += 15;
					ventana.PlotCBitmap(imagen[0],nr,nc,x,y,1.0); 
					Sleep(50);
					borraTanque(x,y);
					y += 15;
				}
				posicion = 2;
				ventana.PlotCBitmap(imagen[0],nr,nc,x,y,1.0); 
				Sleep(50);
				break;
			//Al presionar la tecla D se mueve hacia la derecha	
			case 68: 
				choqueCaja = chocaCaja(x+30,y,xCoord,yCoord);
				if(choqueCaja){
					borraTanque(x,y);
					x += 15;
					ventana.PlotCBitmap(imagen[3],nr,nc,x,y,1.0); 
					Sleep(50);
					borraTanque(x,y);
					x += 15;	
				}
				posicion = 3;
				ventana.PlotCBitmap(imagen[3],nr,nc,x,y,1.0); 
				Sleep(50);
				break;
			//Al presionar la tecla W se mueve hacia arriba	
			case 87:
				choqueCaja = chocaCaja(x,y-30,xCoord,yCoord);
				if(choqueCaja){
					borraTanque(x,y); 
					y -= 15;
					ventana.PlotCBitmap(imagen[2],nr,nc,x,y,1.0); 
					Sleep(50);
					borraTanque(x,y);
					y -= 15;
				}
				posicion = 4;				
				ventana.PlotCBitmap(imagen[2],nr,nc,x,y,1.0); 
				Sleep(50);
				break;
			//Al presionar la J es donde usaremos la variable posicion por que dira a donde ira la bala
			//primero comprobamos si hay una caja delante o el otro tanque, si la hay la destruye y regresa un uno de la funcion 	
			case 74:
				//si destruye un tanque entonces la funcion regresa un 0 y nuestro programa termina
				switch(posicion){
					case 1: n=destruyeCaja(cajas,xCoord,yCoord,x-30,y);
						gameOver = destruyeTanque(x-30,y,xr,yr);
						break;
					case 2: n=destruyeCaja(cajas,xCoord,yCoord,x,y+30);
						gameOver = destruyeTanque(x,y+30,xr,yr);
						break;
					case 3: n=destruyeCaja(cajas,xCoord,yCoord,x+30,y);
						gameOver = destruyeTanque(x+30,y,xr,yr);
						break;		
					case 4: n=destruyeCaja(cajas,xCoord,yCoord,x,y-30);
						gameOver = destruyeTanque(x,y-30,xr,yr);
						break;	
				}
				//si no habia tanque ni caja enfrente entonces la bala seguira avanzando, si la bala le da otro tanque gameOver toma valor 0
				if(n!=1 && gameOver==1)
					gameOver = disparaBala(x,y,posicion,balas,cajas,xCoord,yCoord,xr,yr);						
				break;
			//Se va a repetir lo mismo al presionar las teclas de las flechitas para mover al segundo tanque y enter para disparar la bala	
			case 37: 
				choqueCaja = chocaCaja(xr-30,yr,xCoord,yCoord);
				if(choqueCaja){
					borraTanque(xr,yr);
					xr -= 15;
					ventana.PlotCBitmap(imagen2[1],nr,nc,xr,yr,1.0); 
					Sleep(50);
					borraTanque(xr,yr);
					xr -= 15;
				}
				posicionr = 1;
				ventana.PlotCBitmap(imagen2[1],nr,nc,xr,yr,1.0); 
				Sleep(50);
				break;
			case 40: 
				choqueCaja = chocaCaja(xr,yr+30,xCoord,yCoord);
				if(choqueCaja){
					borraTanque(xr,yr);
					yr += 15;
					ventana.PlotCBitmap(imagen2[0],nr,nc,xr,yr,1.0); 
					Sleep(50);
					borraTanque(xr,yr);
					yr += 15;
				}
				posicionr = 2;
				ventana.PlotCBitmap(imagen2[0],nr,nc,xr,yr,1.0); 
				Sleep(50);
				break;
			case 39: 
				choqueCaja = chocaCaja(xr+30,yr,xCoord,yCoord);
				if(choqueCaja){
					borraTanque(xr,yr);
					xr += 15;
					ventana.PlotCBitmap(imagen2[3],nr,nc,xr,yr,1.0); 
					Sleep(50);
					borraTanque(xr,yr);
					xr += 15;
				}
				posicionr = 3;
				ventana.PlotCBitmap(imagen2[3],nr,nc,xr,yr,1.0); 
				Sleep(50);
				break;
			case 38:
				choqueCaja = chocaCaja(xr,yr-30,xCoord,yCoord);
				if(choqueCaja){
					borraTanque(xr,yr); 
					yr -= 15;
					ventana.PlotCBitmap(imagen2[2],nr,nc,xr,yr,1.0); 
					Sleep(50);
					borraTanque(xr,yr);
					yr -= 15;
				}
				posicionr = 4;
				ventana.PlotCBitmap(imagen2[2],nr,nc,xr,yr,1.0); 
				Sleep(50);
				break;
			case 13:
				switch(posicionr){
					case 1: n=destruyeCaja(cajas,xCoord,yCoord,xr-30,yr);
						gameOver = destruyeTanque(xr-30,yr,x,y);
						break;
					case 2: n=destruyeCaja(cajas,xCoord,yCoord,xr,yr+30);
						gameOver = destruyeTanque(xr,yr+30,x,y);
						break;
					case 3: n=destruyeCaja(cajas,xCoord,yCoord,xr+30,yr);
						gameOver = destruyeTanque(xr+30,yr,x,y);
						break;		
					case 4: n=destruyeCaja(cajas,xCoord,yCoord,xr,yr-30);
						gameOver = destruyeTanque(xr,yr-30,x,y);
						break;	
				}
				if(n!=1 && gameOver==1)
					gameOver = disparaBala(xr,yr,posicionr,balas,cajas,xCoord,yCoord,x,y);
				break;	
			default:
				break;		
		}
	}
	Sleep(500);
	//al momento de darle un tanque al otro, se acabara el while y se imprimira una imagen de gameOver en la pantalla
	DimensionesBmp("gameover.bmp",&nr,&nc);
	COLORREF** imagenGameover;
	imagenGameover = (COLORREF**)malloc(nr*sizeof(COLORREF*));
	for(i=0;i<nr;i++)
		imagenGameover[i] = (COLORREF*)malloc(nc*sizeof(COLORREF));
	LeeBmpColor(imagenGameover,"gameover.bmp");				
	ventana.PlotCBitmap(imagenGameover,nr,nc,0,0,1.0);	
	//finalmente liberamos toda la memoria que habiamos reservado
	for(i=0;i<nr;i++)
		free(imagenGameover[i]);
	free(imagenGameover);	
	for(i=0;i<4;i++){
		for(j=0;j<nr;j++)
			free(imagen[i][j]);
	}
	for(i=0;i<4;i++)
		free(imagen[i]);			
	free(imagen);
	free(xCoord);
	free(yCoord);
	return MainLoop();
}
//variable para borrar un tanque, pinta un cuadro de 30x30 pixeles en la coordenada que se indique
void borraTanque(int x, int y){
	int nr = 30, nc = 30,i;
	COLORREF**imagen;
	imagen = (COLORREF**)malloc(nr*sizeof(COLORREF*));
	for(i=0;i<nr;i++)
		imagen[i] = (COLORREF*)malloc(nc*sizeof(COLORREF));
	LeeBmpColor(imagen,"tborrador.bmp");
	ventana.PlotCBitmap(imagen,nr,nc,x,y,1.0);	
}
//variable para dibujar las cajas con las coordenadas dadas
void dibujacaja(COLORREF***cajas,int*x,int*y,int n){
	int i;
	for(i=0;i<n;i++)
		ventana.PlotCBitmap(cajas[i],30,30,x[i],y[i],1);
}
//funcion para dar valores aleatorios a las coordenadas de las cajas
int*coordenadas(int n,int k){
	int i;
	int*z = (int*)malloc(n*sizeof(int));
	for(i=0;i<n;i++)
		z[i] = (rand()%k)*30;
	return z;
}
//funcion para dar memoria a la matriz de las cajas, crearemos 30 matrices, cada matriz con una caja
COLORREF*** matrizCajas(int n,int*x,int*y){
	int i,j;
	COLORREF ***imagen;
	imagen = (COLORREF***)malloc(n*sizeof(COLORREF**));
	for(i=0;i<n;i++)
		imagen[i] = (COLORREF**)malloc(30*sizeof(COLORREF*));
	for(i=0;i<n;i++){
		for(j=0;j<30;j++)
			imagen[i][j] = (COLORREF*)malloc(30*sizeof(COLORREF));
	}
	for(i=0;i<n;i++)
		LeeBmpColor(imagen[i],"tcaja.bmp");
	return imagen;
}
//esta funcion es para ver si un tanque choca con una caja o puede avanzar, para eso compara las coordenadas a donde
//el tanque avanzara con las coordenadas de las 30 cajas, si choca con una regresa un 0
int chocaCaja(int x,int y, int*xCoord,int*yCoord){
	int i;
	for(i=0;i<30;i++){
		if(x==xCoord[i] && y==yCoord[i]){
			return 0;
		}
	}
	return 1;
}
//funcion para crear las 4 matrices de las cajas
COLORREF***balasMatriz(){
	int i,j;
	COLORREF*** imagen;
	imagen = (COLORREF***)malloc(4*sizeof(COLORREF**));
	for(i=0;i<4;i++)
		imagen[i] = (COLORREF**)malloc(30*sizeof(COLORREF*));
	for(i=0;i<4;i++){
		for(j=0;j<30;j++)
			imagen[i][j] = (COLORREF*)malloc(30*sizeof(COLORREF));	
	}
	LeeBmpColor(imagen[0],"tbalal.bmp");
	LeeBmpColor(imagen[1],"tbaladown.bmp");
	LeeBmpColor(imagen[2],"tbalar.bmp");
	LeeBmpColor(imagen[3],"tbalaup.bmp");
	return imagen;
}
//funcion dispara bala, que nos hace ver si la bala impacta con una caja o con el otro tanque
int disparaBala(int x,int y, int posicion,COLORREF***imagen,COLORREF***cajas,int*xCoord,int*yCoord,int xEnemigo,int yEnemigo){
	int i,n=0,d=0;
	//la bala avanza 4 veces 15 coordenadas o hasta impactar con algo
	for(i=2;i<6;i++){
		Sleep(50);
		//la posicion nos indica hacia donde saldra la bala
		switch(posicion){
			case 1:	
				//borramos la bala donde se ubicaba
				if(i>2)
					borraTanque(x-15*(i-1),y);
				//imprimimos la bala y comprobamos si destruye un tanque o una caja	
				ventana.PlotCBitmap(imagen[0],30,30,x-15*i,y,1.0);
				n=destruyeCaja(cajas,xCoord,yCoord,x-15*(i+1),y);
				d = destruyeTanque(x-15*(i+1),y,xEnemigo,yEnemigo); 	
				//si la bala destruye una caja o ya llega a su valor final la borramos y se acaba el for
				if(i==5 || n==1){
					borraTanque(x-15*i,y);
					i=6;
				}
				break;
			//se repite los pasos con las otras posiciones, simplemente cambiando hacia donde se mueve la bala	
			case 2: 
				if(i>2)
					borraTanque(x,y+15*(i-1)); 
				ventana.PlotCBitmap(imagen[1],30,30,x,y+15*i,1.0);
				n=destruyeCaja(cajas,xCoord,yCoord,x,y+15*(i+1));
				d = destruyeTanque(x,y+15*(i+1),xEnemigo,yEnemigo);	
				if(i==5 || n==1){
					borraTanque(x,y+15*i);
					i=6;
				}
				break;
			case 3: 
				if(i>2)
					borraTanque(x+15*(i-1),y); 
				ventana.PlotCBitmap(imagen[2],30,30,x+15*i,y,1.0); 	
				n=destruyeCaja(cajas,xCoord,yCoord,x+15*(i+1),y);
				d = destruyeTanque(x+15*(i+1),y,xEnemigo,yEnemigo);
				if(i==5 || n==1){
					borraTanque(x+15*i,y);
					i=6;
				}
				break;
			case 4: 
				if(i>2)
					borraTanque(x,y-15*(i-1));
				ventana.PlotCBitmap(imagen[3],30,30,x,y-15*i,1.0); 	
				n=destruyeCaja(cajas,xCoord,yCoord,x,y-15*(i+1));
				d = destruyeTanque(x,y-15*(i+1),xEnemigo,yEnemigo);
				if(i==5 || n==1){
					borraTanque(x,y-15*i);
					i=6;
				}
				break;			
		}
		//d es el valor devuelto por la funcion destruye tanque, si impacta a un tanque la bala regresa un 0 y entonces esta funcion regresara
		//un 0, hacia gameOver=0 y dejaria de correr el while del main
		if(!d)
			return 0;			
	}
	//de lo contrario regresa un 1 indicando que aun corre el while
	return 1;
}
int destruyeCaja(COLORREF***cajas,int*xCoord,int*yCoord,int x,int y){
	//hacemos una matriz para guardar la imagen de una explosion que aparecera cuando la bala le de a una caja
	int i;
	COLORREF**imagenExplosion = (COLORREF**)malloc(30*sizeof(COLORREF*));
	for(i=0;i<30;i++)
		imagenExplosion[i] = (COLORREF*)malloc(30*sizeof(COLORREF));
	LeeBmpColor(imagenExplosion,"texplosion.bmp");
	//vemos las 30 coordenadas de las cajas, si la bala impacta a una caja entonces las coordenadas seran iguales y se destruira la caja
	for(i=0;i<30;i++){
		//las coordenadas de la caja impactada se cambiaran para que ya no genere problemas despues de destruirla
		if(x == xCoord[i] && y == yCoord[i]){
			ventana.PlotCBitmap(imagenExplosion,30,30,xCoord[i],yCoord[i],1.0);
			Sleep(100);
			borraTanque(xCoord[i],yCoord[i]); 	
			xCoord[i] = 1;
			yCoord[i] = 1;
			//regresara un 1 si si impacta y un 0 si no
			return 1;
		}
	}
	for(i=0;i<30;i++)
		free(imagenExplosion[i]);
	free(imagenExplosion);	
	return 0;
}
int destruyeTanque(int xr,int yr,int x,int y){
	int i;
	//igual hacemos la matriz para la imagen de explosion
	COLORREF**imagenExplosion = (COLORREF**)malloc(30*sizeof(COLORREF*));
	for(i=0;i<30;i++)
		imagenExplosion[i] = (COLORREF*)malloc(30*sizeof(COLORREF));
	LeeBmpColor(imagenExplosion,"texplosion.bmp");
	//comparamos las coordenadas de la bala y el tanque enemigo, si son iguales regresa un 0 y pone la explosion
	if(x==xr && y==yr){
			ventana.PlotCBitmap(imagenExplosion,30,30,x,y,1.0);
			Sleep(200);
			return 0;
	}
	//de lo contrario regresa un 1
	for(i=0;i<30;i++)
		free(imagenExplosion[i]);
	free(imagenExplosion);	
	return 1;	
}
