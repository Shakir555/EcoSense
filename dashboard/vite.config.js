import { fileURLToPath, URL } from 'node:url'
import { defineConfig } from 'vite'
import vue from '@vitejs/plugin-vue'
import vueDevTools from 'vite-plugin-vue-devtools'

// ===== Update this to your ESP32's actual IP address =====
const ESP32_IP = '192.168.1.34' // example; check serial monitor

export default defineConfig({
  plugins: [
    vue(),
    vueDevTools(),
  ],
  resolve: {
    alias: {
      '@': fileURLToPath(new URL('./src', import.meta.url)),
    },
  },
  server: {
    host: '0.0.0.0', // allow access from LAN devices if needed
    port: 5080,
    proxy: {
      '/esp32': {
        target: `http://${ESP32_IP}`, // ✅ send requests to ESP32
        changeOrigin: true,
        rewrite: (path) => path.replace(/^\/esp32/, ''),
      },
    },
  },
})
