<template>
  <main
    class="min-h-screen text-white p-6 overflow-visible
    [background-image:linear-gradient(to_bottom_right,#0a1b3f,#0e121b,#3b0a0a),linear-gradient(to_top_right,rgba(0,102,255,0.4),rgba(255,50,50,0.15))]
    bg-blend-overlay"
  >
    <!-- ===== Header ===== -->
    <header class="flex justify-between items-center mb-6">
      <div class="flex items-center gap-3 relative">
        <div class="absolute inset-0 flex justify-center items-center">
          <div class="absolute w-40 h-40 rounded-full bg-blue-500/10 blur-2xl"></div>
        </div>
          <div class="flex items-center gap-4 group">
            <!-- 🧠 DataLibrary Logo -->
            <img
              src="/logo/dataLibrary.png"
              alt="DataLibrary Logo"
              class="object-contain logo-glow relative p-1 z-12 w-30 h-20 transition-all duration-300 group-hover:scale-110"
            />

            <!-- ✨ Thin Glowing Divider -->
            <div
              class="w-[2.0px] h-10 mx-2 rounded-full 
                    bg-gradient-to-b from-blue-400 via-sky-300 to-rose-400 
                    shadow-[0_0_14px_rgba(255,90,120,0.8)] 
                    transition-all duration-500 
                    group-hover:shadow-[0_0_22px_rgba(255,110,140,1)] 
                    animate-pulse"
            ></div>

            <!-- 🚀 Astro Logo (Rounded, No Border/Glow) -->
            <img
              src="/logo/astro_logo.png"
              alt="Astro Logo"
              class="object-contain relative p-1 z-12 w-20 h-16 rounded-2xl 
                    transition-all duration-300 group-hover:scale-110"
            />
          </div>
      </div>

      <!-- ===== Connection & Page Setting ===== -->
      <div class="flex flex-col items-end gap-1">
        <div
          :class="[
            'px-3 py-1 rounded-md text-sm font-medium shadow-md status-box',
            connected ? 'bg-green-600' : 'bg-red-600'
          ]"
        >
          {{ connected ? '🟢 Connected' : '🔴 Offline' }}
        </div>

        <!-- ✅ Items Per Page Small Setting -->
        <div class="flex items-center gap-2 text-l font-large text-gray-300 mt-1">
          <span>Items per page:</span>
          <button
            @click="setItemsPerPage(20)"
            :class="[
              'p-4 px-2 py-[2px] rounded-md transition-all',
              itemsPerPage === 20
                ? 'bg-blue-600 text-white shadow-[0_0_8px_rgba(37,99,235,0.6)]'
                : 'bg-gray-700 hover:bg-gray-600 text-gray-300'
            ]"
          >
            20
          </button>
          <button
            @click="setItemsPerPage(40)"
            :class="[
              'px-2 py-[2px] rounded-md transition-all',
              itemsPerPage === 40
                ? 'bg-blue-600 text-white shadow-[0_0_8px_rgba(37,99,235,0.6)]'
                : 'bg-gray-700 hover:bg-gray-600 text-gray-300'
            ]"
          >
            40
          </button>
          <button
            @click="setItemsPerPage(60)"
            :class="[
              'px-2 py-[2px] rounded-md transition-all',
              itemsPerPage === 60
                ? 'bg-blue-600 text-white shadow-[0_0_8px_rgba(37,99,235,0.6)]'
                : 'bg-gray-700 hover:bg-gray-600 text-gray-300'
            ]"
          >
            60
          </button>
        </div>
      </div>
    </header>

    <!-- ===== Tabs ===== -->
    <div class="flex justify-center mb-6 -mt-16 space-x-8">
      <!-- 🔴 Alarm Event -->
      <button
        @click="activeTab = 'alarm'"
        :class="[ 
          'flex flex-col items-center justify-center w-40 h-32 rounded-2xl border font-semibold text-lg text-center transition-all duration-300 ease-out transform shadow-lg select-none relative overflow-hidden backdrop-blur-md',
          activeTab === 'alarm'
            ? 'bg-[rgba(255,90,90,0.15)] border-[rgba(255,150,150,0.3)] text-white animate-breath-glass-red'
            : 'bg-[rgba(255,255,255,0.05)] border-[rgba(255,255,255,0.1)] text-gray-200 hover:scale-105 hover:shadow-[0_0_25px_rgba(255,90,90,0.4)] hover:text-white'
        ]"
      >
        <img
          src="/icon/general_alert.png"
          alt="Alarm Icon"
          class="w-12 h-12 mb-3 object-contain drop-shadow-[0_0_12px_rgba(255,120,120,0.8)] transition-transform duration-300"
        />
        Alarm Event
      </button>

      <!-- 🔵 Condition Search -->
      <button
        @click="activeTab = 'condition'"
        :class="[ 
          'flex flex-col items-center justify-center w-40 h-32 rounded-2xl border font-semibold text-lg text-center transition-all duration-300 ease-out transform shadow-lg select-none relative overflow-hidden backdrop-blur-md',
          activeTab === 'condition'
            ? 'bg-[rgba(80,140,255,0.15)] border-[rgba(160,200,255,0.3)] text-white'  // ✅ removed animate-breath-glass-blue
            : 'bg-[rgba(255,255,255,0.05)] border-[rgba(255,255,255,0.1)] text-gray-200 hover:scale-105 hover:shadow-[0_0_25px_rgba(80,140,255,0.4)] hover:text-white'
        ]"
      >
        <img
          src="/icon/obj_search.png"
          alt="Search Icon"
          class="w-12 h-12 mb-3 object-contain drop-shadow-[0_0_12px_rgba(120,180,255,0.8)] transition-transform duration-300"
        />
        Condition Search
      </button>
    </div>


    <!-- ===== ALARM EVENT TAB ===== -->
    <section v-if="activeTab === 'alarm'">
      <div v-if="loading" class="text-gray-400 text-center mb-4 animate-pulse">
        Fetching alarm events...
      </div>
      <div v-if="error" class="text-red-400 text-center mb-4">{{ error }}</div>

      <div
        v-if="events.length"
        class="video-wall grid gap-4 sm:grid-cols-2 md:grid-cols-3 lg:grid-cols-4"
      >
        <div
          v-for="(ev, i) in events.slice((currentPage - 1) * itemsPerPage, currentPage * itemsPerPage)"
          :key="i"
          class="relative bg-[#1b1f2d] border border-gray-700 rounded-xl overflow-hidden shadow-lg transition-transform cursor-pointer group card-glow"
          @click="openPreview(ev.fullImageUri, ev)"
        >
          <div class="relative w-full aspect-video bg-black flex items-center justify-center overflow-hidden">
            <img
              v-if="ev.fullImageUri"
              :src="ev.fullImageUri"
              class="w-full h-auto object-contain transition-transform duration-500 ease-out group-hover:scale-105"
              @error="hideOnError"
              @load="() => console.log('✅ Loaded:', ev.fullImageUri)"
              @click="() => console.log('🔍 URL:', ev.fullImageUri)"
            />
            <div
              v-else
              class="w-full h-full flex items-center justify-center bg-gray-800 text-gray-500 text-sm"
            >
              No Image
            </div>
          </div>

          <div
            class="absolute top-2 left-2 bg-red-600 text-white text-[11px] px-2 py-[2px] rounded-md font-semibold uppercase shadow-md"
          >
            {{ ev.alarmMinor || 'ALARM' }}
          </div>

          <div
            class="absolute bottom-0 left-0 w-full bg-gradient-to-t from-black/80 via-black/40 to-transparent px-3 py-2 text-[12px]"
          >
            <div class="font-semibold">{{ ev.channelName || 'Unknown' }}</div>
            <div class="text-gray-300 text-[11px]">{{ formatTime(ev.timeMs) }}</div>
          </div>
        </div>
      </div>

      <!-- Pagination -->
      <div v-if="events.length" class="flex justify-center items-center gap-6 mt-10">
        <!-- ⬅️ Prev Button -->
        <button
          @click="currentPage = Math.max(1, currentPage - 1)"
          class="w-14 h-14 flex items-center justify-center rounded-xl bg-[rgba(255,255,255,0.05)] border border-[rgba(255,255,255,0.1)] 
                transition-all duration-300 ease-out hover:scale-110 hover:shadow-[0_0_20px_rgba(120,180,255,0.5)]"
        >
          <img
            src="/icon/arrow-up.png"
            alt="Previous Page"
            class="w-7 h-7 rotate-[270deg] drop-shadow-[0_0_8px_rgba(120,180,255,0.8)] invert"
          />
        </button>

        <!-- Page Number -->
        <span class="text-gray-400 text-lg font-semibold select-none">
          {{ currentPage }} / {{ Math.ceil(events.length / itemsPerPage) || 1 }}
        </span>

        <!-- ➡️ Next Button -->
        <button
          @click="currentPage = Math.min(Math.ceil(events.length / itemsPerPage), currentPage + 1)"
          class="w-14 h-14 flex items-center justify-center rounded-xl bg-[rgba(255,255,255,0.05)] border border-[rgba(255,255,255,0.1)] 
                transition-all duration-300 ease-out hover:scale-110 hover:shadow-[0_0_20px_rgba(120,180,255,0.5)]"
        >
          <img
            src="/icon/arrow-up.png"
            alt="Next Page"
            class="w-7 h-7 rotate-90 drop-shadow-[0_0_8px_rgba(120,180,255,0.8)] invert"
          />
        </button>
      </div>
    </section>

    <!-- ===== CONDITION SEARCH TAB ===== -->
    <section v-if="activeTab === 'condition'" class="space-y-6">

      <!-- Filter Bar -->
      <div
        class="filter-bar bg-[#1b1f2d]/80 border border-white/10 rounded-xl 
        p-2 shadow-lg backdrop-blur-md flex flex-nowrap items-center gap-3 text-xs
        whitespace-nowrap overflow-x-auto card-glow"
      >

        <!-- Time Range -->
        <div class="flex items-center gap-1">
          <label class="text-gray-400 text-xs whitespace-nowrap">Time</label>

          <input
            type="datetime-local"
            v-model="startTime"
            class="input-base w-28 h-7 text-xs rounded-md transition-all duration-300 hover:scale-105"
          />

          <span class="text-gray-400">-</span>

          <input
            type="datetime-local"
            v-model="endTime"
            class="input-base w-28 h-7 text-xs rounded-md transition-all duration-300 hover:scale-105"
          />
        </div>

        <!-- Camera Name -->
        <div class="flex items-center gap-1 custom-multiselect">
          <label class="text-gray-300 text-xs whitespace-nowrap">Camera</label>
          <multiselect
            v-model="selectedCameras"
            :options="cameraList"
            :multiple="true"
            placeholder="Select"
            class="w-56 h-7 text-black rounded-md"
          />
        </div>

        <!-- Alarm Type -->
        <div class="flex items-center gap-1 custom-multiselect">
          <label class="text-gray-300 text-xs whitespace-nowrap">Type</label>
          <multiselect
            v-model="selectedAlarmTypes"
            :options="alarmTypeList"
            :multiple="true"
            placeholder="Select"
            class="w-52 h-5 text-black rounded-md"
          />
        </div>

        <!-- Alarm Rule -->
        <div class="flex items-center gap-1 custom-multiselect">
          <label class="text-gray-300 text-xs whitespace-nowrap">Rule</label>
          <multiselect
            v-model="selectedAlarmRules"
            :options="alarmRuleList"
            :multiple="true"
            placeholder="Select"
            class="w-36 text-black rounded-md"
          />
        </div>

        <!-- Buttons -->
        <div class="flex items-center gap-8 ml-auto">
          <!-- 🔍 Search -->
          <button
            @click="searchByChannel"
            class="p-2 rounded-lg bg-white/10 shadow-[0_0_10px_rgba(255,255,255,0.5)] transition-all duration-300 hover:scale-110"
          >
            <img src="/icon/obj_search.png" class="w-7 h-7 invert" />
          </button>

          <!-- 🤖 LM -->
          <button
            @click="goToLargeModel"
            class="p-2 rounded-lg bg-white/10 shadow-[0_0_10px_rgba(255,255,255,0.5)] transition-all duration-300 hover:scale-110"
          >
            <img src="/icon/ai.png" class="w-7 h-7 invert" />
          </button>
        </div>
      </div>

      <!-- Results -->
      <div
        v-if="conditionResults.length"
        class="video-wall grid gap-4 sm:grid-cols-2 md:grid-cols-3 lg:grid-cols-4"
      >
        <div
          v-for="(ev, i) in paginatedResults"
          :key="i"
          class="relative bg-[#1b1f2d] border border-gray-700 rounded-xl overflow-hidden shadow-lg transition-transform cursor-pointer group card-glow"
          @click="openPreview(ev.fullImageUri, ev)"
        >
          <div class="relative w-full aspect-video bg-black flex items-center justify-center overflow-hidden">
            <img
              v-if="ev.fullImageUri"
              :src="ev.fullImageUri"
              class="w-full h-auto object-contain transition-transform duration-500 ease-out group-hover:scale-105"
              @error="hideOnError"
            />
            <div
              v-else
              class="w-full h-full flex items-center justify-center bg-gray-800 text-gray-500 text-sm"
            >
              No Image
            </div>
          </div>

          <div
            class="absolute top-2 left-2 bg-blue-600 text-white text-[11px] px-2 py-[2px] rounded-md font-semibold uppercase shadow-md"
          >
            {{ ev.alarmMinor || 'ALARM' }}
          </div>

          <div
            class="absolute bottom-0 left-0 w-full bg-gradient-to-t from-black/80 via-black/40 to-transparent px-3 py-2 text-[12px]"
          >
            <div class="font-semibold">{{ ev.channelName || 'Unknown' }}</div>
            <div class="text-gray-300 text-[11px]">{{ formatTime(ev.timeMs) }}</div>
          </div>
        </div>
      </div>


      <!-- Pagination -->
      <div
        v-if="conditionResults.length"
        class="flex justify-center items-center gap-6 mt-6"
      >
        <!-- ⬅️ Prev Button -->
        <button
          @click="currentPage = Math.max(1, currentPage - 1)"
          class="w-12 h-12 flex items-center justify-center rounded-xl bg-[rgba(255,255,255,0.05)] border border-[rgba(255,255,255,0.1)]
                transition-all duration-300 ease-out hover:scale-110 hover:shadow-[0_0_20px_rgba(120,180,255,0.5)]"
        >
          <img
            src="/icon/arrow-up.png"
            alt="Previous Page"
            class="w-7 h-7 rotate-[270deg] drop-shadow-[0_0_8px_rgba(120,180,255,0.8)] invert"
          />
        </button>

        <!-- Page Indicator -->
        <span class="text-gray-400 text-lg font-semibold select-none">
           {{ currentPage }} / {{ Math.ceil(conditionResults.length / itemsPerPage) || 1 }}
        </span>

        <!-- ➡️ Next Button -->
        <button
          @click="currentPage = Math.min(Math.ceil(conditionResults.length / itemsPerPage), currentPage + 1)"
          class="w-12 h-12 flex items-center justify-center rounded-xl bg-[rgba(255,255,255,0.05)] border border-[rgba(255,255,255,0.1)]
                transition-all duration-300 ease-out hover:scale-110 hover:shadow-[0_0_20px_rgba(120,180,255,0.5)]"
        >
          <img
            src="/icon/arrow-up.png"
            alt="Next Page"
            class="w-7 h-7 rotate-90 drop-shadow-[0_0_8px_rgba(120,180,255,0.8)] invert"
          />
        </button>
      </div>
    </section>

    <!-- ===== LARGE MODEL SEARCH TAB ===== -->
    <section v-if="activeTab === 'largeModel'" class="space-y-6">
      <!-- Filter Bar -->
      <div
        class="filter-bar bg-[#1b1f2d]/80 border border-white/10 rounded-xl p-8 shadow-lg backdrop-blur-md flex flex-col sm:flex-row sm:items-center sm:justify-between gap-6 text-sm card-glow"
      >
        <!-- 🔍 Keyword / Global Search -->
        <div class="w-full flex justify-center">
          <div class="flex items-center gap-6 max-w-[1000px] w-full justify-center">
            <!-- Label -->
            <label class="text-gray-400 text-xl font-bold whitespace-nowrap">LM Search</label>

            <!-- Input -->
            <input
              v-model="lmQuery"
              @keyup.enter="searchLargeModel"
              type="text"
              placeholder="Type to search camera name, alarm type or alarm rule..."
              class="input-base flex-1 min-w-[600px] max-w-[800px] text-lg font-medium"
            />

            <!-- Search Button -->
            <button
              @click="searchLargeModel"
              class="ml-4 px-6 py-3 bg-[#1e1e2f] rounded-md 
                    shadow-[0_0_15px_rgba(255,255,255,0.9)] 
                    transition-all duration-300 transform hover:scale-110 
                    flex items-center justify-center"
              title="Search"
            >
              <img
                src="/icon/algo.png"
                alt="Search Icon"
                class="w-9 h-9 object-contain invert drop-shadow-[0_0_8px_rgba(255,255,255,0.9)] 
                      contrast-125 brightness-125 ml-[2px]"
              />
            </button>
          </div>
        </div>
      </div>

      <!-- ===== LLM Results ===== -->
      <div
        v-if="largeModelResults.length"
        class="video-wall grid gap-4 sm:grid-cols-2 md:grid-cols-3 lg:grid-cols-4"
      >
        <div
          v-for="(ev, i) in paginatedLargeModel"
          :key="i"
          class="relative bg-[#1b1f2d] border border-gray-700 rounded-xl overflow-hidden shadow-lg transition-transform cursor-pointer group card-glow"
          @click="openPreview(ev.fullImageUri, ev)"
        >
          <!-- Image -->
          <img
            v-if="ev.fullImageUri"
            :src="ev.fullImageUri"
            class="w-full h-auto object-contain transition-transform duration-500 ease-out group-hover:scale-105"
            @error="hideOnError"
          />
          <div
            v-else
            class="w-full h-52 flex items-center justify-center bg-gray-800 text-gray-500 text-sm"
          >
            No Image
          </div>

          <!-- Label -->
          <div
            class="absolute top-2 left-2 bg-purple-600 text-white text-[11px] px-2 py-[2px] rounded-md font-semibold uppercase shadow-md"
          >
            {{ ev.alarmMinor || 'LLM Result' }}
          </div>

          <!-- Info Overlay -->
          <div
            class="absolute bottom-0 left-0 w-full bg-gradient-to-t from-black/80 via-black/40 to-transparent px-3 py-2 text-[12px]"
          >
            <div class="font-semibold">{{ ev.channelName || 'Unknown' }}</div>
            <div class="text-gray-300 text-[11px]">{{ formatTime(ev.timeMs) }}</div>
          </div>
        </div>
      </div>

      <!-- Pagination -->
      <div
        v-if="largeModelResults.length"
        class="flex justify-center items-center gap-6 mt-10"
      >
        <!-- ⬅️ Prev Button -->
        <button
          @click="currentPage = Math.max(1, currentPage - 1)"
          class="w-14 h-14 flex items-center justify-center rounded-xl 
                bg-[rgba(255,255,255,0.05)] border border-[rgba(255,255,255,0.1)] 
                transition-all duration-300 ease-out hover:scale-110 
                hover:shadow-[0_0_20px_rgba(120,180,255,0.5)]"
        >
          <img
            src="/icon/arrow-up.png"
            alt="Previous Page"
            class="w-7 h-7 rotate-[270deg] drop-shadow-[0_0_8px_rgba(120,180,255,0.8)] invert"
          />
        </button>

        <!-- Page Number -->
        <span class="text-gray-400 text-lg font-semibold select-none">
          {{ currentPage }} / {{ Math.ceil(largeModelResults.length / itemsPerPage) || 1 }}
        </span>

        <!-- ➡️ Next Button -->
        <button
          @click="currentPage = Math.min(Math.ceil(largeModelResults.length / itemsPerPage), currentPage + 1)"
          class="w-14 h-14 flex items-center justify-center rounded-xl 
                bg-[rgba(255,255,255,0.05)] border border-[rgba(255,255,255,0.1)] 
                transition-all duration-300 ease-out hover:scale-110 
                hover:shadow-[0_0_20px_rgba(120,180,255,0.5)]"
        >
          <img
            src="/icon/arrow-up.png"
            alt="Next Page"
            class="w-7 h-7 rotate-90 drop-shadow-[0_0_8px_rgba(120,180,255,0.8)] invert"
          />
        </button>
      </div>


      <!-- Empty State -->
      <div v-if="!loading && !largeModelResults.length && !error" class="text-gray-400 text-center mt-10">
        Type something and press <strong>Enter</strong> to start global search.
      </div>

      <!-- Error Message -->
      <div v-if="error" class="text-red-400 text-center mt-6">{{ error }}</div>
    </section>


    <!-- ===== Image Preview ===== -->
    <div
      v-if="previewImg"
      class="fixed inset-0 bg-black/80 flex items-center justify-center z-[999999] backdrop-blur-md"
      @click="previewImg = null"
    > 
      <div class="relative max-h-[85vh] max-w-[90vw]">
        <!-- Full Image (no glow) -->
        <img
          :src="previewImg"
          class="max-h-[85vh] max-w-[90vw] rounded-xl border border-blue-500/30 object-contain"
        />

        <!-- ✅ Black Tag Overlay (bottom-left) -->
        <div
          class="absolute bottom-4 left-4 bg-black/85 text-white px-4 py-3 rounded-md 
                flex flex-col gap-[2px] leading-tight font-[Poppins] 
                shadow-[0_2px_6px_rgba(0,0,0,0.6)] min-w-[220px]"
        >
          <div class="font-semibold text-sm tracking-wide">
            {{ selectedEvent?.channelName || 'Unknown Channel' }}
          </div>
          <div class="text-gray-300 text-xs">
            {{ selectedEvent ? formatTime(selectedEvent.timeMs) : '' }}
          </div>
          <div class="text-blue-400 font-semibold text-xs uppercase">
            {{ selectedEvent?.alarmMinor || 'Event' }}
          </div>
        </div>

        <!-- Optional close button -->
        <button
          class="absolute top-4 right-5 text-white/70 hover:text-white text-2xl font-bold"
          @click.stop="previewImg = null"
        >
          ×
        </button>
      </div>
    </div>
  </main>
