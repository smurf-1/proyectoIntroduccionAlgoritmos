// Juan Sebastian Higinio, Cristian Bernal y David Bejarano
// Algoritmo PuertaRigoberto
#include <iostream>
#include <fstream> // Para crear, escribir y leer archivos .txt
#include <sstream> // Para ir acumulando todas las instrucciones y mensajes
#include <string>
#include <limits>  // Para limpiar entradas invalidas de forma clasica

using namespace std;

// struct DatosRecibo que se llena justo antes de guardar y de aqui se escribe el primer .txt (resumen del trabajo).
struct DatosRecibo {
    string nombreArchivo;   // Nombre que el usuario le da al archivo del recibo (sin extension)
    string cliente;         // Nombre del cliente
    string material;        // Material de la lamina (dato informativo)
    double altoLamina;      // Alto de la lamina en cm
    double anchoLamina;     // Ancho de la lamina en cm
    double areaLamina;      // Area de la lamina en cm2
    double altoPuerta;      // Alto de la puerta en cm
    double anchoPuerta;     // Ancho de la puerta en cm
    double areaPuerta;      // Area de la puerta en cm2
    int cantidadLaminas;    // Cantidad de laminas/retazos necesarios
    double areaRetazos;     // Area de retazos/sobrantes en cm2
};

// Prototipos de las funciones del programa
int menuopciones();
int calcularcortes();
int tamanolamina();
int instruccionescortes(const string& textoInstrucciones, const string& nombreArchivo = "instrucciones_corte.txt", const string& rutaCarpeta = "recibos/");
void mostrarArchivosCortes(const string& nombreArchivo = "instrucciones_corte.txt", const string& rutaCarpeta = "");
double leerNumeroPositivo(const string& mensaje, bool permitirCeroEnAlto = false);

double altolamina = 100, ancholamina = 150; // Tamano por defecto de la lamina (se puede cambiar en el menu)

// Funcion auxiliar sencilla para leer numeros sin romperse si escriben letras
double leerNumeroPositivo(const string& mensaje, bool permitirCeroEnAlto) {
    double valor = 0;
    while (true) {
        cout << mensaje << endl;
        cin >> valor;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Entrada invalida. Por favor ingrese un numero." << endl;
            continue;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (valor < 0) {
            cout << "?? No puedes hacer una medida negativa" << endl;
        } else if (valor == 0 && !permitirCeroEnAlto) {
            cout << "La medida debe ser mayor que cero." << endl;
        } else {
            return valor;
        }
    }
}

// Muestra el menu principal y devuelve la opcion que eligio el usuario
int menuopciones() {
    int opcion = 0;
    while (true) {
        cout << "=== Menu de Opciones ===" << endl;
        cout << "1. Ingresar medidas de la puerta" << endl;
        cout << "2. Cambiar el Tamano de la Lamina" << endl;
        cout << "3. Mostrar archivos de Cortes" << endl;
        cout << "4. Salir" << endl;
        cout << "\nSeleccione una opcion: ";

        cin >> opcion;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Opcion invalida. Ingrese un numero del 1 al 4." << endl << endl;
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return opcion;
        }
    }
}

