<template>
  <div class="flex flex-col h-screen bg-[#0f1218] text-gray-200">
    <!-- ===== Top Bar ===== -->
    <header class="h-16 flex items-center px-6 border-b border-white/10 bg-[#111520] relative flex-shrink-0">
      <div class="flex items-center gap-3">
        <img src="/logo/dataLibrary.png" alt="logo" class="h-[120px] w-[120px] object-contain" />
      </div>

      <div class="ml-auto flex items-center gap-6 text-sm relative">
        <!-- Admin -->
        <div class="relative group">
          <div
            class="admin-container flex items-center gap-2 cursor-pointer px-3 py-1 rounded-full transition-all duration-300"
            @click="toggleLogout"
          >
            <!-- ✅ Ultra-thin ring + slightly stronger glow -->
            <div
              class="admin-ring w-10 h-10 rounded-full flex items-center justify-center border border-white transition-all duration-300 group-hover:ring-[0.8px] group-hover:ring-white/90 group-hover:shadow-[0_0_12px_rgba(255,255,255,0.55)]"
            >
              <img src="/icon/admin.png" class="w-5 h-5 object-contain icon-invert" />
            </div>
            <span class="opacity-80 text-sm font-medium">Administrator</span>
          </div>

          <div
            v-if="showLogout"
            class="absolute right-0 mt-2 w-32 bg-[#1a1f2f] border border-white/10 rounded-md shadow-lg z-50"
          >
            <button @click="logout" class="w-full text-left px-4 py-2 text-sm hover:bg-white/10">Log Out</button>
          </div>
        </div>

        <!-- Toolbar -->
        <div class="flex items-center gap-4">
          <button class="icon-btn"><img src="/icon/notification.png" class="w-5 h-5 icon-invert" /></button>
          <button class="icon-btn"><img src="/icon/alerts.png" class="w-5 h-5 icon-invert" /></button>
          <button class="icon-btn"><img src="/icon/application.png" class="w-5 h-5 icon-invert" /></button>

          <!-- Hide/Unhide Titles Button -->
          <button class="arrow-btn" @click="showTitles = !showTitles" :title="showTitles ? 'Hide titles' : 'Show titles'">
            <img src="/icon/arrow-down.png" class="w-5 h-5 icon-invert transition-transform duration-300" :class="{ 'rotate-180': showTitles }" />
          </button>

          <button class="icon-btn" @click="enterFullscreen">
            <img src="/icon/maximize.png" class="w-5 h-5 icon-invert" />
          </button>
        </div>
      </div>
    </header>

    <!-- ===== Navigation Bar ===== -->
    <nav
      v-if="showTitles"
      class="h-12 flex items-center justify-center bg-[#121725] border-b border-white/10 px-6 space-x-8 text-sm flex-shrink-0"
    >
      <button
        v-for="s in sections"
        :key="s"
        @click="activeSection = s"
        class="px-2 py-1 transition"
        :class="activeSection === s 
          ? 'text-blue-400 border-b-2 border-blue-400 font-semibold' 
          : 'hover:text-blue-300 text-gray-300'"
      >
        {{ s }}
      </button>
    </nav>

    <!-- ✨ Thinner White Glow Line When Hidden -->
    <div v-else class="h-[3px] relative w-full overflow-hidden bg-transparent">
      <div class="glow-line"></div>
    </div>

    <!-- ===== Scrollable Content ===== -->
    <div class="flex-1 overflow-y-auto scroll-smooth transition-all duration-300 px-2 pb-6">
      <CustomizeDashboard v-if="activeSection === 'Dashboard'" />
      <div v-else class="h-full flex items-center justify-center text-gray-500 text-sm py-10">
        {{ activeSection }} page content here...
      </div>
    </div>
  </div>
</template>

<script setup>
import { ref } from 'vue'
import { useRouter } from 'vue-router'
import CustomizeDashboard from './CustomizeDashboard.vue'

const router = useRouter()
const sections = ['Dashboard', 'Record', 'Configuration', 'System', 'Operations']
const activeSection = ref('Dashboard')
const showTitles = ref(true)
const showLogout = ref(false)

function toggleLogout() { showLogout.value = !showLogout.value }
function logout() { router.push('/login-user') }
async function enterFullscreen() { if (!document.fullscreenElement) await document.documentElement.requestFullscreen() }
</script>

<style scoped>
.icon-btn { @apply w-8 h-8 flex items-center justify-center rounded-full hover:bg-white/10 transition; }
.icon-invert { filter: invert(1) brightness(2); }
.arrow-btn { @apply w-9 h-9 flex items-center justify-center rounded-full transition; background: rgba(255,255,255,0.05); }
.arrow-btn:hover { background: rgba(255,255,255,0.15); }
.arrow-btn img { transition: transform 0.3s ease; }

/* ✨ Thin glowing white line when nav is hidden */
.glow-line {
  position: absolute;
  width: 200%;
  height: 2px;
  left: -100%;
  top: 0;
  background: linear-gradient(
    90deg,
    transparent,
    rgba(255, 255, 255, 0.75),
    transparent
  );
  box-shadow: 0 0 12px rgba(255, 255, 255, 0.45);
  animation: glowTravel 3s ease-in-out infinite;
}

@keyframes glowTravel {
  0% { left: -100%; opacity: 0.25; }
  25% { left: 20%; opacity: 0.9; }
  50% { left: 60%; opacity: 0.7; }
  75% { left: 20%; opacity: 0.9; }
  100% { left: -100%; opacity: 0.25; }
}

/* Remove fade transitions */
nav,
.glow-line {
  transition: none !important;
}
</style>

<style>
/* 🌐 Modern Scrollbar Styling */
::-webkit-scrollbar {
  width: 8px;
  height: 8px;
}

::-webkit-scrollbar-track {
  background: transparent;
}

::-webkit-scrollbar-thumb {
  background: linear-gradient(180deg, rgba(59,130,246,0.6), rgba(59,130,246,0.3));
  border-radius: 10px;
  box-shadow: 0 0 8px rgba(59,130,246,0.4);
  transition: background 0.3s ease, box-shadow 0.3s ease;
}

::-webkit-scrollbar-thumb:hover {
  background: linear-gradient(180deg, rgba(59,130,246,0.9), rgba(59,130,246,0.5));
  box-shadow: 0 0 12px rgba(59,130,246,0.6);
}

::-webkit-scrollbar-corner {
  background: transparent;
}

* {
  scrollbar-width: thin;
  scrollbar-color: rgba(59,130,246,0.6) transparent;
}

html {
  scroll-behavior: smooth;
}
</style>
