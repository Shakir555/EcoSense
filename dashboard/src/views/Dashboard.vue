<template>
  <!-- Main Background -->
    <main
    class="relative min-h-screen flex flex-col items-center justify-center text-white overflow-hidden
            bg-gradient-to-br from-[#001510] via-[#00321d] to-[#00a86b]"
    >
  <!-- 💡 Radial Glow -->
  <div
    class="absolute inset-0 bg-[radial-gradient(circle_at_center,rgba(0,255,150,0.15)_0%,transparent_70%)] pointer-events-none animate-pulse-slow"
  ></div>
    <!-- Header -->
    <div class="flex items-center gap-3 absolute top-8 right-8">
      <div
        class="w-4 h-4 rounded-full shadow-lg transition-all duration-500"
        :class="connected
          ? 'bg-green-500 shadow-[0_0_12px_rgba(16,255,100,0.9)]'
          : 'bg-red-500 shadow-[0_0_12px_rgba(255,80,80,0.9)]'"
      ></div>
      <span class="text-sm font-medium tracking-wide">
        {{ connected ? 'API Connected' : 'API Disconnected' }}
      </span>
    </div>

    <!-- Title -->
    <h1 class="text-4xl font-bold drop-shadow-lg">EcoSense</h1>

    <!-- Live Sensor Data -->
    <div
      class="bg-white/10 backdrop-blur-sm rounded-xl px-8 py-5 mb-8
            shadow-[0_0_15px_rgba(0,255,150,0.3)] text-center space-y-2
            transition-all duration-500 hover:scale-[1.01]"
    >
      <h2 class="text-lg font-semibold text-emerald-300">Live Sensor Data</h2>
      <p class="text-2xl font-bold">
        🌡️ {{ temperature.toFixed(1) }} °C &nbsp;&nbsp; 💧 {{ humidity.toFixed(1) }} %
      </p>
    </div>

    <!-- Data -->
    <p class="text-xl font-medium transition-all duration-300"
      :class="loading ? 'opacity-70' : 'opacity-100'">
      {{ data }}
    </p>

    <!-- Data -->
    <p
      class="text-xl font-medium transition-all duration-300"
      :class="loading ? 'opacity-70' : 'opacity-100'"
    >
      {{ data }}
    </p>

    <!-- Get Data Button -->
    <button
      @click="getData"
      class="px-6 py-3 rounded-lg bg-gradient-to-r from-blue-500 to-sky-400
             text-gray-900 font-semibold
             shadow-[0_0_15px_rgba(0,150,255,0.5)] hover:scale-105
             hover:shadow-[0_0_25px_rgba(0,200,255,0.8)]
             transition-all duration-300 disabled:opacity-50"
      :disabled="loading"
    >
      {{ loading ? 'Requesting data...' : 'Get Hello World' }}
    </button>
  </main>
</template>

<script setup>
import { ref, onMounted } from 'vue'

// ✅ Your ESP32 IP
const ESP32IP = 'http://192.168.1.32/hello'

// Reactive variables
const data = ref('Click the button to receive data')
const loading = ref(false)
const connected = ref(false)

const temperature = ref(0)
const humidity = ref(0)

// Function to fetch ESP32 data
const getData = async () => {
  loading.value = true
  data.value = 'Connecting...'

  try {
    const res = await fetch(ESP32IP)
    if (!res.ok) throw new Error('HTTP Error')

    const text = await res.text()
    data.value = text
    connected.value = true
  } catch (err) {
    data.value = '⚠️ Failed to connect to ESP32'
    connected.value = false
  } finally {
    loading.value = false
  }
}

const fetchSensorData = async () => {
  try {
    const res = await fetch('http://192.168.1.32/data')
    if (!res.ok) throw new Error('HTTP Error')
    const json = await res.json()
    temperature.value = json.temperature
    humidity.value = json.humidity
    connected.value = true
  } catch (err) {
    connected.value = false
  }
}

// Auto run on page load
onMounted(() => {
  getData()
  setInterval(fetchSensorData, 5000)
})
</script>

<style scoped>
.main-bg {
  background-image: 
    linear-gradient(to bottom right, #000000, #0f2f1b, #1b5e20),
    linear-gradient(to top right, rgba(0,255,150,0.3), rgba(50,255,150,0.15));
  background-blend-mode: overlay;
}

@keyframes pulse-slow {
  0%, 100% { opacity: 0.7; }
  50% { opacity: 1; }
}

.animate-pulse-slow {
  animation: pulse-slow 6s ease-in-out infinite;
}
</style>