// Funcion principal: repite el menu hasta que el usuario elija salir (opcion 4)
int main() {
    int opcion;

    do {
        opcion = menuopciones();
        switch (opcion) {
            case 1:
                calcularcortes();
                break;
            case 2:
                tamanolamina();
                break;
            case 3:
                cout << "Opcion 3 seleccionada: Mostrar archivos de Cortes" << endl;
                mostrarArchivosCortes();
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

// Pide las medidas, calcula los cortes, muestra las instrucciones en pantalla
// y, si el usuario quiere, las guarda en archivos de texto
int calcularcortes() {
    // Medidas de la puerta y de la lamina
    double altopuerta = 0, anchopuerta = 0;
    // Areas y primeros sobrantes
    double arealamina = 0, areapuerta = 0, primersobrantealto = 0, primersobranteancho = 0;
    // Variables para ir armando la puerta cuando toca pegar retazos
    double ensamblepuertaancho = 0, ensamblepuertaalto = 0, anchocortar = 0, altocortar = 0;
    // Sobrantes que quedan despues de los cortes
    double sobranteultimoretazoancho = 0, sobrantelateralancho = 0, sobranteultimoretazoalto = 0;
    double arearetazos = 0;     // Area total de lo que sobra
    double areaAcumulada = 0;   // Area acumulada de las laminas usadas
    int cantidadLaminas = 0;    // Cantidad de laminas/retazos necesarios

    // Variables para repetir las instrucciones de corte por cada lamina
    double altoCubierto = 0;
    double anchoCubierto = 0;
    double altoPieza = 0;
    double anchoPieza = 0;
    int filaActual = 1;
    bool puertaCubierta = false;

    stringstream instrucciones;

    arealamina = altolamina * ancholamina;
    arearetazos = 0;

    do {
        altopuerta = leerNumeroPositivo("Cual es el alto de la puerta en cm", true);
        anchopuerta = leerNumeroPositivo("Cual es el ancho de la puerta en cm", true);

        if (altopuerta == 0 || anchopuerta == 0) {
            cout << "Las medidas de la puerta deben ser mayores que cero." << endl;
        }
    } while (!(altopuerta > 0 && anchopuerta > 0));

    areapuerta = altopuerta * anchopuerta;

    // Si el area de la puerta es mayor que una sola lamina, se agregan laminas/retazos en bucle
    if (areapuerta > arealamina) {
        cout << "Con una sola lamina no alcanza. Se agregaran laminas/retazos adicionales del mismo tamano." << endl;
        instrucciones << "Con una sola lamina no alcanza. Se agregaran laminas/retazos adicionales del mismo tamano." << endl;

        do {
            cantidadLaminas++;
            areaAcumulada += arealamina;

            cout << "Lamina/retazo #" << cantidadLaminas
                 << " agregado (" << altolamina << "x" << ancholamina << " cm). "
                 << "Area acumulada: " << areaAcumulada << " cm2 / Area puerta: " << areapuerta << " cm2." << endl;
            instrucciones << "Lamina/retazo #" << cantidadLaminas
                          << " agregado (" << altolamina << "x" << ancholamina << " cm). "
                          << "Area acumulada: " << areaAcumulada << " cm2 / Area puerta: " << areapuerta << " cm2." << endl;

            if (areapuerta > areaAcumulada) {
                cout << "Aun falta area; se necesita otro retazo/lamina." << endl;
                instrucciones << "Aun falta area; se necesita otro retazo/lamina." << endl;
            }
        } while (areapuerta > areaAcumulada);

        arearetazos = areaAcumulada - areapuerta;

        for (int numeroLamina = 1; numeroLamina <= cantidadLaminas; numeroLamina++) {
            cout << "----- Lamina " << numeroLamina << " de " << cantidadLaminas
                 << " (" << altolamina << "x" << ancholamina << " cm) -----" << endl;
            instrucciones << "----- Lamina " << numeroLamina << " de " << cantidadLaminas
                          << " (" << altolamina << "x" << ancholamina << " cm) -----" << endl;

            if (puertaCubierta) {
                cout << "La puerta ya esta cubierta; esta lamina no hace falta cortarla." << endl;
                instrucciones << "La puerta ya esta cubierta; esta lamina no hace falta cortarla." << endl;
            } else {
                anchoPieza = ancholamina;
                if ((anchopuerta - anchoCubierto) < ancholamina) {
                    anchoPieza = anchopuerta - anchoCubierto;
                }

                altoPieza = altolamina;
                if ((altopuerta - altoCubierto) < altolamina) {
                    altoPieza = altopuerta - altoCubierto;
                }

                cout << "1. Corte de la lamina " << numeroLamina << " una pieza de " << altoPieza
                     << " cm de alto por " << anchoPieza << " cm de ancho." << endl;
                instrucciones << "1. Corte de la lamina " << numeroLamina << " una pieza de " << altoPieza
                              << " cm de alto por " << anchoPieza << " cm de ancho." << endl;

                if (anchoCubierto == 0) {
                    cout << "2. Pegue la pieza al inicio de la fila " << filaActual << " de la puerta." << endl;
                    instrucciones << "2. Pegue la pieza al inicio de la fila " << filaActual << " de la puerta." << endl;
                } else {
                    cout << "2. Pegue la pieza en la fila " << filaActual
                         << " de la puerta, a la derecha de lo que ya tiene armado." << endl;
                    instrucciones << "2. Pegue la pieza en la fila " << filaActual
                                  << " de la puerta, a la derecha de lo que ya tiene armado." << endl;
                }

                anchoCubierto = anchoCubierto + anchoPieza;
                cout << "Ancho cubierto de la fila " << filaActual << ": " << anchoCubierto
                     << " cm de " << anchopuerta << " cm." << endl;
                instrucciones << "Ancho cubierto de la fila " << filaActual << ": " << anchoCubierto
                              << " cm de " << anchopuerta << " cm." << endl;

                if (anchoCubierto >= anchopuerta) {
                    altoCubierto = altoCubierto + altoPieza;
                    cout << "Fila " << filaActual << " completa. Alto cubierto de la puerta: " << altoCubierto
                         << " cm de " << altopuerta << " cm." << endl;
                    instrucciones << "Fila " << filaActual << " completa. Alto cubierto de la puerta: " << altoCubierto
                                  << " cm de " << altopuerta << " cm." << endl;
                    anchoCubierto = 0;
                    filaActual++;
                }

                if ((ancholamina - anchoPieza) > 0) {
                    cout << "3. Sobra una tira lateral de " << altolamina << " cm de alto y "
                         << (ancholamina - anchoPieza) << " cm de ancho." << endl;
                    instrucciones << "3. Sobra una tira lateral de " << altolamina << " cm de alto y "
                                  << (ancholamina - anchoPieza) << " cm de ancho." << endl;
                }
                if ((altolamina - altoPieza) > 0) {
                    cout << "3. Sobra un retazo superior de " << (altolamina - altoPieza) << " cm de alto y "
                         << anchoPieza << " cm de ancho." << endl;
                    instrucciones << "3. Sobra un retazo superior de " << (altolamina - altoPieza) << " cm de alto y "
                                  << anchoPieza << " cm de ancho." << endl;
                }
                if ((ancholamina - anchoPieza) <= 0 && (altolamina - altoPieza) <= 0) {
                    cout << "3. De esta lamina no sobro nada." << endl;
                    instrucciones << "3. De esta lamina no sobro nada." << endl;
                }

                if (altoCubierto >= altopuerta) {
                    puertaCubierta = true;
                    cout << "VERIFICACION: ya se cubrieron el alto y el ancho de la puerta." << endl;
                    instrucciones << "VERIFICACION: ya se cubrieron el alto y el ancho de la puerta." << endl;
                } else {
                    cout << "VERIFICACION: aun falta cubrir " << (altopuerta - altoCubierto)
                         << " cm de alto; en la fila actual faltan " << (anchopuerta - anchoCubierto)
                         << " cm de ancho." << endl;
                    instrucciones << "VERIFICACION: aun falta cubrir " << (altopuerta - altoCubierto)
                                  << " cm de alto; en la fila actual faltan " << (anchopuerta - anchoCubierto)
                                  << " cm de ancho." << endl;
                }
            }
        }

        if (!puertaCubierta) {
            cout << "AVISO: con las laminas calculadas por area aun no se cubre toda la puerta por la forma de los cortes." << endl;
            instrucciones << "AVISO: con las laminas calculadas por area aun no se cubre toda la puerta por la forma de los cortes." << endl;
        }

        cout << "----------------------------------------------------" << endl;
        cout << "Total de laminas/retazos necesarios por area: " << cantidadLaminas << endl;
        cout << "Sobrante total estimado por area: " << arearetazos << " cm2" << endl;
        instrucciones << "Total de laminas/retazos necesarios por area: " << cantidadLaminas << endl;
        instrucciones << "Sobrante total estimado por area: " << arearetazos << " cm2" << endl;
        if (areaAcumulada >= areapuerta) {
            cout << "CORRECTO." << endl;
        } else {
            cout << "FALSO." << endl;
        }
        cout << "----------------------------------------------------" << endl;

    } else {
        // La puerta cabe en el area de una sola lamina
        cantidadLaminas = 1;
        areaAcumulada = arealamina;
        primersobrantealto = altolamina - altopuerta;
        primersobranteancho = ancholamina - anchopuerta;

        if (altopuerta <= altolamina && anchopuerta <= ancholamina) {
            cout << "Con la lamina si puedes armar la puerta" << endl;
            cout << "Debes cortar " << (altolamina - altopuerta) << " cm del alto y "
                 << (ancholamina - anchopuerta) << " cm del ancho de la lamina." << endl;
            cout << "De ese corte sobraron dos pedazos:" << endl;
            cout << "uno de " << altolamina << " cm por " << (ancholamina - anchopuerta)
                 << " cm y otro de " << (altolamina - altopuerta) << " cm por " << anchopuerta << " cm." << endl;
            cout << "no se debe agregar ningun pedazo :)" << endl;

            instrucciones << "Con la lamina si puedes armar la puerta" << endl;
            instrucciones << "Debes cortar " << (altolamina - altopuerta) << " cm del alto y "
                          << (ancholamina - anchopuerta) << " cm del ancho de la lamina." << endl;
            instrucciones << "De ese corte sobraron dos pedazos:" << endl;
            instrucciones << "uno de " << altolamina << " cm por " << (ancholamina - anchopuerta)
                          << " cm y otro de " << (altolamina - altopuerta) << " cm por " << anchopuerta << " cm." << endl;
            instrucciones << "no se debe agregar ningun pedazo :)" << endl;

            arearetazos = (altolamina * (ancholamina - anchopuerta)) + ((altolamina - altopuerta) * anchopuerta);

            cout << "----------------------------------------------------" << endl;
            if (areapuerta == (arealamina - arearetazos)) {
                cout << "CORRECTO." << endl;
            } else {
                cout << "FALSO." << endl;
            }
            cout << "----------------------------------------------------" << endl;

        } else if (anchopuerta > ancholamina) {
            // El ancho de la puerta es mayor que el ancho de la lamina
            cout << "Con la lamina si se puede armar la puerta pero toca pegar varios retazos a la derecha de la puerta." << endl;
            instrucciones << "Con la lamina si se puede armar la puerta pero toca pegar varios retazos a la derecha de la puerta." << endl;

            ensamblepuertaancho = ancholamina;
            ensamblepuertaalto = altolamina - altopuerta;

            cout << "1. corta la lamina " << altopuerta << " cm del alto por " << ancholamina
                 << " del ancho de la lamina." << endl;
            cout << "sobra un pedazo de " << ensamblepuertaalto << " cm de alto y " << ancholamina
                 << " cm de ancho." << endl;
            instrucciones << "1. corta la lamina " << altopuerta << " cm del alto por " << ancholamina
                          << " del ancho de la lamina." << endl;
            instrucciones << "sobra un pedazo de " << ensamblepuertaalto << " cm de alto y " << ancholamina
                          << " cm de ancho." << endl;

            while (ensamblepuertaancho < anchopuerta && ensamblepuertaalto >= altopuerta) {
                if ((anchopuerta - ensamblepuertaancho) > ancholamina) {
                    anchocortar = ancholamina;
                } else {
                    anchocortar = anchopuerta - ensamblepuertaancho;
                    sobranteultimoretazoancho = ancholamina - anchocortar;
                }

                cout << "Aun faltan " << (anchopuerta - ensamblepuertaancho) << " cm de ancho." << endl;
                cout << "Del sobrante de la lamina, corte una tira de " << altopuerta
                     << " cm de alto por " << anchocortar << " cm de ancho." << endl;
                instrucciones << "Aun faltan " << (anchopuerta - ensamblepuertaancho) << " cm de ancho." << endl;
                instrucciones << "Del sobrante de la lamina, corte una tira de " << altopuerta
                              << " cm de alto por " << anchocortar << " cm de ancho." << endl;

                ensamblepuertaancho = ensamblepuertaancho + anchocortar;
                ensamblepuertaalto = ensamblepuertaalto - altopuerta;

                cout << "Ancho actual de la puerta ensamblada es de " << ensamblepuertaancho << " cm." << endl;
                cout << "Sobrante restante de altura disponible es de " << ensamblepuertaalto << " cm." << endl;
                instrucciones << "Ancho actual de la puerta ensamblada es de " << ensamblepuertaancho << " cm." << endl;
                instrucciones << "Sobrante restante de altura disponible es de " << ensamblepuertaalto << " cm." << endl;
            }

            if (ensamblepuertaancho >= anchopuerta) {
                cout << "Ya tienes la puerta de " << altopuerta << " cm de alto y "
                     << ensamblepuertaancho << " cm de ancho." << endl;
                cout << "De los cortes sobraron dos pedazos:" << endl;
                instrucciones << "Ya tienes la puerta de " << altopuerta << " cm de alto y "
                              << ensamblepuertaancho << " cm de ancho." << endl;
                instrucciones << "De los cortes sobraron dos pedazos:" << endl;

                if (ensamblepuertaalto > 0) {
                    cout << "1. Un pedazo de lamina de " << ensamblepuertaalto << " cm de alto y "
                         << ancholamina << " cm de ancho." << endl;
                    instrucciones << "1. Un pedazo de lamina de " << ensamblepuertaalto << " cm de alto y "
                                  << ancholamina << " cm de ancho." << endl;
                } else {
                    cout << "1. No sobro nada de altura al fondo." << endl;
                    instrucciones << "1. No sobro nada de altura al fondo." << endl;
                }

                if (sobranteultimoretazoancho > 0) {
                    cout << "2. Un retazo lateral de " << altopuerta << " cm de alto y "
                         << sobranteultimoretazoancho << " cm de ancho." << endl;
                    instrucciones << "2. Un retazo lateral de " << altopuerta << " cm de alto y "
                                  << sobranteultimoretazoancho << " cm de ancho." << endl;
                }

                arearetazos = (ensamblepuertaalto * ancholamina) + (altopuerta * sobranteultimoretazoancho);

                cout << "----------------------------------------------------" << endl;
                if (areapuerta == (arealamina - arearetazos)) {
                    cout << "CORRECTO." << endl;
                } else {
                    cout << "FALSO." << endl;
                }
                cout << "----------------------------------------------------" << endl;
            } else {
                cout << "Lamentablemente la lamina no tiene suficiente altura para sacar mas tiras y completar el ancho de la puerta." << endl;
                instrucciones << "Lamentablemente la lamina no tiene suficiente altura para sacar mas tiras y completar el ancho de la puerta." << endl;
            }

        } else if (altopuerta > altolamina) {
            // El alto de la puerta es mayor que el alto de la lamina
            cout << "Con la lamina si puedes armar la puerta uniendo tramos de alto de " << anchopuerta << " cm de ancho." << endl;
            instrucciones << "Con la lamina si puedes armar la puerta uniendo tramos de alto de " << anchopuerta << " cm de ancho." << endl;

            ensamblepuertaalto = altolamina;
            ensamblepuertaancho = anchopuerta;

            cout << "1. corta la lamina a todo el alto de " << altolamina << " cm por un ancho de "
                 << anchopuerta << " cm." << endl;
            instrucciones << "1. corta la lamina a todo el alto de " << altolamina << " cm por un ancho de "
                          << anchopuerta << " cm." << endl;

            while (ensamblepuertaalto < altopuerta && (ancholamina - ensamblepuertaancho) >= anchopuerta) {
                if ((altopuerta - ensamblepuertaalto) > altolamina) {
                    altocortar = altolamina;
                } else {
                    altocortar = altopuerta - ensamblepuertaalto;
                    sobranteultimoretazoalto = altolamina - altocortar;
                }

                cout << "Aun faltan tramos para terminar la puerta " << (altopuerta - ensamblepuertaalto)
                     << " cm de alto." << endl;
                cout << "Corte otro tramo de la lamina de " << altocortar
                     << " cm de alto por " << anchopuerta << " cm de ancho y unalo para subir el alto." << endl;
                instrucciones << "Aun faltan tramos para terminar la puerta " << (altopuerta - ensamblepuertaalto)
                              << " cm de alto." << endl;
                instrucciones << "Corte otro tramo de la lamina de " << altocortar
                              << " cm de alto por " << anchopuerta << " cm de ancho y unalo para subir el alto." << endl;

                ensamblepuertaalto = ensamblepuertaalto + altocortar;
                ensamblepuertaancho = ensamblepuertaancho + anchopuerta;

                cout << "Altura actual de la puerta ensamblada es de " << ensamblepuertaalto << " cm." << endl;
                instrucciones << "Altura actual de la puerta ensamblada es de " << ensamblepuertaalto << " cm." << endl;
            }

            sobrantelateralancho = ancholamina - ensamblepuertaancho;

            if (ensamblepuertaalto >= altopuerta) {
                // Bug corregido: se imprime altopuerta y anchopuerta reales
                cout << "Ya tienes la puerta de " << altopuerta << " cm de alto y "
                     << anchopuerta << " cm de ancho." << endl;
                cout << "De los cortes sobraron dos pedazos:" << endl;
                instrucciones << "Ya tienes la puerta de " << altopuerta << " cm de alto y "
                              << anchopuerta << " cm de ancho." << endl;
                instrucciones << "De los cortes sobraron dos pedazos:" << endl;

                if (sobrantelateralancho > 0) {
                    cout << "1. Una tira lateral de " << altolamina << " cm de alto y "
                         << sobrantelateralancho << " cm de ancho." << endl;
                    instrucciones << "1. Una tira lateral de " << altolamina << " cm de alto y "
                                  << sobrantelateralancho << " cm de ancho." << endl;
                } else {
                    cout << "1. No sobro nada de ancho lateral." << endl;
                    instrucciones << "1. No sobro nada de ancho lateral." << endl;
                }

                if (sobranteultimoretazoalto > 0) {
                    cout << "2. Un retazo superior de " << sobranteultimoretazoalto << " cm de alto y "
                         << anchopuerta << " cm de ancho." << endl;
                    instrucciones << "2. Un retazo superior de " << sobranteultimoretazoalto << " cm de alto y "
                                  << anchopuerta << " cm de ancho." << endl;
                } else {
                    cout << "2. No sobro nada arriba en el ultimo corte." << endl;
                    instrucciones << "2. No sobro nada arriba en el ultimo corte." << endl;
                }

                arearetazos = (altolamina * sobrantelateralancho) + (sobranteultimoretazoalto * anchopuerta);

                cout << "----------------------------------------------------" << endl;
                if (areapuerta == (arealamina - arearetazos)) {
                    cout << "CORRECTO." << endl;
                } else {
                    cout << "FALSO." << endl;
                }
                cout << "----------------------------------------------------" << endl;
            } else {
                cout << "Lamentablemente la lamina no tiene suficiente ancho para sacar mas trozos y pegarlas arriba para completar la altura de la puerta." << endl;
                instrucciones << "Lamentablemente la lamina no tiene suficiente ancho para sacar mas trozos y pegarlas arriba para completar la altura de la puerta." << endl;
            }
        } else {
            cout << "Lamentablemente con esas medidas para la puerta no puedes sacarla solo con la lamina." << endl;
            instrucciones << "Lamentablemente con esas medidas para la puerta no puedes sacarla solo con la lamina." << endl;
        }
    }

    // Resumen en pantalla de todas las variables calculadas
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

    // ----- Guardado de los resultados en archivos de texto -----
    char guardarArchivo;
    cout << "Desea guardar los resultados en un archivo de texto? (s/n): ";
    cin >> guardarArchivo;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (guardarArchivo == 's' || guardarArchivo == 'S') {
        DatosRecibo recibo;

        cout << "Ingrese el nombre del archivo (sin extension): ";
        cin >> recibo.nombreArchivo;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "Ingrese el nombre del cliente: ";
        getline(cin, recibo.cliente);

        cout << "Ingrese el material de la lamina: ";
        getline(cin, recibo.material);

        recibo.altoLamina = altolamina;
        recibo.anchoLamina = ancholamina;
        recibo.areaLamina = arealamina;
        recibo.altoPuerta = altopuerta;
        recibo.anchoPuerta = anchopuerta;
        recibo.areaPuerta = areapuerta;
        recibo.cantidadLaminas = cantidadLaminas;
        recibo.areaRetazos = arearetazos;

        string rutaCarpeta = "recibos/";
        string archivoRecibo = rutaCarpeta + recibo.nombreArchivo + ".txt";

        ofstream archivo(archivoRecibo.c_str());
        if (archivo.is_open()) {
            archivo << "=== Recibo Digital ===" << endl;
            archivo << "Cliente: " << recibo.cliente << endl;
            archivo << "Material de la lamina: " << recibo.material << endl;
            archivo << endl;
            archivo << "Alto lamina: " << recibo.altoLamina << " cm" << endl;
            archivo << "Ancho lamina: " << recibo.anchoLamina << " cm" << endl;
            archivo << "Area lamina: " << recibo.areaLamina << " cm2" << endl;
            archivo << "Alto puerta: " << recibo.altoPuerta << " cm" << endl;
            archivo << "Ancho puerta: " << recibo.anchoPuerta << " cm" << endl;
            archivo << "Area puerta: " << recibo.areaPuerta << " cm2" << endl;
            archivo << "Cantidad laminas/retazos: " << recibo.cantidadLaminas << endl;
            archivo << "Area de retazos/sobrantes: " << recibo.areaRetazos << " cm2" << endl;

            archivo.close();
            cout << "Recibo guardado como " << archivoRecibo << endl;
        } else {
            cout << "No se pudo crear el archivo del recibo." << endl;
        }

        instruccionescortes(instrucciones.str(), "instrucciones_corte.txt", rutaCarpeta);
    }

    return 0;
}

int tamanolamina() {
    altolamina = leerNumeroPositivo("Cual es el alto de la lamina en cm", false);
    ancholamina = leerNumeroPositivo("Cual es el ancho de la lamina en cm", false);

    cout << "Nuevo tamano de la lamina: " << altolamina << " cm de alto y " << ancholamina << " cm de ancho." << endl;

    return 0;
}

int instruccionescortes(const string& textoInstrucciones, const string& nombreArchivo, const string& rutaCarpeta) {
    string rutaCompleta = rutaCarpeta + nombreArchivo;

    ofstream archivoCortes(rutaCompleta.c_str());
    if (!archivoCortes.is_open()) {
        cout << "No se pudo crear el archivo de instrucciones en: " << rutaCompleta << endl;
        return 1;
    }

    archivoCortes << "=== INSTRUCCIONES DE CORTE ===" << endl << endl;
    archivoCortes << textoInstrucciones;
    archivoCortes.close();

    cout << "Instrucciones guardadas como " << rutaCompleta << endl;
    return 0;
}

void mostrarArchivosCortes(const string& nombreArchivo, const string& rutaCarpeta) {
    string rutaCompleta = rutaCarpeta + nombreArchivo;

    ifstream archivoLectura(rutaCompleta.c_str());
    if (!archivoLectura.is_open()) {
        cout << "Aun no existe el archivo " << rutaCompleta << endl;
        cout << "Primero calcula unos cortes (opcion 1) y guardalos con 's' para crearlo." << endl;
        return;
    }

    string linea;
    cout << "----- Contenido de " << rutaCompleta << " -----" << endl;
    while (getline(archivoLectura, linea)) {
        cout << linea << endl;
    }
    cout << "----- Fin del archivo -----" << endl;

    archivoLectura.close();
}
