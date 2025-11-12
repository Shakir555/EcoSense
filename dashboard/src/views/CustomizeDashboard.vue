<template>
  <div class="flex flex-col min-h-full py-4">
    <!-- Tabs & Dashboard Manager -->
    <div class="h-10 flex items-center border-b border-white/10 bg-[#121725] px-2 gap-3 flex-shrink-0">
      <div class="relative">
        <button
          class="flex items-center gap-1 px-3 h-7 bg-[#1a1f2f] hover:bg-[#1f2638] rounded-md text-xs"
          @click="showManager = !showManager"
        >
          Dashboard ▾
        </button>

        <transition name="fade">
          <div
            v-if="showManager"
            class="absolute mt-2 w-80 bg-[#1a1f2f] border border-white/10 rounded-md shadow-xl z-50 p-2"
          >
            <div class="max-h-80 overflow-auto space-y-1 pr-1">
              <div
                v-for="d in dashboards"
                :key="d.id"
                class="flex items-center justify-between px-3 py-2 rounded hover:bg-white/5 transition-colors duration-200 text-[12px]"
              >
                <div class="flex items-center gap-2 w-[75%]">
                  <button
                    class="text-left w-full rounded transition-colors duration-200 font-[500] tracking-wide"
                    :class="currentId === d.id ? 'text-blue-400' : 'text-gray-200 hover:text-blue-300'"
                    @click="switchDashboard(d.id)"
                  >
                    {{ d.title }}
                  </button>
                </div>
                <div class="flex items-center gap-1">
                  <button class="p-1 hover:bg-white/10 rounded" @click="startRename(d)">
                    <img src="/icon/edit.png" class="w-4 h-4 object-contain icon-invert" />
                  </button>
                  <button class="p-1 hover:bg-white/10 rounded" @click="deleteDashboard(d.id)">
                    <img src="/icon/delete.png" class="w-4 h-4 object-contain icon-invert" />
                  </button>
                </div>
              </div>
            </div>

            <div v-if="renaming" class="mt-2 flex gap-2">
              <input v-model="renameText" class="flex-1 bg-transparent border-b border-blue-400 text-xs focus:outline-none" autofocus />
              <button class="px-2 py-1 bg-blue-500/30 rounded text-xs" @click="confirmRename">Save</button>
              <button class="px-2 py-1 bg-white/10 rounded text-xs" @click="cancelRename">Cancel</button>
            </div>

            <div class="mt-2 flex justify-end">
              <button class="px-3 py-1 bg-white/10 hover:bg-white/15 rounded-md text-xs" @click="resetAllToDefaults">↺ Reset Defaults</button>
            </div>
          </div>
        </transition>
      </div>

      <!-- Add Dashboard -->
      <button
        class="flex items-center gap-1.5 px-4 py-1.5 bg-gradient-to-r from-white/10 to-white/5 backdrop-blur-md border border-white/20 rounded-lg text-[11px] font-semibold text-white shadow-[0_0_10px_rgba(255,255,255,0.2)] hover:shadow-[0_0_15px_rgba(255,255,255,0.4)] transition-all duration-300 active:scale-95 focus:ring-2 focus:ring-white/30"
        @click="addNewLayout"
      >
        <span class="text-base leading-none">＋</span> Add Dashboard
      </button>

      <!-- Tabs -->
      <div class="flex items-center gap-2 overflow-x-auto">
        <div
          v-for="tab in dashboards"
          :key="tab.id"
          @click="switchDashboard(tab.id)"
          class="flex items-center gap-2 px-4 py-1 rounded-md text-xs cursor-pointer transition"
          :class="currentId === tab.id ? 'bg-blue-500/30 border border-blue-400 text-blue-300' : 'bg-[#1a1f2f] border border-white/10 hover:bg-white/5'"
        >
          {{ tab.title }}
          <button class="text-red-400 hover:text-red-500" @click.stop="deleteDashboard(tab.id)">✕</button>
        </div>
      </div>
    </div>

    <!-- Active Dashboard -->
    <div class="grid grid-cols-[1fr_240px] gap-3 px-3 py-3">
      <div class="rounded-md border border-white/10 bg-[#101521]">
        <div class="flex items-center justify-between px-4 py-3 border-b border-white/10">
          <h2 class="text-sm font-semibold">{{ activeDash.title }}</h2>
          <div class="flex items-center gap-2 text-xs">
            <button class="px-2 py-1 rounded bg-white/10 hover:bg-white/15" @click="toggleEditMode(activeDash)">
              {{ activeDash.editMode ? 'Lock Layout' : 'Edit Layout' }}
            </button>
            <button class="px-2 py-1 rounded bg-white/10 hover:bg-white/15" @click="resetLayoutToDefaultStructure(activeDash)">
              Reset Layout
            </button>
          </div>
        </div>

        <!-- Grid Layout -->
        <div class="p-3">
          <GridLayout
            :key="gridKey"
            :layout="activeDash.layout"
            :col-num="12"
            :row-height="24"
            :is-draggable="activeDash.editMode"
            :is-resizable="activeDash.editMode"
            :margin="[10, 10]"
            :use-css-transforms="true"
            :auto-size="true"
            :vertical-compact="true"
            :compact-type="'vertical'"
            :prevent-collision="false"
            :is-bounded="true"
            :transform-scale="1"
            :draggable-handle="'.drag-handle'"
            @layout-updated="onLayoutUpdated(activeDash, $event)"
          >
            <GridItem
              v-for="w in activeDash.layout"
              :key="w.i"
              :i="w.i"
              :x="w.x"
              :y="w.y"
              :w="w.w"
              :h="w.h"
            >
              <div class="h-full rounded-md border border-white/10 bg-[#0e1320] flex flex-col vue-item">
                <div class="flex items-center justify-between px-3 py-2 text-xs border-b border-white/10">
                  <div class="flex items-center gap-2">
                    <span
                      class="drag-handle inline-flex items-center justify-center w-5 h-5 rounded hover:bg-white/10 cursor-move select-none"
                      :class="activeDash.editMode ? 'opacity-100' : 'opacity-30 cursor-default'"
                      title="Drag"
                    >⋮⋮</span>
                    <div class="opacity-80">{{ placeholderTitle(w) }}</div>
                  </div>
                </div>
                <div class="flex-1 p-3 text-[11px] text-gray-400 flex items-center justify-center">
                  <div v-if="w.type === 'placeholder'" class="opacity-60">(Empty)</div>
                </div>
              </div>
            </GridItem>
          </GridLayout>
        </div>
      </div>

      <!-- Sidebar Widgets -->
      <DashboardWidgets />
    </div>
  </div>
