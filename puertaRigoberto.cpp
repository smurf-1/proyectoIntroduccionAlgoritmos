// Juan Sebastian Higinio, Cristian Bernal y David Bejarano
// Algoritmo PuertaRigoberto
#include <iostream>
#include <fstream>//Para crear, escribir y leer archivos .txt
#include <sstream> // Para ir acumulando todas las instrucciones y mensajes
#include <string>

using namespace std;
// struct DatosRecibo que Se llena justo antes de guardar y de aqui se escribe el primer .txt (resumen del trabajo).

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
// (los valores por defecto de los parametros se escriben SOLO aqui, en el prototipo,
//  y no se repiten en la definicion de la funcion)
int menuopciones();
int calcularcortes();
int tamanolamina();
int instruccionescortes(const string& textoInstrucciones, const string& nombreArchivo = "instrucciones_corte.txt", const string& rutaCarpeta = "");
void mostrarArchivosCortes(const string& nombreArchivo = "instrucciones_corte.txt", const string& rutaCarpeta = "");

double altolamina = 100, ancholamina = 150; // Tamaño por defecto de la lamina (se puede cambiar en el menu)

// Muestra el menu principal y devuelve la opcion que eligio el usuario
int menuopciones(){
    int opcion;
    cout << "=== Menu de Opciones ===" << endl;
    cout << "1. Ingresar medidas de la puerta" << endl;
    cout << "2. Cambiar el Tamano de la Lamina" << endl;
    cout << "3. Mostrar archivos de Cortes" << endl;
    cout << "4. Salir" << endl;
    cout << "\nSeleccione una opcion: ";
    
    cin >> opcion; // Se lee la opcion elegida

    return opcion;
}

// Funcion principal: repite el menu hasta que el usuario elija salir (opcion 4)
int main(){

    int opcion;

    do {
        opcion = menuopciones(); // Se muestra el menu y se lee la opcion
        switch (opcion) {
            case 1:
                calcularcortes(); // Llamar a la función principal para ingresar medidas de la puerta
                break;
            case 2:
                tamanolamina(); // Llamar a la función para cambiar el tamaño de la lámina
                break;
            case 3:
                cout << "Opcion 3 seleccionada: Mostrar archivos de Cortes" << endl;
                mostrarArchivosCortes(); // Llamar a la función que lee y muestra el archivo de cortes
                break;
            case 4:
                cout << "Saliendo del programa..." << endl;
                break;
            default:
                cout << "Opcion invalida. Por favor, seleccione una opcion valida." << endl;
        }
    } while (opcion != 4); // El menu se repite mientras no se elija salir

    return 0;
}



