<template>
  <div
    class="min-h-screen w-full bg-cover bg-center bg-no-repeat flex items-center justify-center relative"
    :style="{ backgroundImage: `url(/bg/intelligence.jpg)` }"
  >
    <!-- Overlay -->
    <div class="absolute inset-0 bg-black/70 backdrop-blur-sm"></div>

    <!-- Login Card -->
    <div
      class="relative z-10 w-full max-w-sm p-4 rounded-2xl bg-white/10 backdrop-blur-lg border border-white/20 shadow-2xl text-gray-100 transform hover:scale-[1.01] transition"
    >
      <!-- ✅ Logo -->
      <div class="flex justify-center mb-2">
        <img
          src="/logo/senturian_logo.png"
          alt="Senturian Logo"
          class="w-20 h-20 object-contain drop-shadow-[0_0_12px_rgba(59,130,246,0.5)] hover:scale-110 transition-transform duration-500"
        />
      </div>

      <h2 class="text-2xl font-bold text-center mb-1 mt-[-4px] tracking-wide">
        Data Library Access
      </h2>
      <p class="text-center text-gray-400 mb-3 text-sm">
        Secure login for Big Data & IoT Management
      </p>

      <!-- Login User -->
      <div class="mb-3">
        <label class="block text-sm font-medium mb-1">Login User</label>
        <input
          v-model="username"
          type="text"
          placeholder="Enter username"
          class="w-full px-3 py-2 rounded-lg bg-gray-900/60 border border-white/20 focus:ring-2 focus:ring-blue-500 outline-none placeholder-gray-500"
        />
      </div>

      <!-- Password Verification -->
      <div class="mb-3">
        <label class="block text-sm font-medium mb-1">Password</label>
        <input
          v-model="password"
          type="password"
          placeholder="Enter password"
          class="w-full px-3 py-2 rounded-lg bg-gray-900/60 border border-white/20 focus:ring-2 focus:ring-blue-500 outline-none placeholder-gray-500"
        />
      </div>

      <!-- Multi-factor Authentication -->
      <div class="mb-3">
        <label class="block text-sm font-medium mb-1">Multi-Factor Code</label>
        <input
          v-model="mfaCode"
          type="text"
          placeholder="Enter authentication code"
          class="w-full px-3 py-2 rounded-lg bg-gray-900/60 border border-white/20 focus:ring-2 focus:ring-blue-500 outline-none placeholder-gray-500"
        />
      </div>

      <!-- Save Login Options -->
      <div class="flex flex-col space-y-2 mb-4 text-sm">
        <!-- Save login user -->
        <label
          class="flex items-center justify-between bg-gray-800/40 px-3 py-2 rounded-lg cursor-pointer hover:bg-gray-800/60 transition"
        >
          <span class="text-[14px] tracking-wide font-medium text-gray-200">Save login user</span>
          <input type="checkbox" v-model="saveUser" class="toggle" />
        </label>

        <!-- Save password -->
        <label
          class="flex items-center justify-between bg-gray-800/40 px-3 py-2 rounded-lg cursor-pointer hover:bg-gray-800/60 transition"
        >
          <span class="text-[14px] tracking-wide font-medium text-gray-200">Save password</span>
          <input type="checkbox" v-model="savePassword" class="toggle" />
        </label>

        <!-- ✅ Delete Saved Button - Modern Look -->
        <button
          @click="clearSaved"
          class="relative inline-block text-[14px] font-semibold text-transparent bg-clip-text bg-gradient-to-r from-red-400 to-pink-400 hover:from-red-300 hover:to-pink-300 transition-all duration-300 mt-1 tracking-wide group"
        >
          Delete saved records
          <span
            class="absolute left-0 bottom-[-2px] w-0 h-[2px] bg-gradient-to-r from-red-400 to-pink-400 transition-all duration-300 group-hover:w-full"
          ></span>
        </button>
      </div>

      <!-- Login Button -->
      <button
        @click="login"
        class="w-full py-2 bg-gradient-to-r from-blue-600 to-cyan-500 hover:from-blue-500 hover:to-cyan-400 rounded-lg font-semibold text-base tracking-wide shadow-lg hover:shadow-blue-500/30 transition"
      >
        Login
      </button>

      <!-- Footer -->
      <div class="mt-3 text-xs text-center text-gray-400">
        © 2025 Senturian Systems. All rights reserved.
      </div>
    </div>
  </div>
</template>

<script setup>
import { ref, onMounted, watch } from "vue";
import { useRouter } from "vue-router";

const router = useRouter();
const username = ref("");
const password = ref("");
const mfaCode = ref("");
const saveUser = ref(false);
const savePassword = ref(false);

// ✅ Load saved credentials on mount
onMounted(() => {
  const savedUser = localStorage.getItem("savedUsername");
  const savedPass = localStorage.getItem("savedPassword");
  if (savedUser) {
    username.value = savedUser;
    saveUser.value = true;
  }
  if (savedPass) {
    password.value = savedPass;
    savePassword.value = true;
  }
});

// ✅ Auto-save when toggled or updated
watch(saveUser, (val) => {
  if (val) localStorage.setItem("savedUsername", username.value);
  else localStorage.removeItem("savedUsername");
});

watch(savePassword, (val) => {
  if (val) localStorage.setItem("savedPassword", password.value);
  else localStorage.removeItem("savedPassword");
});

watch(username, (val) => {
  if (saveUser.value) localStorage.setItem("savedUsername", val);
});
watch(password, (val) => {
  if (savePassword.value) localStorage.setItem("savedPassword", val);
});

// ✅ Delete all saved data
const clearSaved = () => {
  localStorage.removeItem("savedUsername");
  localStorage.removeItem("savedPassword");
  saveUser.value = false;
  savePassword.value = false;
  username.value = "";
  password.value = "";
  alert("✅ Saved login data deleted successfully.");
};

// ✅ Login button
const login = () => {
  if (username.value && password.value && mfaCode.value) {
    console.log("✅ Login successful");
    router.push("/dashboard");
  } else {
    alert("Please fill in all fields.");
  }
};
</script>

<style scoped>
/* Custom modern toggle switch */
input.toggle {
  appearance: none;
  width: 42px;
  height: 22px;
  background: #444;
  border-radius: 999px;
  position: relative;
  cursor: pointer;
  transition: background 0.3s;
}
input.toggle:checked {
  background: linear-gradient(90deg, #3b82f6, #06b6d4);
}
input.toggle::before {
  content: "";
  position: absolute;
  top: 2px;
  left: 2px;
  width: 18px;
  height: 18px;
  background: #fff;
  border-radius: 50%;
  transition: transform 0.3s;
}
input.toggle:checked::before {
  transform: translateX(20px);
}
</style>
