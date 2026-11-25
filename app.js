const inputArchivo = document.getElementById('archivoInput');
const btnCargar    = document.getElementById('btnCargar');
const btnImprimir  = document.getElementById('btnImprimir');
const btnCopiar    = document.getElementById('btnCopiar');
const contenido    = document.getElementById('contenido');
const receiptEl    = document.getElementById('receipt');
const estadoEl     = document.getElementById('estado');

async function cargarRecibo(nombre) {
  const ruta = `recibos/${encodeURIComponent(nombre)}?_=${Date.now()}`;

  estadoEl.textContent = 'Cargando...';
  estadoEl.className = 'estado';
  receiptEl.classList.remove('show');
  btnImprimir.disabled = true;
  btnCopiar.disabled = true;

  try {
    const resp = await fetch(ruta, { cache: 'no-store' });
    if (!resp.ok) throw new Error(`HTTP ${resp.status}`);
    const texto = await resp.text();
    if (!texto.trim()) throw new Error('Archivo vacío');

    contenido.textContent = texto;

    // pequeña pausa para que se vea la animación "salir del ticket"
    setTimeout(() => {
      receiptEl.classList.add('show');
      btnImprimir.disabled = false;
      btnCopiar.disabled = false;
    }, 150);

    estadoEl.textContent = `✔ ${nombre} cargado`;
    estadoEl.className = 'estado ok';
  } catch (e) {
    contenido.textContent = 'No se pudo cargar el recibo.';
    estadoEl.textContent = `✖ Error: ${e.message}`;
    estadoEl.className = 'estado err';
  }
}

btnCargar.addEventListener('click', () => {
  const n = inputArchivo.value.trim();
  if (n) cargarRecibo(n);
});

inputArchivo.addEventListener('keydown', e => {
  if (e.key === 'Enter') btnCargar.click();
});

btnImprimir.addEventListener('click', () => {
  // animación "printing"
  document.body.classList.add('printing');
  setTimeout(() => {
    window.print();
    document.body.classList.remove('printing');
  }, 400);
});

btnCopiar.addEventListener('click', async () => {
  try {
    await navigator.clipboard.writeText(contenido.textContent);
    estadoEl.textContent = '📋 Recibo copiado al portapapeles';
    estadoEl.className = 'estado ok';
  } catch {
    estadoEl.textContent = 'No se pudo copiar';
    estadoEl.className = 'estado err';
  }
});

// Auto-carga si viene ?archivo= en la URL
window.addEventListener('DOMContentLoaded', () => {
  const archivo = new URLSearchParams(location.search).get('archivo');
  if (archivo) {
    inputArchivo.value = archivo;
    cargarRecibo(archivo);
  }
});