<template>
  <main
    class="relative min-h-screen text-white overflow-hidden
           bg-gradient-to-br from-[#000b07] via-[#012418] to-[#038253]"
  >
    <!-- Ambient radial glow -->
    <div
      class="pointer-events-none absolute inset-0
             bg-[radial-gradient(circle_at_center,rgba(0,255,150,0.18)_0%,transparent_70%)]
             animate-pulse-slow opacity-70"
    ></div>

    <!-- TOP BAR -->
    <header
      class="relative z-10 flex items-center justify-between px-10 py-4
            border-b border-white/10 backdrop-blur-xl
            bg-black/10"
    >
      <!-- Brand -->
      <div class="flex items-center gap-3">

        <!-- Logo -->
        <img
          src="/logo/enviro-iot.png"
          alt="logo"
          class="w-20 h-20 object-contain transition-transform duration-300 hover:scale-110"
        />
        <!-- Title -->
        <div class="flex flex-col leading-tight">
          <h1 class="text-2xl font-bold tracking-wide">
            EcoSense
          </h1>
        </div>
      </div>

      <!-- Right side: status + time -->
      <div class="flex flex-col items-end justify-center text-sm leading-tight gap-2">

        <!-- API Status pill (TOP) -->
        <div
          class="flex items-center gap-2 rounded-full px-3 py-1.5
                border border-white/15 bg-white/5 backdrop-blur-xl"
        >
          <span
            class="h-2.5 w-2.5 rounded-full"
            :class="connected
              ? 'bg-emerald-400 shadow-[0_0_10px_rgba(0,255,150,0.9)]'
              : 'bg-red-500 shadow-[0_0_10px_rgba(255,80,80,0.9)]'"
          ></span>

          <span class="font-medium">
            {{ connected ? 'API Connected' : 'API Disconnected' }}
          </span>
        </div>

        <!-- Last sensor update (BOTTOM) -->
        <div class="text-right text-xs text-gray-300/80">
          <p class="uppercase tracking-widest text-[0.65rem]">Last sensor update</p>
          <p class="font-mono">
            {{ lastUpdated || '—' }}
          </p>
        </div>
      </div>
    </header>

    <!-- LAYOUT -->
    <section
      class="relative z-10 mx-auto flex max-w-none gap-6 px-10 pb-10 pt-6 lg:pl-80 top-5"
    >
      SIDEBAR
      <aside
        class="hidden w-60 flex-shrink-0 flex-col gap-4 rounded-2xl
               border border-white/10 bg-black/20 p-4 backdrop-blur-xl
               lg:flex fixed left-0 -ml-30 overflow-y-auto fixed "
      >
        <p class="mb-1 text-xs font-semibold uppercase tracking-[0.18em] text-gray-400">
          Navigation
        </p>

        <button class="sidebar-link-active">
          <span class="text-sm">Dashboard</span>
          <span class="text-[0.65rem] uppercase tracking-widest text-emerald-300/80">Live</span>
        </button>
      </aside>

      <!-- MAIN CONTENT -->
      <div class="flex flex-1 flex-col items-center justify-center space-y-6">
        <!-- TOP CARDS -->
        <div class="grid gap-4 md:grid-cols-2 xl:grid-cols-4 w-fit mx-auto justify-center">
          <!-- Temperature -->
          <div class="glass-card card-hover p-10 px-4 py-6 justify-center">
            <div class="flex items-center justify-between">
              <p class="text-xs font-semibold uppercase tracking-[0.2em] text-gray-300">Temperature</p>
              <span class="rounded-full bg-emerald-400/10 px-4 py-1 text-[0.7rem] text-emerald-200">°C</span>
            </div>
            <p class="mt-3 text-3xl font-semibold">
              {{ temperature.toFixed(1) }}
              <span class="text-base text-gray-300">°C</span>
            </p>
            <p class="mt-1 text-xs text-gray-300/80">Indoor ambient reading from DHT11 sensor</p>
          </div>

          <!-- Humidity -->
          <div class="glass-card card-hover p-4 px-4 py-6">
            <div class="flex items-center justify-between">
              <p class="text-xs font-semibold uppercase tracking-[0.2em] text-gray-300">Humidity</p>
              <span class="rounded-full bg-sky-400/15 px-2 py-0.5 text-[0.7rem] text-sky-200">%</span>
            </div>
            <p class="mt-3 text-3xl font-semibold">
              {{ humidity.toFixed(1) }}
              <span class="text-base text-gray-300">%</span>
            </p>
            <p class="mt-1 text-xs text-gray-300/80">Relative humidity based on latest sensor sample</p>
          </div>
        </div>

        <!-- TWO SEPARATE CHARTS -->
        <div class="grid gap-4 lg:grid-cols-2">
          
          <!-- Temperature Chart -->
          <div class="glass-card card-hover p-5">
            <div class="mb-3">
              <p class="text-xs font-semibold uppercase tracking-[0.2em] text-gray-300">
                Temperature
              </p>
            </div>
            <div class="h-80">
              <canvas ref="tempCanvas"></canvas>
            </div>
          </div>

          <!-- Humidity Chart -->
          <div class="glass-card card-hover p-5">
            <div class="mb-3">
              <p class="text-xs font-semibold uppercase tracking-[0.2em] text-gray-300">
                Humidity
              </p>
            </div>
            <div class="h-80">
              <canvas ref="humidityCanvas"></canvas>
            </div>
          </div>
        </div>
      </div>
    </section>
  </main>
</template>

<script setup>
import { ref, onMounted, nextTick } from 'vue'
import { Chart, registerables } from 'chart.js'
Chart.register(...registerables)

// ESP32 endpoints
const HELLO_URL = 'http://192.168.1.32/hello'
const DATA_URL = 'http://192.168.1.32/data'

// State
const data = ref('Click the button to receive data')
const loading = ref(false)
const connected = ref(false)

const temperature = ref(0)
const humidity = ref(0)
const lastUpdated = ref('')

const history = ref([])

const tempCanvas = ref(null)
const humidityCanvas = ref(null)

let tempChart = null
let humidityChart = null

const updateLastUpdated = () => {
  lastUpdated.value = new Date().toLocaleTimeString()
}

// GET /hello
const getData = async () => {
  loading.value = true
  data.value = 'Connecting...'

  try {
    const res = await fetch(HELLO_URL)
    if (!res.ok) throw new Error('HTTP Error')

    const text = await res.text()
    data.value = text
    connected.value = true
  } catch (e) {
    data.value = '⚠️ Failed to connect to ESP32'
    connected.value = false
  } finally {
    loading.value = false
  }
}

// GET /data
const fetchSensorData = async () => {
  try {
    const res = await fetch(DATA_URL)
    if (!res.ok) throw new Error('HTTP Error')

    const json = await res.json()

    if (typeof json.temperature === 'number' && typeof json.humidity === 'number') {
      temperature.value = json.temperature
      humidity.value = json.humidity
      connected.value = true
      updateLastUpdated()

      history.value.push({
        time: new Date().toLocaleTimeString(),
        temperature: json.temperature,
        humidity: json.humidity
      })

      if (history.value.length > 20) history.value.shift()

      updateCharts()
    }
  } catch (e) {
    connected.value = false
  }
}

// Temperature chart
const initTempChart = () => {
  const ctx = tempCanvas.value.getContext('2d')
  tempChart = new Chart(ctx, {
    type: 'line',
    data: {
      labels: history.value.map(p => p.time),
      datasets: [{
        label: 'Temperature (°C)',
        data: history.value.map(p => p.temperature),
        tension: 0.3,
        borderWidth: 2,
        pointRadius: 2,
        borderColor: 'rgba(16,185,129,1)',
        backgroundColor: 'rgba(16,185,129,0.12)'
      }]
    },
    options: {
      responsive: true,
      maintainAspectRatio: false,
      plugins: { legend: { labels: { color: '#e5e7eb' } } },
      scales: {
        x: { ticks: { color: '#9ca3af' }, grid: { color: 'rgba(148,163,184,0.15)' } },
        y: { ticks: { color: '#9ca3af' }, grid: { color: 'rgba(148,163,184,0.18)' } }
      }
    }
  })
}

// Humidity chart
const initHumidityChart = () => {
  const ctx = humidityCanvas.value.getContext('2d')
  humidityChart = new Chart(ctx, {
    type: 'line',
    data: {
      labels: history.value.map(p => p.time),
      datasets: [{
        label: 'Humidity (%)',
        data: history.value.map(p => p.humidity),
        tension: 0.3,
        borderWidth: 2,
        pointRadius: 2,
        borderColor: 'rgba(56,189,248,1)',
        backgroundColor: 'rgba(56,189,248,0.12)'
      }]
    },
    options: {
      responsive: true,
      maintainAspectRatio: false,
      plugins: { legend: { labels: { color: '#e5e7eb' } } },
      scales: {
        x: { ticks: { color: '#9ca3af' }, grid: { color: 'rgba(148,163,184,0.15)' } },
        y: { ticks: { color: '#9ca3af' }, grid: { color: 'rgba(148,163,184,0.18)' } }
      }
    }
  })
}

const updateCharts = () => {
  if (tempChart) {
    tempChart.data.labels = history.value.map(p => p.time)
    tempChart.data.datasets[0].data = history.value.map(p => p.temperature)
    tempChart.update('active')
  }

  if (humidityChart) {
    humidityChart.data.labels = history.value.map(p => p.time)
    humidityChart.data.datasets[0].data = history.value.map(p => p.humidity)
    humidityChart.update('active')
  }
}

onMounted(async () => {
  getData()
  fetchSensorData()
  setInterval(fetchSensorData, 5000)

  await nextTick()
  initTempChart()
  initHumidityChart()
})
</script>

<style scoped>
.glass-card {
  background: radial-gradient(circle at top left, rgba(16, 185, 129, 0.14), transparent 55%),
    rgba(15, 23, 42, 0.72);
  border-radius: 18px;
  border: 1px solid rgba(148, 163, 184, 0.35);
  backdrop-filter: blur(16px);
}

.card-hover {
  transition: transform 0.28s ease, box-shadow 0.28s ease, border-color 0.28s ease;
}
.card-hover:hover {
  transform: translateY(-2px);
  box-shadow: 0 18px 40px rgba(15, 23, 42, 0.7);
  border-color: rgba(45, 212, 191, 0.65);
}

.sidebar-link,
.sidebar-link-active {  
  display: flex;
  flex-direction: column;
  align-items: flex-start;
  gap: 2px;
  border-radius: 0.9rem;
  padding: 0.6rem 0.8rem;
  border: 1px solid rgba(148, 163, 184, 0.25);
  background: rgba(15, 23, 42, 0.72);
  font-size: 0.8rem;
  text-align: left;
  transition: all 0.25s ease;
}

.sidebar-link:hover {
  border-color: rgba(45, 212, 191, 0.8);
  background: rgba(15, 23, 42, 0.9);
  box-shadow: 0 0 16px rgba(34, 197, 94, 0.35);
}

.sidebar-link-active {
  border-color: rgba(45, 212, 191, 0.9);
  background: radial-gradient(circle at top left, rgba(45, 212, 191, 0.3), rgba(15, 23, 42, 0.95));
  box-shadow: 0 0 22px rgba(16, 185, 129, 0.85);
}

@keyframes pulse-slow {
  0%, 100% { opacity: 0.6; }
  50% { opacity: 1; }
}
.animate-pulse-slow {
  animation: pulse-slow 7s ease-in-out infinite;
}
</style>