</template>

<script setup>
import { ref, onMounted, onUnmounted, computed, watch } from 'vue'
import Multiselect from 'vue-multiselect'
import 'vue-multiselect/dist/vue-multiselect.min.css'
import { useRouter } from 'vue-router'
import md5 from 'crypto-js/md5'

/* ---------- Page Pagination ---------- */
const itemsPerPage = ref(parseInt(localStorage.getItem('dl_items_per_page')) || 20)
const currentPage = ref(1)
const setItemsPerPage = (val) => {
  itemsPerPage.value = val
  localStorage.setItem('dl_items_per_page', val)
  currentPage.value = 1
}

const BASE_URL = '/cube/api/v1'
const BASE = '';
const secretKey = '48a35a267aff46fe118dfb564cf1aa10'
const cappkey = 'c8a13387c2d941c23e64af'


function signRequest(params = { uri: '', method: '', queryParamStr: '', requestBody: '{}', secretKey: '', ctimestamp: '', cnonce: '', cappkey: '' }) {
  const requestBodyMD5 = md5(params.requestBody).toString()
  const formattedParams = `${params.uri}-${params.method}-${params.queryParamStr}-${requestBodyMD5}-${params.secretKey}-${params.ctimestamp}-${params.cnonce}-${params.cappkey}`
  const csign = md5(formattedParams).toString()

  return {
    'uri': params.uri,
    'requestBodyMD5': requestBodyMD5,
    'secretKey': params.secretKey,
    'csign': csign,
    'ctimestamp': params.ctimestamp,
    'cnonce': params.cnonce,
    'cappkey': params.cappkey
  }
}

