<template>
  <aside class="rounded-md border border-white/10 bg-[#121826] p-3">
    <!-- ✅ 2-column grid layout -->
    <div class="grid grid-cols-2 gap-3">
      <button class="widget-rect hover:scale-[1.03] transition">
        <img src="/icon/video-cam.png" class="w-7 h-7 mb-1 icon-invert" />
        <div class="opacity-90 text-[10px] font-medium">Video Channel</div>
      </button>

      <button class="widget-rect hover:scale-[1.03] transition">
        <img src="/icon/analytics.png" class="w-7 h-7 mb-1 icon-invert" />
        <div class="opacity-90 text-[10px] font-medium">Analytics</div>
      </button>

      <button class="widget-rect hover:scale-[1.03] transition">
        <img src="/icon/alarm-real-time.png" class="w-7 h-7 mb-1 icon-invert" />
        <div class="opacity-90 text-[10px] font-medium">
          Alarm <br /> Real Time
        </div>
      </button>
    </div>
  </aside>
</template>

<script setup>
// No script logic needed
</script>

<style scoped>
.widget-rect {
  /* ✅ Smaller size buttons */
  @apply flex flex-col items-center justify-center w-full h-24 rounded-md border border-white/10 bg-[#1a2030] px-2 py-2 hover:bg-white/5 transition cursor-pointer;
}

.icon-invert {
  filter: invert(1) brightness(2);
}
</style>
