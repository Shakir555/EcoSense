<template>
  <!-- CHARTS GRID -->
  <div class="grid gap-6 mt-10 grid-cols-1 md:grid-cols-2 lg:grid-cols-3">

    <!-- Temperature Chart -->
    <div class="glass-card card-hover p-4 cursor-pointer"
      @click="openModal(
        'Temperature History',
        history.map(p => p.temperature),
        history.map(p => p.time),
        'rgba(16,185,129,1)',
        'rgba(16,185,129,0.12)'
      )">
      <h3 class="text-xs font-semibold uppercase tracking-[0.2em] text-gray-300 mb-2">
        Temperature
      </h3>
      <div class="h-64">
        <canvas ref="tempCanvas"></canvas>
      </div>
    </div>

    <!-- Humidity Chart -->
    <div class="glass-card card-hover p-4 cursor-pointer"
      @click="openModal(
        'Humidity History',
        history.map(p => p.humidity),
        history.map(p => p.time),
        'rgba(56,189,248,1)',
        'rgba(56,189,248,0.12)'
      )">
      <h3 class="text-xs font-semibold uppercase tracking-[0.2em] text-gray-300 mb-2">
        Humidity
      </h3>
      <div class="h-64">
        <canvas ref="humidityCanvas"></canvas>
      </div>
    </div>

    <!-- Rain Chart -->
    <div class="glass-card card-hover p-4 cursor-pointer"
      @click="openModal(
        'Rain Wetness (%)',
        rainHistory.map(p => p.percent),
        rainHistory.map(p => p.time),
        'rgba(96,165,250,1)',
        'rgba(96,165,250,0.12)'
      )">
      <h3 class="text-xs font-semibold uppercase tracking-[0.2em] text-gray-300 mb-2">
        Rain Wetness (%)
      </h3>
      <div class="h-64">
        <canvas ref="rainSensorCanvas"></canvas>
      </div>
    </div>

  </div>

  <!-- FULLSCREEN MODAL -->
  <div
    v-if="showModal"
    class="fixed inset-0 bg-black/70 backdrop-blur-md flex items-center justify-center z-[999]
          transition-opacity duration-300 animate-fadeIn"
    @click.self="closeModal">

    <div
      class="bg-black/40 border border-white/20 rounded-2xl p-6 w-[90%] max-w-4xl h-[70vh] relative
            shadow-2xl backdrop-blur-xl transform transition-all duration-300 animate-zoomIn">

      <!-- Close -->
      <button
        class="absolute top-3 right-3 text-white text-2xl hover:text-emerald-300"
        @click="closeModal">✕</button>

      <!-- Title -->
      <h2 class="text-center text-lg font-semibold mb-2 text-emerald-200">
        {{ modalTitle }}
      </h2>

      <!-- Download -->
      <button
        class="absolute top-3 left-3 text-sm px-3 py-1 rounded-lg 
              bg-emerald-400/20 border border-emerald-300/40 text-emerald-200
              hover:bg-emerald-400/30 transition-all"
        @click="downloadChart">
        ⬇ Download PNG
      </button>

      <canvas ref="modalCanvas" class="w-full h-full"></canvas>

    </div>
  </div>
</template>


<script setup>
import { ref, nextTick, onMounted } from 'vue'
import { Chart, registerables } from 'chart.js'
Chart.register(...registerables)

/* --- PROPS FROM DASHBOARD --- */
const props = defineProps({
  history: Array,
  rainHistory: Array
})

/* --- REFS --- */
const tempCanvas = ref(null)
const humidityCanvas = ref(null)
const rainSensorCanvas = ref(null)

let tempChart = null
let humidityChart = null
let rainSensorChart = null

/* --- MODAL --- */
const showModal = ref(false)
const modalTitle = ref('')
const modalCanvas = ref(null)
let modalChart = null

/* =====================================================
       OPEN MODAL FROM DASHBOARD
   =====================================================*/
const openModal = (title, chartData, labels, borderColor, bgColor) => {
  showModal.value = true
  modalTitle.value = title

  nextTick(() => {
    const ctx = modalCanvas.value.getContext('2d')
    if (modalChart) modalChart.destroy()

    modalChart = new Chart(ctx, {
      type: 'line',
      data: { labels, datasets: [{
        label: title,
        data: chartData,
        tension: 0.3,
        borderWidth: 2,
        pointRadius: 2,
        borderColor,
        backgroundColor: bgColor
      }]},
      options: {
        responsive: true,
        maintainAspectRatio: false,
        plugins: { legend: { labels: { color: '#e5e7eb' }}},
        scales: {
          x: { ticks: { color: '#9ca3af' }},
          y: { ticks: { color: '#9ca3af' }}
        }
      }
    })
  })
}

const closeModal = () => (showModal.value = false)

const downloadChart = () => {
  if (!modalChart) return
  const a = document.createElement('a')
  a.href = modalChart.toBase64Image('image/png', 1.0)
  a.download = `${modalTitle.value.replace(/ /g, '_')}.png`
  a.click()
}


/* =====================================================
                INITIAL CHART SETUP
   =====================================================*/

const initTempChart = () => {
  tempChart = new Chart(tempCanvas.value.getContext('2d'), {
    type: 'line',
    data: {
      labels: props.history.map(p => p.time),
      datasets: [{
        label: 'Temperature (°C)',
        data: props.history.map(p => p.temperature),
        borderColor: 'rgba(16,185,129,1)',
        backgroundColor: 'rgba(16,185,129,0.12)',
        tension: 0.3,
        borderWidth: 2,
        pointRadius: 2
      }]
    },
    options: { responsive: true, maintainAspectRatio: false }
  })
}

const initHumidityChart = () => {
  humidityChart = new Chart(humidityCanvas.value.getContext('2d'), {
    type: 'line',
    data: {
      labels: props.history.map(p => p.time),
      datasets: [{
        label: 'Humidity (%)',
        data: props.history.map(p => p.humidity),
        borderColor: 'rgba(56,189,248,1)',
        backgroundColor: 'rgba(56,189,248,0.12)',
        tension: 0.3,
        borderWidth: 2
      }]
    },
    options: { responsive: true, maintainAspectRatio: false }
  })
}

const initRainChart = () => {
  rainSensorChart = new Chart(rainSensorCanvas.value.getContext('2d'), {
    type: 'line',
    data: {
      labels: props.rainHistory.map(p => p.time),
      datasets: [{
        label: 'Rain Wetness (%)',
        data: props.rainHistory.map(p => p.percent),
        borderColor: 'rgba(96,165,250,1)',
        backgroundColor: 'rgba(96,165,250,0.12)',
        tension: 0.3,
        borderWidth: 2
      }]
    },
    options: { responsive: true, maintainAspectRatio: false }
  })
}

onMounted(() => {
  initTempChart()
  initHumidityChart()
  initRainChart()
})
</script>


<style scoped>
/* same glass-card + animations stay here or parent */
</style>