/* ---------- State ---------- */
const connected = ref(false)
const activeTab = ref('alarm')
const events = ref([])
const conditionResults = ref([])
const loading = ref(false)
const error = ref('')
const previewImg = ref(null)
const selectedEvent = ref(null)

/* ------ Router --------- */
const router = useRouter()

/* ---------- Auto Refresh Timer ---------- */
let refreshTimer = null

/* ---------- Search Fields ---------- */
function toLocalISOString(date) {
  const offset = date.getTimezoneOffset()
  const local = new Date(date.getTime() - offset * 60000)
  return local.toISOString().slice(0, 16)
}

const now = new Date()
const startOfDay = new Date(now.getFullYear(), now.getMonth(), now.getDate(), 0, 0, 0)

const startTime = ref(toLocalISOString(startOfDay))
const endTime = ref(toLocalISOString(now))
const selectedCameras = ref([])
const selectedAlarmTypes = ref([])
const selectedAlarmRules = ref([])

/* ---------- Dropdown Data ---------- */
const cameraList = ref([])
const alarmTypeList = ref([])
const alarmRuleList = ref([])
const alarmTypeMap = ref({})
const alarmRuleMap = ref({})

/* ---------- LM Shared State (inherits from Condition Search) ---------- */
const lmCameras = ref([])
const lmAlarmTypes = ref([])
const lmAlarmRules = ref([])
const lmQuery = ref('')   // new free-text LLM input
const largeModelResults = ref([])

