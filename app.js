const inputArchivo = document.getElementById('archivoInput');
const btnCargar    = document.getElementById('btnCargar');
const btnImprimir  = document.getElementById('btnImprimir');
const reciboEl     = document.getElementById('recibo');
const estadoEl     = document.getElementById('estado');

/**
 * Carga el archivo .txt generado por C++.
 * El archivo debe estar en la carpeta /recibos del servidor.
 */
async function cargarRecibo(nombreArchivo) {
  const ruta = `recibos/${encodeURIComponent(nombreArchivo)}?_=${Date.now()}`;

  setEstado('Cargando...', '');

  try {
    const resp = await fetch(ruta, { cache: 'no-store' });

    if (!resp.ok) throw new Error(`HTTP ${resp.status} - ${resp.statusText}`);

    const texto = await resp.text();

    if (!texto.trim()) throw new Error('El archivo está vacío.');

    reciboEl.textContent = texto;
    btnImprimir.disabled = false;
    setEstado(`✔ Recibo "${nombreArchivo}" cargado correctamente.`, 'ok');
  } catch (err) {
    reciboEl.textContent = 'No se pudo cargar el recibo.';
    btnImprimir.disabled = true;
    setEstado(`✖ Error: ${err.message}. Verifica que el archivo exista en /recibos.`, 'err');
    console.error(err);
  }
}

function setEstado(msg, tipo) {
  estadoEl.textContent = msg;
  estadoEl.className = 'estado' + (tipo ? ' ' + tipo : '');
}

function imprimir() {
  if (btnImprimir.disabled) return;
  window.print();
}

// ====== Eventos ======
btnCargar.addEventListener('click', () => {
  const nombre = inputArchivo.value.trim();
  if (!nombre) {
    setEstado('Escribe el nombre del archivo (ej: recibo_001.txt).', 'err');
    return;
  }
  cargarRecibo(nombre);
});

// Enter en el input = cargar
inputArchivo.addEventListener('keydown', (e) => {
  if (e.key === 'Enter') btnCargar.click();
});

btnImprimir.addEventListener('click', imprimir);

// Auto-cargar si llega ?archivo=recibo_001.txt
window.addEventListener('DOMContentLoaded', () => {
  const params = new URLSearchParams(location.search);
  const archivo = params.get('archivo');
  if (archivo) {
    inputArchivo.value = archivo;
    cargarRecibo(archivo);
  }
});