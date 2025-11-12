<template>
  <div class="min-h-screen bg-[#0f1218] text-gray-200 p-6">
    <header class="flex items-center justify-between mb-6">
      <h1 class="text-2xl font-bold">
        {{ mode === 'add' ? 'Create New Dashboard' : 'Edit Dashboard' }}
      </h1>
      <button
        class="px-4 py-2 bg-blue-600 hover:bg-blue-500 rounded text-sm font-medium"
        @click="saveDashboard"
      >
        💾 Save
      </button>
    </header>

    <!-- 🧰 Layout Builder Section -->
    <div class="grid grid-cols-3 gap-4">
      <!-- Left: Available Widgets -->
      <div class="p-4 bg-[#1a1f2f] rounded-md border border-white/10">
        <h2 class="text-lg font-semibold mb-2">Available Widgets</h2>
        <div class="space-y-2">
          <div
            v-for="w in widgets"
            :key="w.id"
            class="p-2 rounded bg-[#121826] hover:bg-[#2a3145] cursor-pointer transition"
            @click="addWidget(w)"
          >
            {{ w.name }}
          </div>
        </div>
      </div>

      <!-- Center: Canvas Preview -->
      <div class="col-span-2 p-4 bg-[#101521] rounded-md border border-white/10">
        <h2 class="text-lg font-semibold mb-2">Layout Preview</h2>
        <div class="grid grid-cols-2 gap-4 min-h-[400px] border border-white/10 p-3 rounded-md">
          <div
            v-for="(w, index) in selectedWidgets"
            :key="index"
            class="p-3 bg-[#1a2030] rounded-md relative"
          >
            <div class="text-sm font-semibold mb-1">{{ w.name }}</div>
            <div class="text-xs opacity-70">{{ w.description }}</div>
            <button
              class="absolute top-2 right-2 text-red-400 hover:text-red-500"
              @click="removeWidget(index)"
            >
              ✕
            </button>
          </div>
        </div>
      </div>
    </div>
  </div>
</template>

<script setup>
import { ref, onMounted } from 'vue'
import { useRoute, useRouter } from 'vue-router'

const route = useRoute()
const router = useRouter()
const mode = ref(route.query.mode || 'add')
const dashboardId = ref(route.query.id || null)

const widgets = ref([
  { id: 1, name: 'Visitor Chart', description: 'Shows visitor trends' },
  { id: 2, name: 'Heatmap', description: 'Visualizes occupancy' },
  { id: 3, name: 'Alert Table', description: 'Lists alerts in real time' }
])

const selectedWidgets = ref([])

function addWidget(widget) {
  selectedWidgets.value.push(widget)
}

function removeWidget(index) {
  selectedWidgets.value.splice(index, 1)
}

function saveDashboard() {
  console.log('Saved Dashboard Layout:', selectedWidgets.value)
  router.push('/') // or your dashboard main page
}

onMounted(() => {
  if (mode.value === 'edit' && dashboardId.value) {
    // Load existing dashboard layout here from API or local storage
  }
})
</script>

<style scoped>
/* Optional extra styling */
</style>