const paginatedLargeModel = computed(() => {
  const start = (currentPage.value - 1) * itemsPerPage.value
  return largeModelResults.value.slice(start, start + itemsPerPage.value)
})

/* ---------- Computed Pagination ---------- */
const paginatedResults = computed(() => {
  const start = (currentPage.value - 1) * itemsPerPage.value
  return conditionResults.value.slice(start, start + itemsPerPage.value)
})

/* ---------- Lifecycle ---------- */
onMounted(async () => {
  try {
    const res = await fetch('/cube/monitor', { credentials: 'include' })
    connected.value = res.ok
  } catch {
    connected.value = false
  }

  await Promise.all([
    fetchAllAlarms(),
    fetchCameraList(),
    fetchAlarmType(),
    fetchAlarmRule()
  ])

  startAutoRefresh()
})

onUnmounted(() => stopAutoRefresh())

/* ---------- Auto Refresh Controls ---------- */
const startAutoRefresh = () => {
  stopAutoRefresh()
  refreshTimer = setInterval(async () => {
    if (activeTab.value === 'alarm') {
      await fetchAllAlarms(false)
    }
  }, 500)
}

const stopAutoRefresh = () => {
  if (refreshTimer) {
    clearInterval(refreshTimer)
    refreshTimer = null
  }
}

