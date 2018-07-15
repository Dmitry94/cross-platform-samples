const {
  ipcRenderer
} = require('electron')

ipcRenderer.on(
  'paramsChanged',
  (event, _numberOfPoints, _radius) => {
    numberOfPoints = _numberOfPoints
    radius = _radius
    redraw()
  })
  
  
function resizeCanvas(canvas) {
  const width = canvas.clientWidth;
  const height = canvas.clientHeight;
  if (canvas.width !== width || canvas.height !== height) {
    canvas.width = width;
    canvas.height = height;
  }
  return {width, height}
}

var numberOfPoints = 5
var radius = 100
  
function redraw() {
  var canvas = document.getElementById('mainCanvas')
  let {width, height} = resizeCanvas(canvas)

  var ctx = canvas.getContext('2d')  
  ctx.clearRect(0, 0, width, height);
  
  let x0 = width / 2
  let y0 = height / 2
  
  let phi = 0
  let x = x0
  let y = y0 + radius
  
  ctx.beginPath()
  ctx.moveTo(x, y)
  for (let i = 0; i < numberOfPoints; i++) {
    phi += 2 * Math.PI / numberOfPoints
    x = x0 + radius * Math.sin(phi)
    y = y0 + radius * Math.cos(phi)
    ctx.lineTo(x, y)
    ctx.arc(x, y, 2, 0, 2 * Math.PI, false)
  }
  ctx.stroke()
}

window.addEventListener('resize', function(e){
  e.preventDefault();
  redraw()
})

window.onload = function() {
  redraw()
}



