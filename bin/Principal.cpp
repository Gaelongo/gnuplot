#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <conio.h>

int prov[50] = {65, 36, 49, 84, 79, 56, 28, 43, 67, 36,
43, 78, 37, 40, 68, 72, 55, 62, 22, 82,
88, 50, 60, 56, 57, 46, 39, 57, 73, 65}, cont2, modas[2][5];
float prom;
char opc;
int cont = 30;
//promex es para determinar si ya se calculó la media o no
bool promex;

int intDatos(void);
int mostrarMenu(void);
void tallosHojas(void);
void diaPuntos(void);
void histograma(void);
void media(void);
void mm(void);
void vardesv(void);
void regresion(void);

int main(){
	int opc, cent;

	printf("Bienvenido al software de Estadistica Avanzada \n");
	do{
		printf("--- MENU GENERAL: --- \n");
		printf("[1]	Estadisticos de una muestra	[2]	Regresion lineal	[3]	Salir\n");
		printf("Ingrese una opcion: \n");
		scanf("%d",&opc);
		if (opc == 1){
			printf("Para comenzar, ingrese los datos de la muestra: \n");
			/*cont = */intDatos();
			do{	
				cent = mostrarMenu();
			}
			while(cent != 1);
		}
		if(opc == 2){
			regresion();
		}
	}
	while(opc != 3);
	
	printf("Terminando programa...");
	
	return 0;
}

int intDatos(){
	
	int /*cont = 0,*/ i, l, n, ing, temp;
	FILE *arc;
	arc = fopen("datoscalculados.txt", "a");
	/*printf("Ingrese los datos, un valor mayor a 100 detendra el proceso. \n\n");
	do{
		printf("Dato numero %d: ", cont+1);
		scanf("%f", &ing);
		if(ing < 100){
			prov[cont] = ing;
			//cont va contando la cantidad de datos ingresada
			cont++;
		}
	}

	while(ing < 100);*/
	printf("\nIngreso terminado con %d datos. \n", cont);

	//Organizando el vector de menor a mayor
	do{
		n = 0;
		for(i = 1; i < cont; i++){
			if(prov[i-1] > prov[i]){
				temp = prov[i-1];
				prov[i-1] = prov[i];
				prov[i] = temp;
				n = i;
			}
		}
		l = n;
	} while(n != 0);
	
	//Mostrando el vector ordenado
	fputs("Estadisticos calculados de la siguiente muestra: \n\n", arc);
	printf("El vector ordenado de mayor a menor es: \n\n");
	for(i = 0; i < cont; i++){
		fprintf(arc, "%d, ", prov[i]);
		printf("%d, ", prov[i]);
		if((i+1)%5 == 0){
			fprintf(arc, "\n\n");
			printf("\n");
		}
	}
	
	promex = false;
	cont2 = cont;
	fclose(arc);
	return cont;
	
}

int mostrarMenu(){
	
	printf("\n\nSeleccione que quiere obtener: \n");
	printf("a) Diagrama de tallos y hojas.\n");
	printf("b) Diagrama de puntos. \n");
	printf("c) Histograma.\n");
	printf("d) Media, moda y mediana. \n");
	printf("e) Varianza y desviacion estandar. \n");
	printf("f) Volver a ingresar datos. \n");
	printf("s) Atras. \n");
	printf("Ingrese una opcion: ");
	scanf(" %c", &opc);

	switch(opc){
		case 'a':
			
			printf("\nOpcion a seleccionada\n");
			tallosHojas();
			break;
			
		case 'b':
			
			printf("\nOpcion b seleccionada\n");
			diaPuntos();
			break;
			
		case 'c':
			
			printf("\nOpcion c seleccionada\n");
			histograma();
			break;
			
		case 'd':
			
			printf("\nOpcion d seleccionada\n");
			media();
			printf("\nLa media de los datos ingresados es: %.3f\n", prom);
			mm();
			break;
			
		case 'e':
			
			printf("\nOpcion e seleccionada\n");
			vardesv();
			break;
			
		case 'f':
			
			printf("\nIngreso de datos. \n");
			cont = intDatos();
			break;
			
		case 's':
			return 1;
	}
	return 0;
}

void tallosHojas(){
	
	printf("\nGenerando diagama de tallos y hojas... \n\n");
	int tallos, j, i;
	FILE *arc;
	arc = fopen("datoscalculados.txt", "a");
	tallos = prov[cont-1]/10;
	for(j = 0; j < tallos+2; j++){
		fprintf(arc, "%4d, I", j);
		printf("%4d I ", j);
		for(i = 0; i < cont; i++){
			if((prov[i] >= j*10) && (prov[i] < (j+1)*10)){
				fprintf(arc, "%d ", prov[i]-(j*10));
				printf("%d ", prov[i]-(j*10));
				}
		}
		printf("\n");
		fprintf(arc, "\n\n");
	}
	fclose(arc);
}

