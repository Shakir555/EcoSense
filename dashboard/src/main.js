import { createApp } from 'vue'
import App from './App.vue'
import router from './router/index.js'
import './index.css' // TailwindCSS

createApp(App).use(router).mount('#app')
