// Juan Sebastian Higinio, Cristian Bernal y David Bejarano
// Algoritmo PuertaRigoberto
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <limits>
#include <cstdlib>   // para system()

using namespace std;

// Prototipos
int menuopciones();
int calcularcortes();
void mostrarPasos(stringstream &instrucciones, size_t &caracteresMostrados);

// ---------------------------------------------------------------
// Menu principal
// ---------------------------------------------------------------
int menuopciones(){
    int opcion;
    cout << "=== Menu de Opciones ===" << endl;
    cout << "1. Ingresar medidas de la puerta" << endl;
    cout << "2. Cambiar el Tamano de la Lamina" << endl;
    cout << "3. Mostrar archivos de Cortes" << endl;
    cout << "4. Salir" << endl;
    cout << "\nSeleccione una opcion: ";
    cin >> opcion;
    return opcion;
}

// ---------------------------------------------------------------
// Imprime solo las instrucciones nuevas acumuladas
// ---------------------------------------------------------------
void mostrarPasos(stringstream &instrucciones, size_t &caracteresMostrados) {
    string todo = instrucciones.str();
    cout << todo.substr(caracteresMostrados);
    caracteresMostrados = todo.size();
}

// ---------------------------------------------------------------
// main
// ---------------------------------------------------------------
int main(){

    int opcion;

    do {
        opcion = menuopciones();
        switch (opcion) {
            case 1:
                calcularcortes();
                break;
            case 2:
                cout << "Opcion 2 seleccionada: Cambiar el Tamano de la Lamina" << endl;
                break;
            case 3:
                cout << "Opcion 3 seleccionada: Mostrar archivos de Cortes" << endl;
                break;
            case 4:
                cout << "Saliendo del programa..." << endl;
                break;
            default:
                cout << "Opcion invalida. Por favor, seleccione una opcion valida." << endl;
        }
    } while (opcion != 4);

    return 0;
}