void diaPuntos(void){
	
	printf("\nGenerando diagrama de puntos... \n\n");
	FILE *fp = NULL;
	FILE *gnuplot = popen("gnuplot", "w");
	fp = fopen("coordPuntos.txt", "w");
	int var = prov[0], contador = 0, i, prim, ult;
	prim = prov[0];
	ult = prov[cont-1];
	//Compara el valor anterior con el valor actual, si son iguales, aumenta el contador, si no, reinicia la variable y el contador a 1.
	for(i = 0; i < cont; i++){
		if(var == prov[i]){
			contador++;
			fprintf(fp, "%d\t%d\n", var, contador);
		}
		else{
			var = prov[i];
			contador = 1;
			fprintf(fp, "%d\t%d\n", var, contador);
		}
	}
	fprintf(fp, "%d\t%d\n", var, contador);
	fclose(fp);
	fprintf(gnuplot, "set yrange [0:20]\n");
	fprintf(gnuplot, "set xrange [%d:%d]\n", prim-1, ult+1);
	fprintf(gnuplot, "set ytics(1,2,3,4,5)\n");
	fprintf(gnuplot, "plot 'coordPuntos.txt' w points linetype 7 lw 6\n");
	fflush(gnuplot);
	system("pause");
	pclose(gnuplot);	
}

void media(void){
	
	int i, suma = 0;
	FILE *arc;
	arc = fopen("datoscalculados.txt", "a");
	
	for(i = 0; i < cont; i++){
		suma = suma+prov[i];
	}
	prom = suma/cont;
	fprintf(arc, "La media de los datos ingresados es: %.3f\n", prom);
	promex = true;
	fclose(arc);
	
}
void mm(void){
	
	float mediana;
	int indice, contador, slot, i, j, k, var;
	FILE *arc;
	arc = fopen("datoscalculados.txt", "a");
	
	//Mediana
	if((cont % 2) == 0){
		indice = cont/2;
		mediana = (prov[indice]+prov[indice+1])/2;
		fprintf(arc, "La mediana de los datos ingresados es: %.3f\n", mediana);
		printf("La mediana de los datos ingresados es: %.3f\n", mediana);
	}
	else{
		indice = cont+1/2;
		fprintf(arc, "La mediana de los datos ingresados es: %d\n", prov[indice]);
		printf("La mediana de los datos ingresados es: %d\n", prov[indice]);
	}
	
	//Moda
	var = prov[0];
	contador = 0;
	slot = 0;
	for(i = 0; i < cont; i++){
		if (var == prov[i]){
			contador++;
		}
		else{
			if (contador >= 2){
				if (contador > modas[0][1]){
					for(j = 0; j < 2; j++){
						for(k = 0; k < 5; k++){
							modas[j][k] = 0;
						}
					}
					modas[0][0] = var;
					modas[0][1] = contador;
					slot = 0;
				}
				else{
					if(contador == modas[0][1]){
						slot++;
						modas[slot][0] = var;
					}
				}
			}
			var = prov[i];
			contador = 1;
		}
	}
	
	if(slot == 0){
		fprintf(arc, "La moda es: %d\n", modas[0][0]);
		printf("La moda es: %d\n", modas[0][0]);
	}
	else{
		fprintf(arc, "Las modas son: ");
		printf("Las modas son: ");
		for(i = 0; i <= slot; i++){
			fprintf(arc, "%d", modas[i][0]);
			printf("%d", modas[i][0]);
			if(i != slot){
				fprintf(arc, ", ");
				printf(", ");
			}
		}
		fprintf(arc, "\n\n");
		printf("\n");
	}
	fclose(arc);
}
void vardesv(void){
	
	int selec, i, operaciones;
	float suma = 0, resta = 0, var, desv;
	FILE *arc;
	arc = fopen("datoscalculados.txt", "a");
	printf("\nSeleccione el metodo para calcular la varianza: \n\n");
	printf("1) Usando la media\n");
	printf("2) Sin usar la media\n");
	printf("Seleccione una opcion: \n");
	scanf("%d",&selec);
	if (selec == 1){
		//Evaluo si ya fue calculado el promedio, de no ser asi, llamo a calcularlo
		if(promex == false){
			media();
		}
		//Una vez calculado el promedio se calcula la varianza y con ello la desviacion estandar
		for(i = 0; i < cont; i++){
				resta = (prov[i]-prom);
				suma = suma + (resta*resta);
		}
		var = suma/(cont2-1);
		desv = sqrt(var);
		operaciones = (3*cont)+1;
		fprintf(arc, "Usando la media, la varianza es: %.3f, calculada con %d llamadas al procesador.\n", var, operaciones);
		printf("\nLa varianza es: %.3f, calculada en %d llamadas al procesador.\n", var, operaciones);
		fprintf(arc, "La desviacion estandar es: %.3f, calculada en %d llamadas al procesador.\n\n", desv, operaciones+1);
		printf("La desviacion estandar es %.3f, calculada en %d llamadas al procesador.\n", desv, operaciones+1);
	}
	else{
		float suma2;
		for(i = 0; i < cont; i++){
			suma = suma+(prov[i]*prov[i]);
		}
		for(i = 0; i < cont; i++){
			suma2 = suma2+prov[i];
		}
		suma2 = (suma2*suma2)/cont2;
		resta = suma-suma2;
		var = resta/(cont2-1.0);
		desv = sqrt(var);
		operaciones = (cont*3)+3;
		fprintf(arc, "Sin usar la media, la varianza es: %.3f, calculada con %d llamadas al procesador.\n", var, operaciones);
		printf("\nLa varianza es %.3f, calculada en %d llamadas al procesador.\n",var, operaciones);
		fprintf(arc, "La desviacion estandar es: %.3f, calculada en %d llamadas al procesador.\n\n", desv, operaciones+1);
		printf("La desviacion estandar es %.3f, calculada en %d llamadas al procesador. \n", desv, operaciones+1);
	}
	fclose(arc);
}

