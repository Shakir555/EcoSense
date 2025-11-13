<template>
  <div class="space-y-4">

    <!-- 🌡 TEMPERATURE CHART -->
    <div class="glass-card card-hover p-5">
      <div class="mb-3 flex items-center justify-between">
        <div>
          <p class="chart-title">Temperature History</p>
          <p class="chart-subtitle">
            Latest {{ history.length }} readings
          </p>
        </div>
        <span class="chart-badge-emerald">°C Celsius</span>
      </div>

      <div class="h-48">
        <canvas ref="tempChartCanvas" class="h-full w-full"></canvas>
      </div>
    </div>

    <!-- 💧 HUMIDITY CHART -->
    <div class="glass-card card-hover p-5">
      <div class="mb-3 flex items-center justify-between">
        <div>
          <p class="chart-title">Humidity History</p>
          <p class="chart-subtitle">
            Latest {{ history.length }} readings
          </p>
        </div>
        <span class="chart-badge-sky">% Relative Humidity</span>
      </div>

      <div class="h-48">
        <canvas ref="humidityChartCanvas" class="h-full w-full"></canvas>
      </div>
    </div>

  </div>
</template>

<script setup>
import { ref, watch, nextTick } from 'vue'
import { Chart, registerables } from 'chart.js'
Chart.register(...registerables)

// PROPS FROM PARENT
const props = defineProps({
  history: {
    type: Array,
    required: true
  }
})

// Canvas refs
const tempChartCanvas = ref(null)
const humidityChartCanvas = ref(null)

let tempChart = null
let humidityChart = null

// 🌡 Initialize Temperature Chart
const initTempChart = () => {
  if (!tempChartCanvas.value) return
  const ctx = tempChartCanvas.value.getContext('2d')

  tempChart = new Chart(ctx, {
    type: 'line',
    data: {
      labels: props.history.map(p => p.time),
      datasets: [
        {
          label: 'Temperature (°C)',
          data: props.history.map(p => p.temperature),
          borderColor: 'rgba(16,185,129,1)',
          backgroundColor: 'rgba(16,185,129,0.12)',
          tension: 0.3,
          borderWidth: 2,
          pointRadius: 2
        }
      ]
    },
    options: commonChartOptions
  })
}

// 💧 Initialize Humidity Chart
const initHumidityChart = () => {
  if (!humidityChartCanvas.value) return
  const ctx = humidityChartCanvas.value.getContext('2d')

  humidityChart = new Chart(ctx, {
    type: 'line',
    data: {
      labels: props.history.map(p => p.time),
      datasets: [
        {
          label: 'Humidity (%)',
          data: props.history.map(p => p.humidity),
          borderColor: 'rgba(56,189,248,1)',
          backgroundColor: 'rgba(56,189,248,0.12)',
          tension: 0.3,
          borderWidth: 2,
          pointRadius: 2
        }
      ]
    },
    options: commonChartOptions
  })
}

// Shared chart style options
const commonChartOptions = {
  responsive: true,
  maintainAspectRatio: false,
  plugins: {
    legend: {
      labels: {
        color: '#e5e7eb',
        font: { size: 11 }
      }
    }
  },
  scales: {
    x: {
      ticks: { color: '#9ca3af' },
      grid: { color: 'rgba(148,163,184,0.2)' }
    },
    y: {
      ticks: { color: '#9ca3af' },
      grid: { color: 'rgba(148,163,184,0.2)' }
    }
  }
}

// Update charts whenever history changes
watch(
  () => props.history,
  async () => {
    await nextTick()
    updateCharts()
  },
  { deep: true }
)

// Chart update function
const updateCharts = () => {
  if (tempChart) {
    tempChart.data.labels = props.history.map(p => p.time)
    tempChart.data.datasets[0].data = props.history.map(p => p.temperature)
    tempChart.update('active')
  }

  if (humidityChart) {
    humidityChart.data.labels = props.history.map(p => p.time)
    humidityChart.data.datasets[0].data = props.history.map(p => p.humidity)
    humidityChart.update('active')
  }
}

// Init charts on mount
nextTick(() => {
  initTempChart()
  initHumidityChart()
})
</script>

<style scoped>
.chart-title {
  @apply text-xs font-semibold uppercase tracking-[0.2em] text-gray-300;
}
.chart-subtitle {
  @apply text-sm text-gray-200;
}
.chart-badge-emerald {
  @apply rounded-full bg-emerald-400/15 px-2 py-1 text-[0.7rem] text-emerald-200;
}
.chart-badge-sky {
  @apply rounded-full bg-sky-400/15 px-2 py-1 text-[0.7rem] text-sky-200;
}
</style>