// ---------------------------------------------------------------
// calcularcortes
// ---------------------------------------------------------------
int calcularcortes() {
    double altopuerta = 0, anchopuerta = 0, altolamina = 0, ancholamina = 0;
    double arealamina = 0, areapuerta = 0, primersobrantealto = 0, primersobranteancho = 0;

    double ensamblepuertaancho = 0, ensamblepuertaalto = 0, anchocortar = 0, altocortar = 0;
    double sobranteultimoretazoancho = 0, sobrantelateralancho = 0, sobranteultimoretazoalto = 0;

    double arearetazos = 0;
    double areaAcumulada = 0;
    int cantidadLaminas = 0;

    stringstream instrucciones;
    size_t caracteresMostrados = 0;

    // ----- Validacion lamina -----
    do{
        cout << "Cual es el alto de la lamina en cm" << endl;
        cin >> altolamina;
        cout << "Cual es el ancho de la lamina en cm" << endl;
        cin >> ancholamina;

        if (altolamina <= 0 || ancholamina <= 0) {
            cout << "?? No puedes hacer una lamina negativa o de cero tamano" << endl;
        }
    } while (altolamina <= 0 || ancholamina <= 0);

    arealamina = altolamina * ancholamina;
    arearetazos = 0;

    // ----- Validacion puerta -----
    do {
        cout << "Cual es el alto de la puerta en cm" << endl;
        cin >> altopuerta;
        cout << "Cual es el ancho de la puerta en cm" << endl;
        cin >> anchopuerta;

        if (altopuerta <= 0 || anchopuerta <= 0) {
            cout << "?? No puedes hacer una puerta negativa o de cero tamano" << endl;
        }
    } while (!(altopuerta > 0 && anchopuerta > 0));

    areapuerta = altopuerta * anchopuerta;

    // =====================================================================
    // CASO 1: area puerta > area lamina
    // =====================================================================
    if (areapuerta > arealamina) {
        instrucciones << "Con una sola lamina no alcanza. Se agregaran laminas/retazos adicionales del mismo tamano." << endl;
        mostrarPasos(instrucciones, caracteresMostrados);

        do {
            cantidadLaminas++;
            areaAcumulada += arealamina;

            instrucciones << "Lamina/retazo #" << cantidadLaminas
                          << " agregado (" << altolamina << "x" << ancholamina << " cm). "
                          << "Area acumulada: " << areaAcumulada << " cm2 / Area puerta: " << areapuerta << " cm2." << endl;

            if (areapuerta > areaAcumulada) {
                instrucciones << "Aun falta area; se necesita otro retazo/lamina." << endl;
            }
            mostrarPasos(instrucciones, caracteresMostrados);
        } while (areapuerta > areaAcumulada);

        arearetazos = areaAcumulada - areapuerta;

        instrucciones << "Total de laminas/retazos necesarios por area: " << cantidadLaminas << endl;
        instrucciones << "Sobrante total estimado por area: " << arearetazos << " cm2" << endl;

        cout << "----------------------------------------------------" << endl;
        mostrarPasos(instrucciones, caracteresMostrados);
        if (areaAcumulada >= areapuerta) {
            cout << "CORRECTO." << endl;
        } else {
            cout << "FALSO." << endl;
        }
        cout << "----------------------------------------------------" << endl;

    } else {
        // =================================================================
        // CASO 2: area puerta <= area lamina
        // =================================================================
        cantidadLaminas = 1;
        areaAcumulada = arealamina;
        primersobrantealto = altolamina - altopuerta;
        primersobranteancho = ancholamina - anchopuerta;

        // ---- CASO 2.A: la puerta cabe completa ----
        if (altopuerta <= altolamina && anchopuerta <= ancholamina) {
            instrucciones << "Con la lamina si puedes armar la puerta" << endl;
            instrucciones << "Debes cortar " << (altolamina - altopuerta) << " cm del alto y "
                          << (ancholamina - anchopuerta) << " cm del ancho de la lamina." << endl;
            instrucciones << "De ese corte sobraron dos pedazos:" << endl;
            instrucciones << "uno de " << altolamina << " cm por " << (ancholamina - anchopuerta)
                          << " cm y otro de " << (altolamina - altopuerta) << " cm por " << anchopuerta << " cm." << endl;
            instrucciones << "no se debe agregar ningun pedazo :)" << endl;
            mostrarPasos(instrucciones, caracteresMostrados);

            arearetazos = (altolamina * (ancholamina - anchopuerta)) + ((altolamina - altopuerta) * anchopuerta);

            cout << "----------------------------------------------------" << endl;
            if (areapuerta == (arealamina - arearetazos)) {
                cout << "CORRECTO." << endl;
            } else {
                cout << "FALSO." << endl;
            }
            cout << "----------------------------------------------------" << endl;

        } else {

            // ---- CASO 2.B: ancho puerta > ancho lamina ----
            if (anchopuerta > ancholamina) {
                instrucciones << "Con la lamina si se puede armar la puerta pero toca pegar varios retazos a la derecha de la puerta." << endl;

                ensamblepuertaancho = ancholamina;
                ensamblepuertaalto = altolamina - altopuerta;

                instrucciones << "1. corta la lamina " << altopuerta << " cm del alto por " << ancholamina
                              << " del ancho de la lamina." << endl;
                instrucciones << "sobra un pedazo de " << ensamblepuertaalto << " cm de alto y " << ancholamina
                              << " cm de ancho." << endl;
                mostrarPasos(instrucciones, caracteresMostrados);

                while (ensamblepuertaancho < anchopuerta && ensamblepuertaalto >= altopuerta) {
                    if ((anchopuerta - ensamblepuertaancho) > ancholamina) {
                        anchocortar = ancholamina;
                    } else {
                        anchocortar = anchopuerta - ensamblepuertaancho;
                        sobranteultimoretazoancho = ancholamina - anchocortar;
                    }

                    instrucciones << "Aun faltan " << (anchopuerta - ensamblepuertaancho) << " cm de ancho." << endl;
                    instrucciones << "Del sobrante de la lamina, corte una tira de " << altopuerta
                                  << " cm de alto por " << anchocortar << " cm de ancho." << endl;

                    ensamblepuertaancho = ensamblepuertaancho + anchocortar;
                    ensamblepuertaalto = ensamblepuertaalto - altopuerta;

                    instrucciones << "Ancho actual de la puerta ensamblada es de " << ensamblepuertaancho << " cm." << endl;
                    instrucciones << "Sobrante restante de altura disponible es de " << ensamblepuertaalto << " cm." << endl;
                    mostrarPasos(instrucciones, caracteresMostrados);
                }

                if (ensamblepuertaancho >= anchopuerta) {
                    instrucciones << "Ya tienes la puerta de " << altopuerta << " cm de alto y "
                                  << ensamblepuertaancho << " cm de ancho." << endl;
                    instrucciones << "De los cortes sobraron dos pedazos:" << endl;

                    if (ensamblepuertaalto > 0) {
                        instrucciones << "1. Un pedazo de lamina de " << ensamblepuertaalto << " cm de alto y "
                                      << ancholamina << " cm de ancho." << endl;
                    } else {
                        instrucciones << "1. No sobro nada de altura al fondo." << endl;
                    }

                    if (sobranteultimoretazoancho > 0) {
                        instrucciones << "2. Un retazo lateral de " << altopuerta << " cm de alto y "
                                      << sobranteultimoretazoancho << " cm de ancho." << endl;
                    }
                    mostrarPasos(instrucciones, caracteresMostrados);

                    arearetazos = (ensamblepuertaalto * ancholamina) + (altopuerta * sobranteultimoretazoancho);

                    cout << "----------------------------------------------------" << endl;
                    if (areapuerta == (arealamina - arearetazos)) {
                        cout << "CORRECTO." << endl;
                    } else {
                        cout << "FALSO." << endl;
                    }
                    cout << "----------------------------------------------------" << endl;

                } else {
                    instrucciones << "Lamentablemente la lamina no tiene suficiente altura para sacar mas tiras y completar el ancho de la puerta." << endl;
                    mostrarPasos(instrucciones, caracteresMostrados);
                }

            } else {

                // ---- CASO 2.C: alto puerta > alto lamina ----
                if (altopuerta > altolamina) {
                    instrucciones << "Con la lamina si puedes armar la puerta pero toca pegar varios retazos arriba de la puerta." << endl;

                    ensamblepuertaalto = altolamina;
                    ensamblepuertaancho = anchopuerta;

                    instrucciones << "1. corta la lamina a todo el alto de " << altolamina << " cm por un ancho de "
                                  << anchopuerta << " cm." << endl;
                    mostrarPasos(instrucciones, caracteresMostrados);

                    while (ensamblepuertaalto < altopuerta && ensamblepuertaancho <= ancholamina) {
                        if ((altopuerta - ensamblepuertaalto) > altolamina) {
                            altocortar = altolamina;
                        } else {
                            altocortar = altopuerta - ensamblepuertaalto;
                            sobranteultimoretazoalto = altolamina - altocortar;
                        }

                        instrucciones << "Aun faltan retazos para terminar la puerta " << (altopuerta - ensamblepuertaalto)
                                      << " cm de alto." << endl;
                        instrucciones << "Del sobrante de la lamina, corte una tira de " << altocortar
                                      << " cm de alto por " << anchopuerta << " cm de ancho." << endl;

                        ensamblepuertaalto = ensamblepuertaalto + altocortar;
                        ensamblepuertaancho = ensamblepuertaancho + anchopuerta;

                        instrucciones << "Altura actual de la puerta ensamblada es de " << ensamblepuertaalto << " cm." << endl;
                        mostrarPasos(instrucciones, caracteresMostrados);
                    }

                    sobrantelateralancho = ancholamina - ensamblepuertaancho;

                    if (ensamblepuertaalto >= altopuerta) {
                        instrucciones << "Ya tienes la puerta de " << altopuerta << " cm de alto y "
                                      << anchopuerta << " cm de ancho." << endl;
                        instrucciones << "De los cortes sobraron dos pedazos:" << endl;

                        if (sobrantelateralancho > 0) {
                            instrucciones << "1. Una tira lateral de " << altolamina << " cm de alto y "
                                          << sobrantelateralancho << " cm de ancho." << endl;
                        } else {
                            instrucciones << "1. No sobro nada de ancho lateral." << endl;
                        }

                        if (sobranteultimoretazoalto > 0) {
                            instrucciones << "2. Un retazo superior de " << sobranteultimoretazoalto << " cm de alto y "
                                          << anchopuerta << " cm de ancho." << endl;
                        } else {
                            instrucciones << "2. No sobro nada arriba en el ultimo corte." << endl;
                        }
                        mostrarPasos(instrucciones, caracteresMostrados);

                        arearetazos = (altolamina * sobrantelateralancho) + (sobranteultimoretazoalto * anchopuerta);

                        cout << "----------------------------------------------------" << endl;
                        if (areapuerta == (arealamina - arearetazos)) {
                            cout << "CORRECTO." << endl;
                        } else {
                            cout << "FALSO." << endl;
                        }
                        cout << "----------------------------------------------------" << endl;

                    } else {
                        instrucciones << "Lamentablemente la lamina no tiene suficiente ancho para sacar mas trozos y pegarlas arriba para completar la altura de la puerta." << endl;
                        mostrarPasos(instrucciones, caracteresMostrados);
                    }

                } else {
                    instrucciones << "Lamentablemente con esas medidas para la puerta no puedes sacarla solo con la lamina." << endl;
                    mostrarPasos(instrucciones, caracteresMostrados);
                }
            }
        }
    }

    // ----- Resumen de variables (debug) -----
    cout << "     -> alto puerta:____" << altopuerta << "____" << endl;
    cout << "     -> ancho puerta:____" << anchopuerta << "____" << endl;
    cout << "     -> alto lamina:____" << altolamina << "____" << endl;
    cout << "     -> ancho lamina:____" << ancholamina << "____" << endl;
    cout << "     -> area lamina:____" << arealamina << "____" << endl;
    cout << "     -> area puerta:____" << areapuerta << "____" << endl;
    cout << "     -> cantidad laminas/retazos:____" << cantidadLaminas << "____" << endl;
    cout << "     -> area acumulada:____" << areaAcumulada << "____" << endl;
    cout << "     -> primer sobrante alto:____" << primersobrantealto << "____" << endl;
    cout << "     -> primer sobrante ancho:____" << primersobranteancho << "____" << endl;
    cout << "     -> ancho a cortar:____" << anchocortar << "____" << endl;
    cout << "     -> alto a cortar:____" << altocortar << "____" << endl;
    cout << "     -> sumatoria de los retazos para el alto de la puerta:____" << ensamblepuertaalto << "____" << endl;
    cout << "     -> sumatoria de los retazos para el ancho de la puerta:____" << ensamblepuertaancho << "____" << endl;
    cout << "     -> sobrante ultimo retazo ancho:____" << sobranteultimoretazoancho << "____" << endl;
    cout << "     -> sobrante lateral ancho:____" << sobrantelateralancho << "____" << endl;
    cout << "     -> sobrante ultimo retazo alto:____" << sobranteultimoretazoalto << "____" << endl;
    cout << "     -> area retazos:____" << arearetazos << "____" << endl;

    // =====================================================================
    // GUARDADO EN ARCHIVO DE TEXTO
    // =====================================================================
    char guardarArchivo;
    cout << "Desea guardar los resultados en un archivo de texto? (s/n): ";
    cin >> guardarArchivo;

    if (guardarArchivo == 's' || guardarArchivo == 'S') {
        string nombreArchivo;
        string nombreCliente;
        string materialLamina;

        cout << "Ingrese el nombre del archivo (sin extension): ";
        cin >> nombreArchivo;

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "Ingrese el nombre del cliente: ";
        getline(cin, nombreCliente);

        cout << "Ingrese el material de la lamina: ";
        getline(cin, materialLamina);

        // Crear carpeta recibos si no existe (GCC 4.9 no tiene <filesystem>)
        system("if not exist recibos mkdir recibos");

        // Ruta completa dentro de recibos/
        string rutaCompleta = "recibos/" + nombreArchivo + ".txt";
        ofstream archivo(rutaCompleta.c_str());

        if (archivo.is_open()) {

            archivo << "=== Instrucciones de Cortes ===" << endl;
            archivo << "Cliente: " << nombreCliente << endl;
            archivo << "Material de la lamina: " << materialLamina << endl;
            archivo << endl;
            archivo << "--- Medidas de la lamina ---" << endl;
            archivo << "Alto lamina: " << altolamina << " cm" << endl;
            archivo << "Ancho lamina: " << ancholamina << " cm" << endl;
            archivo << "Area lamina: " << arealamina << " cm2" << endl;
            archivo << endl;
            archivo << "--- Medidas de la puerta ---" << endl;
            archivo << "Alto puerta: " << altopuerta << " cm" << endl;
            archivo << "Ancho puerta: " << anchopuerta << " cm" << endl;
            archivo << "Area puerta: " << areapuerta << " cm2" << endl;
            archivo << endl;
            archivo << "Cantidad de laminas/retazos: " << cantidadLaminas << endl;
            archivo << "Area de retazos/sobrantes: " << arearetazos << " cm2" << endl;

            archivo << endl;
            archivo << "========================================" << endl;
            archivo << endl;

            archivo << "=== Instrucciones paso a paso ===" << endl;
            archivo << instrucciones.str();

            archivo.close();
            cout << "Archivo guardado como recibos/" << nombreArchivo << ".txt" << endl;
            cout << "\nAbre esta URL en el navegador para imprimir:" << endl;
            cout << "http://localhost:8080/?archivo=" << nombreArchivo << ".txt" << endl;
        }
        else {
            cout << "No se pudo crear el archivo." << endl;
        }
    }

    return 0;
}
