








<template>
  <main class="min-h-screen flex flex-col items-center justify-center bg-gray-900 text-white text-center space-y-6 relative">
    <!-- ===== Header ===== -->
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

    <!-- ===== Title ===== -->
    <h1 class="text-4xl font-bold drop-shadow-lg">ESP32 → Vue Dashboard</h1>

    <!-- ===== Message Output ===== -->
    <p
      class="text-xl font-medium transition-all duration-300"
      :class="loading ? 'opacity-70' : 'opacity-100'"
    >
      {{ message }}
    </p>

    <!-- ===== Button ===== -->
    <button
      @click="getMessage"
      class="px-6 py-3 rounded-lg bg-gradient-to-r from-blue-500 to-sky-400 text-white font-semibold 
             shadow-[0_0_15px_rgba(0,150,255,0.5)] hover:scale-105 
             hover:shadow-[0_0_25px_rgba(0,200,255,0.8)] 
             transition-all duration-300 disabled:opacity-50"
      :disabled="loading"
    >
      {{ loading ? 'Requesting...' : 'Get Hello World' }}
    </button>
  </main>
</template>


<script setup>
import { ref, onMounted } from 'vue'

const message = ref('Click the button to receive message')
const loading = ref(false)
const connected = ref(false)

// Replace with your ESP32 IP address
const esp32IP = 'http://192.168.1.32/hello'

const getMessage = async () => {
  loading.value = true
  message.value = 'Connecting...'

  try {
    const res = await fetch(esp32IP)
    if (!res.ok) throw new Error('HTTP error')
    const text = await res.text()
    message.value = text
    connected.value = true
  } catch (err) {
    message.value = '⚠️ Failed to connect to ESP32'
    connected.value = false
  } finally {
    loading.value = false
  }
}

// Optional: check connection automatically on load
onMounted(() => {
  getMessage()
})
</script>