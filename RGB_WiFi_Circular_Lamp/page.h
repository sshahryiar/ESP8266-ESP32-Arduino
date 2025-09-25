#pragma once


#include <pgmspace.h>


const char MAIN_page[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <title>ESP8266 RGB Controller</title>
  <style>
    body { text-align:center; background:#222; color:white; font-family:Arial; }
    .btn { width:60px; height:40px; margin:4px; border:none;
           font-size:14px; color:white; border-radius:6px; cursor:pointer; }
    .row { margin:8px; }
    input[type=range] { width:200px; }
  </style>
  <script>
    function send(cmd) { fetch("/set?" + cmd); }

    function updateRGB() {
      var r = document.getElementById("r").value;
      var g = document.getElementById("g").value;
      var b = document.getElementById("b").value;
      fetch(`/set?r=${r}&g=${g}&b=${b}`);
    }

    function updatePicker() {
      var c = document.getElementById("picker").value;
      fetch(`/set?color=${c.substring(1)}`);
    }

    function updateBrightness() {
      var br = document.getElementById("brightness").value;
      fetch(`/set?brightness=${br}`);
    }
  </script>
</head>
<body>
  <h2>RGB LED Remote</h2>

  <!-- Power -->
  <div class="row">
    <button class="btn" style="background:#444" onclick="send('power=off')">OFF</button>
    <button class="btn" style="background:red" onclick="send('power=on')">ON</button>
  </div>

  <!-- Sliders -->
  <div class="row">
    <label>R</label><input id="r" type="range" min="0" max="255" value="255" oninput="updateRGB()"><br>
    <label>G</label><input id="g" type="range" min="0" max="255" value="255" oninput="updateRGB()"><br>
    <label>B</label><input id="b" type="range" min="0" max="255" value="255" oninput="updateRGB()"><br>
    <label>L</label><input id="brightness" type="range" min="0" max="255" value="255" oninput="updateBrightness()">
  </div>

  <!-- Color Picker -->
  <div class="row">
    <input type="color" id="picker" value="#ffffff" onchange="updatePicker()">
  </div>

  <!-- Main Colors -->
  <div class="row">
    <button class="btn" style="background:red" onclick="send('color=FF0000')">R</button>
    <button class="btn" style="background:green" onclick="send('color=00FF00')">G</button>
    <button class="btn" style="background:blue" onclick="send('color=0000FF')">B</button>
    <button class="btn" style="background:white; color:black" onclick="send('color=FFFFFF')">W</button>
  </div>

  <!-- Preset Colors -->
  <div class="row">
    <button class="btn" style="background:orange" onclick="send('color=FF8000')"></button>
    <button class="btn" style="background:yellow; color:black" onclick="send('color=FFFF00')"></button>
    <button class="btn" style="background:cyan; color:black" onclick="send('color=00FFFF')"></button>
    <button class="btn" style="background:magenta" onclick="send('color=FF00FF')"></button>
  </div>
  <div class="row">
    <button class="btn" style="background:#8000FF" onclick="send('color=8000FF')"></button>
    <button class="btn" style="background:#00FF80" onclick="send('color=00FF80')"></button>
    <button class="btn" style="background:#FF0080" onclick="send('color=FF0080')"></button>
    <button class="btn" style="background:#808080" onclick="send('color=808080')"></button>
  </div>

  <!-- Modes -->
  <div class="row">
    <button class="btn" style="background:#555" onclick="send('mode=flash')">FLASH</button>
    <button class="btn" style="background:#555" onclick="send('mode=strobe')">STROBE</button>
  </div>
  <div class="row">
    <button class="btn" style="background:#555" onclick="send('mode=fade')">FADE</button>
    <button class="btn" style="background:#555" onclick="send('mode=smooth')">SMOOTH</button>
  </div>
</body>
</html>
)rawliteral";