/* ---------- Watch Tab Switch ---------- */
watch(activeTab, (tab) => {
  if (tab === 'alarm') startAutoRefresh()
  else stopAutoRefresh()
})

/* ---------- CAMERA LIST ---------- */
const fetchCameraList = async () => {
  try {

    const uri = `${BASE_URL}/device/channel/search`
    const requestBody = JSON.stringify({ pageNum: 1, pageSize: 200 })

    const signature = signRequest({
      uri,
      method: 'POST',
      queryParamStr: '',
      requestBody,
      secretKey,
      ctimestamp: Date.now().toString(),
      cnonce: crypto.randomUUID().replaceAll('-', ''),
      cappkey
    })


    const res = await fetch(uri, {
      method: 'POST',
      credentials: 'include',
      headers: { 'Content-Type': 'application/json', ...signature },
      body: requestBody,
    })
    const json = await res.json()
    cameraList.value = json.data?.list?.map(i => i.name) || []
  } catch (err) {
    console.warn('Camera list fetch failed:', err)
  }
}

/* ---------- ALARM TYPE (Translated to English) ---------- */
const fetchAlarmType = async () => {
  try {

    // const uri = `${BASE_URL}/dict/alarm_major`
    const uri = `${BASE_URL}/event/record/alarmEvent/list`
    //
    const requestBody = JSON.stringify({})

    const signature = signRequest({
      uri,
      method: 'POST',
      queryParamStr: '',
      requestBody: requestBody,
      secretKey,
      ctimestamp: Date.now().toString(),
      cnonce: crypto.randomUUID().replaceAll('-', ''),
      cappkey
    })

    const res = await fetch(uri, { 
        method: 'POST', 
        body: requestBody, 
        headers: { 
        'Content-Type': 'application/json',
         ...signature 
        } 
    })

    const json = await res.json()

    // Chinese → English mapping
    const typeLangMap = {
      '周界警戒': 'Perimeter Defence',
      '行为检测': 'Behavior Detection',
      '目标检测': 'Object Detection',
      '异常检测': 'Advanced Detection',
    }

    if (json.data?.length) {
      // Translate to English
      alarmTypeList.value = json.data.map(i => typeLangMap[i.label] || i.label)
      alarmTypeMap.value = Object.fromEntries(
        json.data.map(i => [typeLangMap[i.label] || i.label, i.value])
      )
    } else {
      // Fallback defaults
      alarmTypeList.value = Object.values(typeLangMap)
      alarmTypeMap.value = {
          'Advanced Detection': 'megalarm-mf-N-person-cu102-t4-fq',
          'Behavior Detection': 'megalarm-mf-N-person-cu102-t4-act-nmix',
          'Perimeter Defence': 'megalarm-mf-N-person-cu102-t4-peri-nmix',
          'Object Detection': 'goods-server-glider-t4_5.3.1_cube-sentinel'
      }
    }
  } catch {
    alarmTypeList.value = [
      'Perimeter Detection',
      'Behavior Detection',
      'Object Detection',
      'Advanced Detection',
    ]
    alarmTypeMap.value = {
      'Perimeter Detection': 'megalarm-mf-N-person-cu102-t4-peri-nmix',
      'Behavior Detection': 'megalarm-mf-N-person-cu102-t4-behavior-nmix',
      'Object Detection': 'megalarm-mf-N-person-cu102-t4-object-nmix',
      'Advanced Detection': 'megalarm-mf-N-person-cu102-t4-anomaly-nmix',
    }
  }
}

