const char index_html[] PROGMEM = R"(<!DOCTYPE HTML><html>
<head>
  <meta name='viewport' content='width=device-width, initial-scale=1'>
  <title>Linienroboter</title>
  <style>
    * {box-sizing: border-box; font-size: 16px;}
    html {font-family: Arial; width: 100%; height: 100%;}
    body {display: flex; flex-direction: column; flex: 1; max-height: 100%;}
    header {display: flex; flex-direction: row; flex: 1; align-items: center; justify-content: flex-start; height: 70px; width: 100%; background-color: white; padding: 10px; border-bottom: 1px solid #EFF0F1;}
    header h1 {color:#626A73; margin: 0 10px; font-size: x-large;}
    #headericon {width: 20%;}
    #wago_logo  {fill: #6EC800; width: 100%; height: 100%;}
    main {display: flex; flex-direction: column; flex: 1; align-items: center; justify-content:flex-start;}
    #controller {display: flex; flex-direction: column; flex: 1; margin: 60px 0;}
    .control {display: flex; flex-direction:row; flex: 1; justify-content:center; height: 100%; width: 100%;}
    .btn_steuerung {width: 20%; height: 20%; margin: 2%; fill: #6EC800;}
    #btn_hupe {width: 10%; height: 10%; margin: 8%; fill: #6EC800;}
    #modus_auswahl {display: flex; flex-direction: column; flex: 1;}
    .switch {position: relative; display: inline-block; width: 60px; height: 34px;}
    .switch input {opacity: 0; width: 0; height: 0;}
    .slider {position: absolute; cursor: pointer; top: 0; left: 0; right: 0; bottom: 0; background-color: #DEDFE1; -webkit-transition: .4s; transition: .4s; border-radius: 34px;}
    .slider:before {position: absolute; content: ''; height: 26px; width: 26px; left: 4px; bottom: 4px; background-color: white; -webkit-transition: .4s; transition: .4s; border-radius: 50%;}
    input:checked + .slider {background-color: #6EC800;}
    input:checked + .slider:before {-webkit-transform: translateX(26px); -ms-transform: translateX(26px); transform: translateX(26px);}
    td {width: 100px; height: 50px; text-align: center;}
  </style>
</head>
<body>
  <header>
    <div id='headericon'>
      <svg id='wago_logo' version='1.1' id='Ebene_1' xmlns='http://www.w3.org/2000/svg' xmlns:xlink='http://www.w3.org/1999/xlink' x='0px' y='0px'
	      viewBox='0 0 1227.4 595.3' style='enable-background:new 0 0 1227.4 595.3;' xml:space='preserve'>
          <path class='st0' d='M748.4,287.6v19.7h27.6V325h-55.1v-55.1H839v-63H681.4c-13.1,0-23.6,10.6-23.6,23.6v133.9
            c0,13.1,10.6,23.6,23.6,23.6h133.9c13.1,0,23.6-10.6,23.6-23.6v-76.8L748.4,287.6L748.4,287.6z'/>
          <path class='st0' d='M1018.6,206.8H884.7c-13.1,0-23.6,10.6-23.6,23.6v133.9c0,13.1,10.6,23.6,23.6,23.6h133.9
            c13.1,0,23.6-10.6,23.6-23.6V230.4C1042.2,217.4,1031.6,206.8,1018.6,206.8 M979.2,325h-55.1v-55.1h55.1V325z'/>
          <path class='st0' d='M529.7,206.8L399.3,407.7h-22.6l29.3,45.2l96.7-45.2h-28.3l33.3-51.2h65V388h63V206.8L529.7,206.8L529.7,206.8
            z M528.2,325l44.5-68.5V325H528.2z'/>
          <polygon class='st0' points='256.1,388 310.8,303.8 310.8,388 386.5,388 517,187.1 537.4,187.1 508.1,141.9 411.3,187.1 
            441.1,187.1 373.8,290.7 373.8,206.8 298.7,206.8 244.2,290.6 244.2,206.8 181.2,206.8 181.2,388 	'/>
      </svg>
    </div>
    <div class='title'>
      <h1>Linienroboter</h1>
    </div>
  </header>
  <main>
    <div id='controller'>
      <div class='control'>
        <svg class='btn_steuerung' viewBox='0 0 512 512' height='10%' width='10%' ontouchstart='vorwaerts(event)' ontouchend='vorwaerts(event)'>
          <path d='M8 256C8 119 119 8 256 8s248 111 248 248-111 248-248 248S8 393 8 256zm292 116V256h70.9c10.7 0 16.1-13 8.5-20.5L264.5 121.2c-4.7-4.7-12.2-4.7-16.9 0l-115 114.3c-7.6 7.6-2.2 20.5 8.5 20.5H212v116c0 6.6 5.4 12 12 12h64c6.6 0 12-5.4 12-12z'/>
        </svg>
      </div>
      <div class='control'>
        <svg class='btn_steuerung' viewBox='0 0 512 512' height='10%' width='10%' ontouchstart='links_fahren(event)' ontouchend='links_fahren(event)'>
          <path d='M256 504C119 504 8 393 8 256S119 8 256 8s248 111 248 248-111 248-248 248zm116-292H256v-70.9c0-10.7-13-16.1-20.5-8.5L121.2 247.5c-4.7 4.7-4.7 12.2 0 16.9l114.3 114.9c7.6 7.6 20.5 2.2 20.5-8.5V300h116c6.6 0 12-5.4 12-12v-64c0-6.6-5.4-12-12-12z'/>
        </svg>
        <svg id='btn_hupe' viewBox='0 0 512 512' height='10%' width='10%' ontouchstart='hupe(event)' ontouchend='hupe(event)'>
          <path d='M576 240c0-23.63-12.95-44.04-32-55.12V32.01C544 23.26 537.02 0 512 0c-7.12 0-14.19 2.38-19.98 7.02l-85.03 68.03C364.28 109.19 310.66 128 256 128H64c-35.35 0-64 28.65-64 64v96c0 35.35 28.65 64 64 64h33.7c-1.39 10.48-2.18 21.14-2.18 32 0 39.77 9.26 77.35 25.56 110.94 5.19 10.69 16.52 17.06 28.4 17.06h74.28c26.05 0 41.69-29.84 25.9-50.56-16.4-21.52-26.15-48.36-26.15-77.44 0-11.11 1.62-21.79 4.41-32H256c54.66 0 108.28 18.81 150.98 52.95l85.03 68.03a32.023 32.023 0 0 0 19.98 7.02c24.92 0 32-22.78 32-32V295.13C563.05 284.04 576 263.63 576 240zm-96 141.42l-33.05-26.44C392.95 311.78 325.12 288 256 288v-96c69.12 0 136.95-23.78 190.95-66.98L480 98.58v282.84z'/>
        </svg>
        <svg class='btn_steuerung' viewBox='0 0 512 512' height='10%' width='10%' ontouchstart='rechts(event)' ontouchend='rechts(event)'>
          <path d='M256 8c137 0 248 111 248 248S393 504 256 504 8 393 8 256 119 8 256 8zM140 300h116v70.9c0 10.7 13 16.1 20.5 8.5l114.3-114.9c4.7-4.7 4.7-12.2 0-16.9l-114.3-115c-7.6-7.6-20.5-2.2-20.5 8.5V212H140c-6.6 0-12 5.4-12 12v64c0 6.6 5.4 12 12 12z'/>
        </svg>
      </div>
      <div class='control'>
        <svg class='btn_steuerung' viewBox='0 0 512 512' height='10%' width='10%' ontouchstart='rueckwaerts(event)' ontouchend='rueckwaerts(event)'>
          <path d='M504 256c0 137-111 248-248 248S8 393 8 256 119 8 256 8s248 111 248 248zM212 140v116h-70.9c-10.7 0-16.1 13-8.5 20.5l114.9 114.3c4.7 4.7 12.2 4.7 16.9 0l114.9-114.3c7.6-7.6 2.2-20.5-8.5-20.5H300V140c0-6.6-5.4-12-12-12h-64c-6.6 0-12 5.4-12 12z'/>
        </svg>
      </div>
    </div>
    <div id='modus_auswahl'>
      <table class='modus_tabelle'>
        <tr>
          <td>
            <label>Steuern</label>
          </td>
          <td>
            <label class='switch'>
              <input type='checkbox' id='cb_manuelle_steuerung' onclick='aendere_modus(id)' checked>
              <span class='slider'></span>
            </label>
          </td>
        </tr>
        <tr>
          <td>
            <label>Linie folgen</label>
          </td>
          <td>
            <label class='switch'>
              <input type='checkbox' id='cb_folge_linie' onclick='aendere_modus(id)'>
              <span class='slider'></span>
            </label>
          </td>
        </tr>
        <tr>
          <td>
            <label>Licht folgen</label>
          </td>
          <td>
            <label class='switch'>
              <input type='checkbox' id='cb_folge_licht' onclick='aendere_modus(id)'>
              <span class='slider'></span>
            </label>
          </td>
        </tr>
      </table>

  </main>
  <footer>    
  </footer>
  <script>
    function vorwaerts(event) {
      let vorwaerts
      if (event.type == "touchstart") {
        vorwaerts = 1 
      } else {
        vorwaerts = 0
      }
      // fetch('http://192.168.4.1/vorwaerts', {
      fetch('/vorwaerts', {
      method: 'POST',
      headers: {'Content-type': 'application/json'},
      body: "{'vorwaerts': " + vorwaerts + "}"
      })
    }

    function rueckwaerts(event) {
      let rueckwaerts
      if (event.type == "touchstart") {
        rueckwaerts = 1 
      } else {
        rueckwaerts = 0
      }
      fetch('/rueckwaerts', {
      method: 'POST',
      headers: {'Content-type': 'application/json'},
      body: "{'rueckwaerts': " + rueckwaerts + "}"
      })
    }

    function links_fahren(event) {
      let links
      console.log("links")
      if (event.type == "touchstart") {
        links = 1 
      } else {
        links = 0
      }
      fetch('/links', {
      method: 'POST',
      headers: {'Content-type': 'application/json'},
      body: "{'links': " + links + "}"
      })
    }

    function rechts(event) {
      let rechts
      if (event.type == "touchstart") {
        rechts = 1 
      } else {
        rechts = 0
      }
      fetch('/rechts', {
      method: 'POST',
      headers: {'Content-type': 'application/json'},
      body: "{'rechts': " + rechts + "}"
      })
    }

    function hupe(event) {
      let hupe
      if (event.type == "touchstart") {
        hupe = 1 
      } else {
        hupe = 0
      }
      fetch('/hupe', {
      method: 'POST',
      headers: {'Content-type': 'application/json'},
      body: "{'hupe': " + hupe + "}"
      })
    }

    function aendere_modus(id) {

      let checkboxes = ['cb_folge_linie', 'cb_folge_licht', 'cb_manuelle_steuerung']
      checkboxes = checkboxes.filter(element => element !== id)

      let current_checkbox = document.getElementById(id)
      let buttons = document.getElementsByClassName('btn_steuerung')

      // uncheck other checkboxes
      checkboxes.forEach(element => {
        var checkbox = document.getElementById(element)
        checkbox.checked = false
      });

      // activate/deactivate manual control panel
      let color;
      let ptrEvents;
      let cb_manual = document.getElementById('cb_manuelle_steuerung')
      if (cb_manual.checked == true) {
        color = '#6EC800'
        ptrEvents = 'auto'
      }
      else {
        color = '#DEDFE1'
        ptrEvents = 'none'
      }
      Array.from(buttons).forEach((el) => {
          el.style.fill = color
          el.style.pointerEvents = ptrEvents
        });
        document.getElementById('btn_hupe').style.fill = color
        document.getElementById('btn_hupe').style.pointerEvents = ptrEvents

      // call rest api
      let api_endpoint
      if (id == 'cb_folge_linie') {
        api_endpoint = 'linie'
      } else if (id == 'cb_folge_licht') {
        api_endpoint = 'licht'    
      } else if (id == 'cb_manuelle_steuerung') {
        api_endpoint = 'manuell'
      }

      fetch('/' + api_endpoint, {
      method: 'GET',
      headers: {'Content-type': 'application/json'}
      })
    }
  </script>
</body>
</html>)";