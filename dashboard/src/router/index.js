import { createRouter, createWebHistory } from 'vue-router'
import LoginUser from '../components/LoginUser.vue'
import Dashboard from '../views/Dashboard.vue'

const routes = [
  {
    path: '/login', // ✅ Now login is at /login
    name: 'Login',
    component: LoginUser
  },
  {
    path: '/dashboard',
    name: 'Dashboard',
    component: Dashboard
  },
  {
    path: '/', // ✅ Redirect root to /login (optional but best practice)
    redirect: '/login'
  },
  {
    path: '/:pathMatch(.*)*', // ✅ Catch-all for unknown routes
    redirect: '/login'
  }
]

const router = createRouter({
  history: createWebHistory(),
  routes
})

export default router