</template>

<script setup>
import { ref, computed, watch } from 'vue'
import { GridLayout, GridItem } from 'vue3-grid-layout'
import DashboardWidgets from './DashboardWidgets.vue'

const STORAGE_KEY = 'dashboards_v5'
const dashboards = ref(load())
const currentId = ref(dashboards.value[0]?.id || null)
const gridKey = ref(0)

const activeDash = computed(() => dashboards.value.find(d => d.id === currentId.value))
function forceGridRefresh() { gridKey.value++ }

function load() {
  const saved = localStorage.getItem(STORAGE_KEY)
  if (saved) return JSON.parse(saved)
  return [createDashboard('Customize Dashboard-1'), createDashboard('Customize Dashboard-2')]
}
function save() { localStorage.setItem(STORAGE_KEY, JSON.stringify(dashboards.value)) }
watch(dashboards, save, { deep: true })

function defaultStructureLayout() {
  return [
    { i: 'left-top', x: 0, y: 0, w: 3, h: 6, type: 'placeholder' },
    { i: 'left-bottom', x: 0, y: 6, w: 3, h: 6, type: 'placeholder' },
    { i: 'center', x: 3, y: 0, w: 7, h: 12, type: 'placeholder' },
    { i: 'right', x: 10, y: 0, w: 2, h: 12, type: 'placeholder' },
    { i: 'bottom', x: 0, y: 12, w: 12, h: 5, type: 'placeholder' }
  ]
}
function createDashboard(title) {
  return { id: Date.now() + Math.random(), title, editMode: false, layout: defaultStructureLayout() }
}

const showManager = ref(false)
const renaming = ref(false)
const renameTargetId = ref(null)
const renameText = ref('')
function startRename(d) { renaming.value = true; renameTargetId.value = d.id; renameText.value = d.title }
function confirmRename() {
  const d = dashboards.value.find(x => x.id === renameTargetId.value)
  if (d) d.title = renameText.value || d.title
  renaming.value = false; renameTargetId.value = null; renameText.value = ''
}
function cancelRename() { renaming.value = false; renameTargetId.value = null }

function switchDashboard(id) {
  dashboards.value.forEach(d => d.editMode = false)
  currentId.value = id
  forceGridRefresh()
}
function deleteDashboard(id) {
  if (!confirm('Delete this dashboard?')) return
  dashboards.value = dashboards.value.filter(d => d.id !== id)
  if (currentId.value === id && dashboards.value.length) currentId.value = dashboards.value[0].id
}

function toggleEditMode(d) {
  d.editMode = !d.editMode
  if (!d.editMode) forceGridRefresh()
}
function resetLayoutToDefaultStructure(d) {
  d.layout = defaultStructureLayout()
  d.editMode = false
  forceGridRefresh()
}
function onLayoutUpdated(d, layout) {
  d.layout = layout.map(it => {
    const prev = d.layout.find(p => p.i === it.i)
    return { ...it, type: prev?.type ?? 'placeholder', props: prev?.props ?? {} }
  })
}
function resetAllToDefaults() {
  if (!confirm('Reset ALL dashboards?')) return
  dashboards.value = [createDashboard('Customize Dashboard-1'), createDashboard('Customize Dashboard-2')]
  currentId.value = dashboards.value[0].id
  forceGridRefresh()
}

function addNewLayout() {
  const newDash = createDashboard(`Customize Dashboard-${dashboards.value.length + 1}`)
  dashboards.value.push(newDash)
  currentId.value = newDash.id
  forceGridRefresh()
}

function placeholderTitle(w) {
  switch (w.i) {
    case 'left-top': return 'Left (Top)'
    case 'left-bottom': return 'Left (Bottom)'
    case 'center': return 'Display Area'
    case 'right': return 'Hidden Tools'
    case 'bottom': return 'Bottom'
    default: return 'Empty'
  }
}
</script>

<style scoped>
.icon-invert { filter: invert(1) brightness(2); }
.fade-enter-active, .fade-leave-active { transition: opacity .3s ease, transform .3s ease; }
.fade-enter-from, .fade-leave-to { opacity: 0; transform: translateY(-8px); }
</style>