/* ---------- ALARM RULE ---------- */
const fetchAlarmRule = async () => {
  try {
    
    const uri = `${BASE_URL}/event/record/alarmEvent/list`
    //
    const requestBody = JSON.stringify({})

    const signature = signRequest({
      uri,
      method: 'POST',
      queryParamStr: '',
      requestBody: requestBody,
      secretKey,
      ctimestamp: Date.now().toString(),
      cnonce: crypto.randomUUID().replaceAll('-', ''),
      cappkey
    })

    const res = await fetch(uri, { method: 'POST', body: requestBody, headers: { 'Content-Type': 'application/json', ...signature } })
    const json = await res.json()

    if (json.data?.length) {
      json.data.list.forEach(item => {
        item.fullImageUri = item.fullImageUri.replace('//10.117.59.61', '');
        item.imageUri = item.imageUri.replace('//10.117.59.61', '');
      })
      alarmRuleList.value = json.data.map(i => i.label)
      alarmRuleMap.value = Object.fromEntries(json.data.map(i => [i.label, i.value]))
    } else {
      alarmRuleList.value = [
        'Call','Exit','Fall','Fight','Gathering','Intrusion',
        'Park','Run','Sleep','Smoking','Watch Phone'
      ]
      alarmRuleMap.value = {
        'Call': 'CALL','Exit': 'EXIT','Fall': 'FALL','Fight': 'FIGHT','Gathering': 'GATHERING',
        'Intrusion': 'INTRUSION','Park': 'PARK','Run': 'RUN','Sleep': 'SLEEP',
        'Smoking': 'SMOKING','Watch Phone': 'WATCH_PHONE'
      }
    }
  } catch {
    alarmRuleList.value = [
      'Call','Exit','Fall','Fight','Gathering','Intrusion',
      'Park','Run','Sleep','Smoking','Watch Phone'
    ]
    alarmRuleMap.value = {
      'Call': 'CALL','Exit': 'EXIT','Fall': 'FALL','Fight': 'FIGHT','Gathering': 'GATHERING',
      'Intrusion': 'INTRUSION','Park': 'PARK','Run': 'RUN','Sleep': 'SLEEP',
      'Smoking': 'SMOKING','Watch Phone': 'WATCH_PHONE'
    }
  }
}

/* ---------- FETCH ALL ALARMS ---------- */
const fetchAllAlarms = async (showLoading = true) => {
  if (showLoading) loading.value = true
  error.value = ''
  try {
    const uri = `${BASE_URL}/event/record/alarmEvent/list`
    const requestBody = JSON.stringify({
        pageNum: 1,
        pageSize: 100,
        startTime: new Date('2025-09-01T00:00:00').getTime(),
        endTime: new Date().getTime(),
      })

    const signature = signRequest({
      uri,
      method: 'POST',
      queryParamStr: '',
      requestBody,
      secretKey,
      ctimestamp: Date.now().toString(),
      cnonce: crypto.randomUUID().replaceAll('-', ''),
      cappkey
    })

    const res = await fetch(uri, {
      method: 'POST',
      credentials: 'include',
      headers: { 'Content-Type': 'application/json', ...signature },
      body: requestBody,
    })
    const json = await res.json()

// "fullImageUri": "//10.117.59.61/cube/api/v1/storage/download/Z3NwOi8vd2stYWxnb3JpdGhtLWNyb3AvMmM3MTJmMDctMWNlMS00Y2E1LTlhYmUtMTc2NzhhNDM5ZjNk",
// "": "//10.117.59.61/cube/api/v1/storage/download/Z3NwOi8vd2stYWxnb3JpdGhtLWNyb3AvMmM3MTJmMDctMWNlMS00Y2E1LTlhYmUtMTc2NzhhNDM5ZjNk",


    if (json.data?.list) {
      events.value = json.data.list.sort((a, b) => b.timeMs - a.timeMs)

      json.data.list.forEach(item => {
        item.fullImageUri = item.fullImageUri.replace('//10.117.59.61', '');
        item.imageUri = item.imageUri.replace('//10.117.59.61', '');
      })

      //debugger
    }
  } catch (err) {
    //error.value = '⚠️ ' + err.message
  } finally {
    loading.value = false
  }
}

