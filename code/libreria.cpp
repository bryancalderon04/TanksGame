#include "libreria.hpp"
//#include "Shell.cpp"


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