// Pide las medidas, calcula los cortes, muestra las instrucciones en pantalla
// y, si el usuario quiere, las guarda en archivos de texto
int calcularcortes() {

    // Medidas de la puerta y de la lamina
    double altopuerta = 0, anchopuerta = 0;
    // Areas y primeros sobrantes (lo que sobra al cortar la puerta de la lamina)
    double arealamina = 0, areapuerta = 0, primersobrantealto = 0, primersobranteancho = 0;
    // Variables para ir armando la puerta cuando toca pegar retazos
    double ensamblepuertaancho = 0, ensamblepuertaalto = 0, anchocortar = 0, altocortar = 0;
    // Sobrantes que quedan despues de los cortes
    double sobranteultimoretazoancho = 0, sobrantelateralancho = 0, sobranteultimoretazoalto = 0;
    double arearetazos = 0;     // Area total de lo que sobra
    double areaAcumulada = 0;   // Area acumulada de las laminas usadas
    int cantidadLaminas = 0;    // Cantidad de laminas/retazos necesarios

    // NUEVO: variables para repetir las instrucciones de corte por cada lamina
    // (se usan cuando se necesita mas de una lamina)
    double altoCubierto = 0;      // Alto de la puerta que ya esta cubierto con filas completas
    double anchoCubierto = 0;     // Ancho cubierto de la fila que se esta armando
    double altoPieza = 0;         // Alto de la pieza que se corta de la lamina actual
    double anchoPieza = 0;        // Ancho de la pieza que se corta de la lamina actual
    int filaActual = 1;           // Numero de la fila de la puerta que se esta armando
    bool puertaCubierta = false;  // Indica si ya se cubrio el alto y el ancho de la puerta

    // Aqui se van guardando las instrucciones de corte para escribirlas luego en el .txt
    stringstream instrucciones;


    arealamina = altolamina * ancholamina; // Area de la lamina
    arearetazos = 0;

    // No se pueden ingresar numeros negativos ni 0 en las medidas de la puerta
    do {
        cout << "Cual es el alto de la puerta en cm" << endl;
        cin >> altopuerta;
        cout << "Cual es el ancho de la puerta en cm" << endl;
        cin >> anchopuerta;

        // Si alguna medida es negativa se avisa (el ciclo tambien rechaza el 0)
        if (altopuerta < 0 || anchopuerta < 0) {
            cout << "?? No puedes hacer una puerta negativa" << endl;
        }
    } while (!(altopuerta > 0 && anchopuerta > 0));

    areapuerta = altopuerta * anchopuerta; // Area de la puerta

    // Si el area de la puerta es mayor que una sola lamina, se agregan laminas/retazos en bucle
    if (areapuerta > arealamina) {
        cout << "Con una sola lamina no alcanza. Se agregaran laminas/retazos adicionales del mismo tamano." << endl;
        instrucciones << "Con una sola lamina no alcanza. Se agregaran laminas/retazos adicionales del mismo tamano." << endl;

        // En cada vuelta se agrega una lamina hasta cubrir el area de la puerta
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

        arearetazos = areaAcumulada - areapuerta; // Lo que sobra en total

        // ------------------------------------------------------------------
        // NUEVO: repetir las instrucciones de corte por cada lamina necesaria.
        // La puerta se arma por filas: en cada lamina se corta una pieza que se
        // pega a la derecha de lo ya armado; cuando la fila completa el ancho de
        // la puerta, se empieza la fila siguiente (hacia arriba) hasta cubrir el alto.
        // ------------------------------------------------------------------
        for (int numeroLamina = 1; numeroLamina <= cantidadLaminas; numeroLamina++) {

            // Se indica con cual lamina se esta trabajando
            cout << "----- Lamina " << numeroLamina << " de " << cantidadLaminas
                 << " (" << altolamina << "x" << ancholamina << " cm) -----" << endl;
            instrucciones << "----- Lamina " << numeroLamina << " de " << cantidadLaminas
                          << " (" << altolamina << "x" << ancholamina << " cm) -----" << endl;

            if (puertaCubierta) {
                // Si la puerta ya estaba completa, esta lamina no se corta
                cout << "La puerta ya esta cubierta; esta lamina no hace falta cortarla." << endl;
                instrucciones << "La puerta ya esta cubierta; esta lamina no hace falta cortarla." << endl;
            } else {
                // Ancho de la pieza: toda la lamina, o solo lo que falta en la fila actual
                anchoPieza = ancholamina;
                if ((anchopuerta - anchoCubierto) < ancholamina) {
                    anchoPieza = anchopuerta - anchoCubierto;
                }
                // Alto de la pieza: toda la lamina, o solo lo que falta de alto en la puerta
                altoPieza = altolamina;
                if ((altopuerta - altoCubierto) < altolamina) {
                    altoPieza = altopuerta - altoCubierto;
                }

                // Instruccion de corte de esta lamina
                cout << "1. Corte de la lamina " << numeroLamina << " una pieza de " << altoPieza
                     << " cm de alto por " << anchoPieza << " cm de ancho." << endl;
                instrucciones << "1. Corte de la lamina " << numeroLamina << " una pieza de " << altoPieza
                              << " cm de alto por " << anchoPieza << " cm de ancho." << endl;

                // Instruccion de pegado: al inicio de la fila o a la derecha de lo ya armado
                if (anchoCubierto == 0) {
                    cout << "2. Pegue la pieza al inicio de la fila " << filaActual << " de la puerta." << endl;
                    instrucciones << "2. Pegue la pieza al inicio de la fila " << filaActual << " de la puerta." << endl;
                } else {
                    cout << "2. Pegue la pieza en la fila " << filaActual
                         << " de la puerta, a la derecha de lo que ya tiene armado." << endl;
                    instrucciones << "2. Pegue la pieza en la fila " << filaActual
                                  << " de la puerta, a la derecha de lo que ya tiene armado." << endl;
                }

                // Se actualiza el ancho cubierto de la fila actual
                anchoCubierto = anchoCubierto + anchoPieza;
                cout << "Ancho cubierto de la fila " << filaActual << ": " << anchoCubierto
                     << " cm de " << anchopuerta << " cm." << endl;
                instrucciones << "Ancho cubierto de la fila " << filaActual << ": " << anchoCubierto
                              << " cm de " << anchopuerta << " cm." << endl;

                // Si la fila ya completo el ancho de la puerta, se suma su alto y se pasa a la fila siguiente
                if (anchoCubierto >= anchopuerta) {
                    altoCubierto = altoCubierto + altoPieza;
                    cout << "Fila " << filaActual << " completa. Alto cubierto de la puerta: " << altoCubierto
                         << " cm de " << altopuerta << " cm." << endl;
                    instrucciones << "Fila " << filaActual << " completa. Alto cubierto de la puerta: " << altoCubierto
                                  << " cm de " << altopuerta << " cm." << endl;
                    anchoCubierto = 0;
                    filaActual++;
                }

                // Sobrantes que dejo esta lamina despues del corte
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

                // Verificacion: se revisa si ya se cubrieron el alto y el ancho de la puerta
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

        // Si al terminar todas las laminas calculadas por area la puerta no quedo cubierta, se avisa
        if (!puertaCubierta) {
            cout << "AVISO: con las laminas calculadas por area aun no se cubre toda la puerta por la forma de los cortes." << endl;
            instrucciones << "AVISO: con las laminas calculadas por area aun no se cubre toda la puerta por la forma de los cortes." << endl;
        }

        cout << "----------------------------------------------------" << endl;
        cout << "Total de laminas/retazos necesarios por area: " << cantidadLaminas << endl;
        cout << "Sobrante total estimado por area: " << arearetazos << " cm2" << endl;
        instrucciones << "Total de laminas/retazos necesarios por area: " << cantidadLaminas << endl;
        instrucciones << "Sobrante total estimado por area: " << arearetazos << " cm2" << endl;
        // Verificacion: el area acumulada debe cubrir el area de la puerta
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

        // Para cuando sobra tanto del ancho como del alto
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

            // Area de los dos pedazos que sobran
            arearetazos = (altolamina * (ancholamina - anchopuerta)) + ((altolamina - altopuerta) * anchopuerta);

            cout << "----------------------------------------------------" << endl;
            // Verificacion: area puerta = area lamina - area sobrantes
            if (areapuerta == (arealamina - arearetazos)) {
                cout << "CORRECTO." << endl;
            } else {
                cout << "FALSO." << endl;
            }
            cout << "----------------------------------------------------" << endl;

        } else { // -----el ancho de la puerta es mas grande que el ancho de la lamina-----
            if (anchopuerta > ancholamina) {
                cout << "Con la lamina si se puede armar la puerta pero toca pegar varios retazos a la derecha de la puerta." << endl;
                instrucciones << "Con la lamina si se puede armar la puerta pero toca pegar varios retazos a la derecha de la puerta." << endl;

                // Ancho que ya se tiene armado y altura de lamina que queda disponible
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

                // Mientras falte ancho y quede altura suficiente, se cortan tiras del sobrante
                while (ensamblepuertaancho < anchopuerta && ensamblepuertaalto >= altopuerta) {
                    // Si falta mas de una lamina de ancho se corta completa; si no, solo lo que falta
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

                    // Se actualiza lo armado y la altura que queda
                    ensamblepuertaancho = ensamblepuertaancho + anchocortar;
                    ensamblepuertaalto = ensamblepuertaalto - altopuerta;

                    cout << "Ancho actual de la puerta ensamblada es de " << ensamblepuertaancho << " cm." << endl;
                    cout << "Sobrante restante de altura disponible es de " << ensamblepuertaalto << " cm." << endl;
                    instrucciones << "Ancho actual de la puerta ensamblada es de " << ensamblepuertaancho << " cm." << endl;
                    instrucciones << "Sobrante restante de altura disponible es de " << ensamblepuertaalto << " cm." << endl;
                }

                // Se revisa si se logro completar el ancho de la puerta
                if (ensamblepuertaancho >= anchopuerta) {
                    cout << "Ya tienes la puerta de " << altopuerta << " cm de alto y "
                         << ensamblepuertaancho << " cm de ancho." << endl;
                    cout << "De los cortes sobraron dos pedazos:" << endl;
                    instrucciones << "Ya tienes la puerta de " << altopuerta << " cm de alto y "
                                  << ensamblepuertaancho << " cm de ancho." << endl;
                    instrucciones << "De los cortes sobraron dos pedazos:" << endl;

                    // Primer sobrante: altura que queda al fondo de la lamina
                    if (ensamblepuertaalto > 0) {
                        cout << "1. Un pedazo de lamina de " << ensamblepuertaalto << " cm de alto y "
                             << ancholamina << " cm de ancho." << endl;
                        instrucciones << "1. Un pedazo de lamina de " << ensamblepuertaalto << " cm de alto y "
                                      << ancholamina << " cm de ancho." << endl;
                    } else {
                        cout << "1. No sobro nada de altura al fondo." << endl;
                        instrucciones << "1. No sobro nada de altura al fondo." << endl;
                    }

                    // Segundo sobrante: lo que sobra del ultimo retazo lateral
                    if (sobranteultimoretazoancho > 0) {
                        cout << "2. Un retazo lateral de " << altopuerta << " cm de alto y "
                             << sobranteultimoretazoancho << " cm de ancho." << endl;
                        instrucciones << "2. Un retazo lateral de " << altopuerta << " cm de alto y "
                                      << sobranteultimoretazoancho << " cm de ancho." << endl;
                    }

                    // Area de los sobrantes
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

            } else { // -----el alto de la puerta es mas grande que el alto de la lamina-----
                if (altopuerta > altolamina) {
                    cout << "Con la lamina si puedes armar la puerta pero toca pegar varios retazos arriba de la puerta." << endl;
                    instrucciones << "Con la lamina si puedes armar la puerta pero toca pegar varios retazos arriba de la puerta." << endl;

                    // Alto que ya se tiene armado y ancho que se usa de la lamina
                    ensamblepuertaalto = altolamina;
                    ensamblepuertaancho = anchopuerta;

                    cout << "1. corta la lamina a todo el alto de " << altolamina << " cm por un ancho de "
                         << anchopuerta << " cm." << endl;
                    instrucciones << "1. corta la lamina a todo el alto de " << altolamina << " cm por un ancho de "
                                  << anchopuerta << " cm." << endl;

                    // Mientras falte alto y todavia quepa ancho en la lamina, se cortan tiras
                    while (ensamblepuertaalto < altopuerta && ensamblepuertaancho <= ancholamina) {
                        // Si falta mas de una lamina de alto se corta completa; si no, solo lo que falta
                        if ((altopuerta - ensamblepuertaalto) > altolamina) {
                            altocortar = altolamina;
                        } else {
                            altocortar = altopuerta - ensamblepuertaalto;
                            sobranteultimoretazoalto = altolamina - altocortar;
                        }

                        cout << "Aun faltan retazos para terminar la puerta " << (altopuerta - ensamblepuertaalto)
                             << " cm de alto." << endl;
                        cout << "Del sobrante de la lamina, corte una tira de " << altocortar
                             << " cm de alto por " << anchopuerta << " cm de ancho." << endl;
                        instrucciones << "Aun faltan retazos para terminar la puerta " << (altopuerta - ensamblepuertaalto)
                                      << " cm de alto." << endl;
                        instrucciones << "Del sobrante de la lamina, corte una tira de " << altocortar
                                      << " cm de alto por " << anchopuerta << " cm de ancho." << endl;

                        // Se actualiza lo armado
                        ensamblepuertaalto = ensamblepuertaalto + altocortar;
                        ensamblepuertaancho = ensamblepuertaancho + anchopuerta;

                        cout << "Altura actual de la puerta ensamblada es de " << ensamblepuertaalto << " cm." << endl;
                        instrucciones << "Altura actual de la puerta ensamblada es de " << ensamblepuertaalto << " cm." << endl;
                    }

                    // Ancho de lamina que queda libre al lado
                    sobrantelateralancho = ancholamina - ensamblepuertaancho;

                    // Se revisa si se logro completar el alto de la puerta
                    if (ensamblepuertaalto >= altopuerta) {
                        cout << "Ya tienes la puerta de " << altocortar << " cm de alto y "
                             << anchopuerta << " cm de ancho." << endl;
                        cout << "De los cortes sobraron dos pedazos:" << endl;
                        instrucciones << "Ya tienes la puerta de " << altocortar << " cm de alto y "
                                      << anchopuerta << " cm de ancho." << endl;
                        instrucciones << "De los cortes sobraron dos pedazos:" << endl;

                        // Primer sobrante: tira lateral
                        if (sobrantelateralancho > 0) {
                            cout << "1. Una tira lateral de " << altolamina << " cm de alto y "
                                 << sobrantelateralancho << " cm de ancho." << endl;
                            instrucciones << "1. Una tira lateral de " << altolamina << " cm de alto y "
                                          << sobrantelateralancho << " cm de ancho." << endl;
                        } else {
                            cout << "1. No sobro nada de ancho lateral." << endl;
                            instrucciones << "1. No sobro nada de ancho lateral." << endl;
                        }

                        // Segundo sobrante: retazo superior del ultimo corte
                        if (sobranteultimoretazoalto > 0) {
                            cout << "2. Un retazo superior de " << sobranteultimoretazoalto << " cm de alto y "
                                 << anchopuerta << " cm de ancho." << endl;
                            instrucciones << "2. Un retazo superior de " << sobranteultimoretazoalto << " cm de alto y "
                                          << anchopuerta << " cm de ancho." << endl;
                        } else {
                            cout << "2. No sobro nada arriba en el ultimo corte." << endl;
                            instrucciones << "2. No sobro nada arriba en el ultimo corte." << endl;
                        }

                        // Area de los sobrantes
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

    if (guardarArchivo == 's' || guardarArchivo == 'S') {

        // NUEVO: se crea el struct y se van llenando sus campos con los datos pedidos y calculados
        DatosRecibo recibo;

        // Nombre del archivo del recibo (sin extension, se le agrega ".txt" mas abajo)
        cout << "Ingrese el nombre del archivo (sin extension): ";
        cin >> recibo.nombreArchivo;

        cin.ignore(); // Se descarta el salto de linea pendiente para que getline funcione bien

        // Nombre del cliente (getline permite escribir espacios)
        cout << "Ingrese el nombre del cliente: ";
        getline(cin, recibo.cliente);

        // Material de la lamina (dato informativo, no afecta los calculos)
        cout << "Ingrese el material de la lamina: ";
        getline(cin, recibo.material);

        // Se copian al struct los datos de la lamina, la puerta y los resultados
        recibo.altoLamina = altolamina;
        recibo.anchoLamina = ancholamina;
        recibo.areaLamina = arealamina;
        recibo.altoPuerta = altopuerta;
        recibo.anchoPuerta = anchopuerta;
        recibo.areaPuerta = areapuerta;
        recibo.cantidadLaminas = cantidadLaminas;
        recibo.areaRetazos = arearetazos;

        // Carpeta donde se guardaran los archivos.
        // Si se deja vacia (""), C++ guarda en la misma carpeta donde se ejecuta el .exe.
        // Para guardar en otra carpeta que YA exista, escribe la ruta con "\\" al final,
        // por ejemplo: string rutaCarpeta = "C:\\Recibos\\";
        string rutaCarpeta = "";

        // Nombre completo del archivo del recibo
        string archivoRecibo = rutaCarpeta + recibo.nombreArchivo + ".txt"; // Nombre elegido por el usuario

        // ----- ARCHIVO 1: recibo digital (solo datos del struct) -----
        // Se usa c_str() para que compile en Dev-C++ / C++ clasico
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

            archivo.close(); // Se cierra el archivo del recibo
            cout << "Recibo guardado como " << archivoRecibo << endl;
        }
        else {
            cout << "No se pudo crear el archivo del recibo." << endl;
        }

        // ----- ARCHIVO 2: instrucciones de corte (solo las instrucciones paso a paso) -----
        // Se llama a instruccionescortes, que crea el archivo y muestra el mensaje en pantalla.
        // El nombre es fijo ("instrucciones_corte.txt") y se sobrescribe en cada guardado.
        instruccionescortes(instrucciones.str(), "instrucciones_corte.txt", rutaCarpeta);
    }

    return 0;
}

int tamanolamina() {

    //no se pueden ingresar numeros negativos ni 0 en las medidas de la lamina
    do{
        cout << "Cual es el alto de la lamina en cm" << endl;
        cin >> altolamina;
        cout << "Cual es el ancho de la lamina en cm" << endl;
        cin >> ancholamina;

        // Si alguna medida es 0 o negativa se avisa y se vuelven a pedir
        if (altolamina <= 0 || ancholamina <= 0) {
            cout << "?? No puedes hacer una lamina negativa o de tamaño cero." << endl;
        }
    } while (altolamina <= 0 || ancholamina <= 0);

    cout << "Nuevo tamaño de la lamina: " << altolamina << " cm de alto y " << ancholamina << " cm de ancho." << endl;

    return 0;
}

// Guarda las instrucciones de corte en un archivo .txt y devuelve 0 si todo salio bien
// (los valores por defecto de nombreArchivo y rutaCarpeta ya estan en el prototipo del inicio)
int instruccionescortes(const string& textoInstrucciones, const string& nombreArchivo, const string& rutaCarpeta) {
    string rutaCompleta = rutaCarpeta + nombreArchivo;

    ofstream archivoCortes(rutaCompleta.c_str());
    if (!archivoCortes.is_open()) {
        cout << "No se pudo crear el archivo de instrucciones en: " << rutaCompleta << endl;
        return 1; // Indica que hubo un error al abrir/crear el archivo
    }

    archivoCortes << "=== INSTRUCCIONES DE CORTE ===" << endl << endl;
    archivoCortes << textoInstrucciones;
    archivoCortes.close();

    cout << "Instrucciones guardadas como " << rutaCompleta << endl;
    return 0;
}

// Lee el archivo de instrucciones de corte y lo muestra en pantalla linea por linea.
// Si el archivo todavia no existe, avisa con un mensaje claro.
// (los valores por defecto de nombreArchivo y rutaCarpeta ya estan en el prototipo del inicio)
void mostrarArchivosCortes(const string& nombreArchivo, const string& rutaCarpeta) {
    string rutaCompleta = rutaCarpeta + nombreArchivo;

    // Se abre el archivo en modo lectura
    ifstream archivoLectura(rutaCompleta.c_str());
    if (!archivoLectura.is_open()) {
        cout << "Aun no existe el archivo " << rutaCompleta << endl;
        cout << "Primero calcula unos cortes (opcion 1) y guardalos con 's' para crearlo." << endl;
        return; // No hay nada que mostrar
    }

    string linea; // Aqui se guarda cada linea leida del archivo

    cout << "----- Contenido de " << rutaCompleta << " -----" << endl;
    while (getline(archivoLectura, linea)) {
        cout << linea << endl; // Se muestra cada linea en pantalla
    }
    cout << "----- Fin del archivo -----" << endl;

    archivoLectura.close(); // Se cierra el archivo
}