/* ---------- CONDITION SEARCH ---------- */
const searchByChannel = async () => {
  loading.value = true
  error.value = ''
  conditionResults.value = []

  try {
    const uri1 = `${BASE_URL}/device/channel/search`
    const requestBody1 = JSON.stringify({ pageNum: 1, pageSize: 200 })

    const signature1 = signRequest({
      uri: uri1,
      method: 'POST',
      queryParamStr: '',
      requestBody: requestBody1,
      secretKey,
      ctimestamp: Date.now().toString(),
      cnonce: crypto.randomUUID().replaceAll('-', ''),
      cappkey
    })

    const resCam = await fetch(uri1, {
      method: 'POST',
      credentials: 'include',
      headers: { 'Content-Type': 'application/json', ...signature1 },
      body: requestBody1,
    })
    const jsonCam = await resCam.json()
    const allCameras = jsonCam.data?.list || []
    const filtered = selectedCameras.value.length
      ? allCameras.filter(cam => selectedCameras.value.includes(cam.name))
      : allCameras
    const uuids = filtered.map(cam => cam.uuid)

    const majors = selectedAlarmTypes.value.map(t => alarmTypeMap.value[t] || t)
    const minors = selectedAlarmRules.value.map(r => alarmRuleMap.value[r] || r)

    const payload = {
      pageNum: 1,
      pageSize: 100,
      startTime: startTime.value
        ? new Date(startTime.value).getTime()
        : Date.now() - 7 * 24 * 3600 * 1000,
      endTime: endTime.value ? new Date(endTime.value).getTime() : Date.now(),
    }
    if (uuids.length) payload.channelUuids = uuids
    if (majors.length) payload.alarmMajors = majors
    if (minors.length) payload.alarmMinors = minors

    const uri2 = `${BASE_URL}/event/record/alarmEvent/list`
    const requestBody2 = JSON.stringify(payload)

    const signature2 = signRequest({
      uri: uri2,
      method: 'POST',
      queryParamStr: '',
      requestBody: requestBody2,
      secretKey,
      ctimestamp: Date.now().toString(),
      cnonce: crypto.randomUUID().replaceAll('-', ''),
      cappkey
    })

    const resEv = await fetch(uri2, {
      method: 'POST',
      credentials: 'include',
      headers: { 'Content-Type': 'application/json', ...signature2 },
      body: requestBody2,
    })
    const txt = await resEv.text()
    if (txt.trim().startsWith('<')) throw new Error('Login required')
    const jsonEv = JSON.parse(txt)

  //       if (jsonEv.code === 0 && jsonEv.data?.list?.length)
  //     conditionResults.value = jsonEv.data.list
  //   else
  //     error.value = '⚠️ No results found for current filters.'
  // } catch (err) {
  //   error.value = '⚠️ ' + err.message
  // } finally {
  //   loading.value = false
  // }
  
    if (jsonEv.code === 0 && jsonEv.data?.list?.length) {

      // ✅ Clean URLs before assigning
      jsonEv.data.list.forEach(item => {
        if (item.fullImageUri)
          item.fullImageUri = item.fullImageUri.replace('//10.117.59.61', '')
        if (item.imageUri)
          item.imageUri = item.imageUri.replace('//10.117.59.61', '')
      })

      // ✅ Assign cleaned results
      conditionResults.value = jsonEv.data.list.sort((a, b) => b.timeMs - a.timeMs)

    } else {
      error.value = '⚠️ No results found for current filters.'
    }
  } catch (err) {
    error.value = '⚠️ ' + err.message
  } finally {
    loading.value = false
  }
}

/* ---------- LARGE MODEL GLOBAL SEARCH (TEXT-BASED) ---------- */
const searchLargeModel = async () => {
  loading.value = true
  error.value = ''
  largeModelResults.value = []

  try {
    const query = lmQuery.value.trim()
    if (!query) {
      error.value = '⚠️ Please enter a keyword to search (camera, type, or rule).'
      loading.value = false
      return
    }

    const uri1 = `${BASE_URL}/device/channel/search`
    const requestBody1 = JSON.stringify({ pageNum: 1, pageSize: 500 })

    const signature1 = signRequest({
      uri: uri1,
      method: 'POST',
      queryParamStr: '',
      requestBody: requestBody1,
      secretKey,
      ctimestamp: Date.now().toString(),
      cnonce: crypto.randomUUID().replaceAll('-', ''),
      cappkey
    })

    /* 1️⃣ Get all cameras */
    const resCam = await fetch(uri1, {
      method: 'POST',
      credentials: 'include',
      headers: { 'Content-Type': 'application/json', ...signature1 },
      body: requestBody1,
    })
    const jsonCam = await resCam.json()
    const allCameras = jsonCam.data?.list || []

    /* 2️⃣ Match text with camera name, alarm type, and rule */
    const matchingCameras = allCameras.filter(cam =>
      cam.name.toLowerCase().includes(query.toLowerCase())
    )
    const matchingAlarmTypes = alarmTypeList.value.filter(t =>
      t.toLowerCase().includes(query.toLowerCase())
    )
    const matchingAlarmRules = alarmRuleList.value.filter(r =>
      r.toLowerCase().includes(query.toLowerCase())
    )

    /* 3️⃣ Collect matched UUIDs and alarm values */
    const uuids = matchingCameras.map(cam => cam.uuid)
    const majors = matchingAlarmTypes.map(t => alarmTypeMap.value[t] || t)
    const minors = matchingAlarmRules.map(r => alarmRuleMap.value[r] || r)

    /* 4️⃣ Build global search payload */
    const payload = {
      pageNum: 1,
      pageSize: 200,
      startTime: startTime.value
        ? new Date(startTime.value).getTime()
        : Date.now() - 30 * 24 * 3600 * 1000, // last 30 days
      endTime: endTime.value ? new Date(endTime.value).getTime() : Date.now(),
    }
    if (uuids.length) payload.channelUuids = uuids
    if (majors.length) payload.alarmMajors = majors
    if (minors.length) payload.alarmMinors = minors

    /* 5️⃣ If no matches found, still send text for LLM global search */
    if (!uuids.length && !majors.length && !minors.length) {
      payload.text = query
    }

    const uri2 = `${BASE_URL}/event/record/alarmEvent/list`
    const requestBody2 = JSON.stringify(payload)

    const signature2 = signRequest({
      uri: uri2,
      method: 'POST',
      queryParamStr: '',
      requestBody: requestBody2,
      secretKey,
      ctimestamp: Date.now().toString(),
      cnonce: crypto.randomUUID().replaceAll('-', ''),
      cappkey
    })

    /* 6️⃣ Query backend */
    const resEv = await fetch('/cube/api/v1/event/record/alarmEvent/list', {
    // const resEv = await fetch('/event/record/alarmEvent/list', {
      method: 'POST',
      credentials: 'include',
      headers: { 'Content-Type': 'application/json', ...signature2 },
      body: requestBody2,
    })
    const txt = await resEv.text()
    if (txt.trim().startsWith('<')) throw new Error('Login required')
    const jsonEv = JSON.parse(txt)

    /* 7️⃣ Handle results */
    if (jsonEv.code === 0 && jsonEv.data?.list?.length) {

      jsonEv.data.list.forEach(item => {
        item.fullImageUri = item.fullImageUri.replace('//10.117.59.61', '');
        item.imageUri = item.imageUri.replace('//10.117.59.61', '');
      })

      largeModelResults.value = jsonEv.data.list.sort((a, b) => b.timeMs - a.timeMs)
    } else {
      error.value = `⚠️ No results found for "${query}".`
    }
  } catch (err) {
    error.value = '⚠️ ' + err.message
  } finally {
    loading.value = false
  }
}

