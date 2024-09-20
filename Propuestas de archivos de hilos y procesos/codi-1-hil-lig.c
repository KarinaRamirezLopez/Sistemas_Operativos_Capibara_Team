int main() {
    int numeros[] = {100, 20, 30, 40, 50,};
    int longitud = sizeof(numeros) / sizeof(numeros[0]);

    pthread_t hiloSuma, hiloPromedio;
    DatosHilo datosSuma = {numeros, longitud, 0};
    DatosHilo datosPromedio = {numeros, longitud, 0};

    // Crear hilos
    pthread_create(&hiloSuma, NULL, calcularSuma, &datosSuma);
    pthread_create(&hiloPromedio, NULL, calcularPromedio, &datosPromedio);

    // Esperar a que los hilos terminen
    pthread_join(hiloSuma, NULL);
    pthread_join(hiloPromedio, NULL);

    // Imprimir resultados
    printf("La suma de los números es: %d\n", datosSuma.resultado);
    printf("El promedio de los números es: %d\n", datosPromedio.resultado);

    printf("Programa principal: Cálculos finalizados\n");
    return 0;
}