void histograma(void){
	
	int clases = (1 + (log10(cont)/log10(2))) + 1, i, j;
	float liminf, limsup, suma, frecrel, frecmayor = 0;
	float rango = prov[cont-1]-prov[0];
	float intervalo = (rango/clases);
	FILE *pdata = NULL;
	pdata = fopen("histograma.txt", "w");
	FILE *gnuplot = popen("gnuplot", "w");
	for(i = 0; i < clases; i++){
		liminf = prov[0]+(intervalo*i);
		limsup = prov[0]+(intervalo*(i+1));
		fprintf(pdata, "%.3f--%.3f\t", liminf, limsup);
		suma = 0;
		
		for(j = 0; j < cont; j++){
			if(prov[j] >= liminf){
				if(i == (clases-1)){
					if(prov[j] < (limsup+0.00001)){
						suma = suma+1;
					}
					else{
						j = cont;
					}
				}
				else{
					if(prov[j] < limsup){
						suma = suma+1;
					}
					else{
						j = cont;
					}
				}			
			}
		}
		frecrel = (suma/cont2)*100;
		if (frecrel > frecmayor){
			frecmayor = frecrel;
		}
		fprintf(pdata, "%.3f\n", frecrel);
	}
	fclose(pdata);
	printf("Generando histograma...\n\n");
	fprintf(gnuplot, "set boxwidth 0.5\n");
	fprintf(gnuplot, "set style fill solid\n");
	fprintf(gnuplot, "set yrange[0:%.3f]\n",frecmayor+10);
	fprintf(gnuplot, "set xrange[-1:%d]\n", clases);
	fprintf(gnuplot, "plot 'histograma.txt' using 2:xtic(1) with boxes\n");
	fflush(gnuplot);
	system("pause");
	pclose(gnuplot);
	
}
void regresion(void){
	int muestra[50][2], difer[50], i, j, elem = 0;
	float pendiente, b, div1, div2, sx = 0, sy = 0, sxy = 0, sxc = 0, elemf = 0;
	bool ext = true;
	FILE *arc;
	arc = fopen("datoscalculados.txt", "a");
	
	printf("\n*** Regresion lineal por minimos cuadrados ***\n");
	printf("Ingrese la muestra a graficar: \n");
	do{
		for(j = 0; j < 2; j++){
			printf("Ingrese el dato %d,%d: ", elem+1, j+1);
			scanf("%d", &muestra[elem][j]);
			if((j == 0) && (muestra[elem][j] > 10000)){
				ext = false;
			}
		}
		elem++;
	}
	while(ext == true);
	elem--;
	elemf = elem;
	FILE *puntos = NULL;
	puntos = fopen("points.txt", "w");
	for(i = 0; i < elem; i++){
		fprintf(puntos, "%d\t%d\n",muestra[i][0], muestra[i][1]);
	}
	fclose(puntos);
	
	for(i = 0; i < elem; i++){
		sx = sx+muestra[i][0];
		sy = sy+muestra[i][1];
		sxy = sxy+(muestra[i][0]*muestra[i][1]);
		sxc = sxc+(muestra[i][0]*muestra[i][0]);
	}
	pendiente = (sxy-((sx*sy)/elemf))/(sxc-((sx*sx)/elemf));
	div1 = (sxy-((sx*sy)/elemf));
	div2 = (sxc-((sx*sx)/elemf));
	pendiente = div1/div2;
	b = (sy/elemf)-(pendiente*(sx/elemf));
	fprintf(arc, "La ecuacion de la recta calculada es: %.3fx+%.3f\n\n", pendiente, b);
	fclose(arc);
	printf("LA ECUACION DE LA RECTA ES: %.3fx+%.3f\n\n", pendiente, b);
	printf("Graficando...");
	FILE *gnuplot = popen("gnuplot", "w");
	fprintf(gnuplot, "plot 'points.txt' with points\n");
	fprintf(gnuplot, "plot 'points.txt' title 'puntos' with points, \\\n%f*x+%f title 'linea'\n", pendiente, b);
	fflush(gnuplot);
	system("pause");
	pclose(gnuplot);
}