/* ---------- Helpers ---------- */
const normalizeUrl = uri =>
  !uri
    ? ''
    : uri.startsWith('http')
    ? uri
    : uri.startsWith('//')
    ? 'http:' + uri
    // : 'http://192.168.1.189' + uri
    : 'http://117.161.21.102' + uri

const formatTime = ms => (ms ? new Date(parseInt(ms)).toLocaleString() : 'N/A')
const hideOnError = e => (e.target.style.display = 'none')
const openPreview = (url, ev) => {
  previewImg.value = url
  selectedEvent.value = ev || null
}

/* ---------- When LM button is clicked ---------- */
const goToLargeModel = () => {
  // optional: pass selected filters as query parameters
  router.push({
    name: 'LargeModelSearch',
    query: {
      cameras: selectedCameras.value.join(','),
      alarmTypes: selectedAlarmTypes.value.join(','),
      alarmRules: selectedAlarmRules.value.join(',')
    }
  })
}

</script>

<style scoped>
/* === Fix for overlay clipping === */
main {
  overflow: visible !important;
}

/* === Filter bar layering fix === */
.filter-bar {
  position: relative;
  z-index: 9999 !important;
  flex-wrap: wrap;
  overflow: visible !important; /* allow dropdown to expand */
}

/* === Video grid stays below filter bar === */
.video-wall {
  position: relative;
  z-index: 1 !important;
  overflow: visible !important;
}

/* === Base input (no hover effect) === */
.input-base {
  padding: 16px 14px;               /* more padding inside */
  border-radius: 8px;
  background: #23283a;
  border: 1px solid #444;
  color: #fff;
  font-size: 16px;
  width: 260px;
  height: 55px;                     /* 👈 increased height */
  outline: none;
  transition: all 0.2s ease;
}
.input-base:hover,
.input-base:focus {
  border-color: #2563eb;
  background: #23283a;
  box-shadow: none;
  transform: none;
}

/* === Dropdown absolute overlay === */
.custom-multiselect {
  position: relative !important;
  z-index: 10000 !important;
  overflow: visible !important;
}
.multiselect__content-wrapper {
  position: absolute !important;
  top: 100%;
  left: 0;
  z-index: 11000 !important;
  background-color: #1b1f2d !important;
  border: 1px solid #333 !important;
  box-shadow: 0 12px 24px rgba(0, 0, 0, 0.7);
  border-radius: 8px;
}

/* === Dropdown styling === */
.multiselect__tags {
  background-color: #1b1f2d !important;
  border: 1px solid rgba(255,255,255,0.1) !important;
  color: #ddd !important;
  border-radius: 8px;
}
.multiselect__option {
  padding: 6px 12px;
  color: #ccc;
}
.multiselect__option--highlight,
.multiselect__option--selected {
  background-color: #2563eb !important;
  color: #fff !important;
}

/* === Fix: Image preview always on top === */
.fixed[inset-0] {
  z-index: 999999 !important; /* topmost layer */
}

/* Optional: smooth fade-in */
.fixed[inset-0] {
  animation: fadeIn 0.2s ease-in-out;
}

@keyframes fadeIn {
  from {
    opacity: 0;
    transform: scale(0.98);
  }
  to {
    opacity: 1;
    transform: scale(1);
  }
}

/* 🔴 Red Glass Breathing */
@keyframes breath-glass-red {
  0% {
    box-shadow:
      0 0 12px rgba(255,120,120,0.4),
      inset 0 0 6px rgba(255,255,255,0.08);
    transform: scale(1);
  }
  50% {
    box-shadow:
      0 0 28px rgba(255,140,140,0.6),
      inset 0 0 16px rgba(255,255,255,0.15);
    transform: scale(1.05);
  }
  100% {
    box-shadow:
      0 0 12px rgba(255,120,120,0.4),
      inset 0 0 6px rgba(255,255,255,0.08);
    transform: scale(1);
  }
}
.animate-breath-glass-red {
  animation: breath-glass-red 3s ease-in-out infinite;
  transition: all 0.3s ease;
}

/* 🔵 Blue Glass Breathing */
@keyframes breath-glass-blue {
  0% {
    box-shadow:
      0 0 12px rgba(120,180,255,0.4),
      inset 0 0 6px rgba(255,255,255,0.08);
    transform: scale(1);
  }
  50% {
    box-shadow:
      0 0 28px rgba(160,210,255,0.6),
      inset 0 0 16px rgba(255,255,255,0.15);
    transform: scale(1.05);
  }
  100% {
    box-shadow:
      0 0 12px rgba(120,180,255,0.4),
      inset 0 0 6px rgba(255,255,255,0.08);
    transform: scale(1);
  }
}
.animate-breath-glass-blue {
  animation: breath-glass-blue 3s ease-in-out infinite;
  transition: all 0.3s ease;
}



</style>