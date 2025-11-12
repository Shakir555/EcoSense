import { createApp } from 'vue'
import App from './App.vue'
import './index.css' // keep Tailwind import

// ❌ Remove this line if it exists:
// import router from './router'

const app = createApp(App)

// ❌ Remove this too:
// app.use(router)

app.mount('#app')
